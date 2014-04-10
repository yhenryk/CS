#include "BTreeNode.h"

using namespace std;

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	//use PafeFile::read to read into buffer
	RC rc = pf.read(pid, buffer);
	return rc; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	//use PageFile::write to write from the buffer
	RC rc = pf.write(pid, buffer);
	return rc; }

	//An entry of all -1 means that it is empty.
	//The first entry is set to all -2 to signify that the node is a leaf node.
	//A -1 PageId in the last 4 bytes means it's the last leaf node.
	
/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
	int numkeys = 0;
	int limit = PageFile::PAGE_SIZE - sizeof(PageId);
	int key;
	RecordId rid;
	//Last 4 bytes are a PageID pointer to the next leaf node
	for(int index = LEAF_FIRST_ENTRY * LEAF_ENTRY_SIZE; index < limit; index += LEAF_ENTRY_SIZE)
	{
		convert(buffer, index, key, rid);
		//All -1's means empty entry
		if(rid.pid == -1 && rid.sid == -1 && key == -1)
			break;
		numkeys++;
	}
	return numkeys; }

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	int numkeys = this->getKeyCount();
	//chck if node is already full
	if(numkeys >= LEAF_MAX_ENTRIES)
	{
		return RC_NODE_FULL;
	}
	int eid;
	int okey;
	RecordId orid;
	
	//find the spot to insert
	RC rc = locate(key, eid);
	
	//no key >= search key, so insert at end
	if(rc != 0)
	{
		eid = numkeys;
	}
	//Move all chars in the buffer by the size of one leaf entry
	for(int i = numkeys * LEAF_ENTRY_SIZE - 1; i >= eid * LEAF_ENTRY_SIZE; i--)
	{
		buffer[i+LEAF_ENTRY_SIZE] = buffer[i];
	}
	char buf[LEAF_ENTRY_SIZE];
	//convert the (key, rid) pair to a char array
	convertToChar(key, rid, buf);
	
	//Insert the new entry
	for(int i = eid * LEAF_ENTRY_SIZE; i < eid * LEAF_ENTRY_SIZE + LEAF_ENTRY_SIZE; i++)
	{
		buffer[i] = buf[i - eid*LEAF_ENTRY_SIZE];
	}
	return 0; }

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	//Make sure the node is full
	if(getKeyCount() < LEAF_MAX_ENTRIES)
	{
		return RC_INVALID_FILE_FORMAT;
	}
	//The entry id of the last entry before the split
	int spid = (int) (LEAF_MAX_ENTRIES / 2 + 1);
	int oKey;
	RecordId oRid;
	int eKey = -1;
	RecordId eRid = {-1, -1};
	
	for(int i = spid * LEAF_ENTRY_SIZE; i < LEAF_MAX_ENTRIES * LEAF_ENTRY_SIZE; i += LEAF_ENTRY_SIZE)
	{
		convert(buffer, i, oKey, oRid);
		//copy over the entries that will no be sibling
		sibling.insert(oKey, oRid);
		
		//Assign the sibling key (will be changed if neccessary)
		if( i == spid*LEAF_ENTRY_SIZE)
		{	
			siblingKey = oKey;
		}
		
		char buf[LEAF_ENTRY_SIZE];
		convertToChar(eKey, eRid, buf);
		//Turn this entry in the current leaf to an empty entry
		for(int j = i; j < i + LEAF_ENTRY_SIZE; j++)
		{
			buffer[j] = buf[j - i];
		}
	}
	//set the sibling's node pointer
	sibling.setNextNodePtr(getNextNodePtr());
	//Now insert the new record
	if(key < siblingKey)
	{
		insert(key, rid);
		
	}
	else
	{
		sibling.insert(key, rid);
	}
	return 0; }

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int numKeys = getKeyCount();
	int oKey;
	RecordId oRid;
	for(int i = LEAF_FIRST_ENTRY; i < numKeys; i++)
	{
		convert(buffer, i * LEAF_ENTRY_SIZE, oKey, oRid);
		if(oKey >= searchKey)
		{
			eid = i;
			return 0;
		}
	}
	return RC_NO_SUCH_RECORD; }

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	if(eid < 0 || eid >= getKeyCount())
	{
		return RC_INVALID_CURSOR;
	}
	convert(buffer, eid*LEAF_ENTRY_SIZE, key, rid);
	return 0; }

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	int index = PageFile::PAGE_SIZE - sizeof(PageId);
	return (buffer[index] << 24) | (buffer[index+1] << 16) | (buffer[index+2] << 8) | buffer[index +3];}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	if(pid < 0)
	{
		return RC_INVALID_PID;
	}
	for(int i = PageFile::PAGE_SIZE - sizeof(PageId); i < PageFile::PAGE_SIZE; i++)
	{
		buffer[i] = (char) ( (pid >> ((3 - i) * 8)) & 0xFF );
	}
	return 0; }

void BTLeafNode::convert(char* buffer, int index, int& key, RecordId rid)
{
	rid.pid = (buffer[index] << 24) | (buffer[index+1] << 16) | (buffer[index+2] << 8) | buffer[index + 3];
	rid.sid = (buffer[index+4] << 24) | (buffer[index+5] << 16) | (buffer[index+6] << 8) | buffer[index+7];
	key = (buffer[index+8] << 24) | (buffer[index+9] << 16) | (buffer[index+10] << 8) | buffer[index+11];
}

void BTLeafNode::convertToChar(int key, RecordId rid, char* buf)
{
	for(int i = 0; i < 4; i++)
	{
		buf[i] = (char) ( (rid.pid >> ((3 - i)*8)) & 0xFF );
	}
	for(int i = 0; i < 4; i++)
	{
		buf[i+4] = (char) ( (rid.sid >> ((3 - i) * 8)) & 0xFF );
	}
	for(int i = 0; i < 4; i++)
	{
		buf[i+8] = (char) ((key >> ((3-i) * 8)) & 0xFF);
	}
}

void BTLeafNode::initialize()
{
	int ekey = -1;
	RecordId erid = {-1, -1};
	char buf[LEAF_ENTRY_SIZE];
	//set all entries to empty
	for(int i = 0; i < LEAF_MAX_ENTRIES; i++)
	{
		convertToChar(ekey, erid, buf);
		for(int j = 0; j < LEAF_ENTRY_SIZE; j++)
		{
			buffer[i*LEAF_ENTRY_SIZE+j] = buf[j];
		}
	}
	//Set the next node pointer to empty (i.e null pointer)
	for(int i = PageFile::PAGE_SIZE - sizeof(PageId); i < PageFile::PAGE_SIZE; i++)
	{
		buffer[i] = (char) ((ekey >> ((3 - i) * 8)) & 0xFF );
	}
}
//-----------------------------------------------------------------------------------------

PageId BTNonLeafNode::getStartPid() const { return * ((PageId*)data); }

void BTNonLeafNode::setStartPid(PageId pid) {
	PageId* start_pid = (PageId*) data;
	*start_pid = pid;
	
}

NonLeafData* BTNonLeafNode::getStartOfBuffer() {
	char* start = data + sizeof(PageId); //the first 4 bytes are reserved for Pid
	return (NonLeafData*) start;
	}
/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
 
 
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	//Copy the contents of the page with id pid from PageFile pf into the 
	//internal buffer. Open the page file
	RC read_status = pf.read(pid, data);
	return read_status; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
	//take the contents of the buffer and write it into the page file at the appropriate pid location
	RC write_status = pf.write(pid, data);
	return write_status; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	//Go through the buffer, convert things to NonLeafData structs, then count how many
	//until we get to pid = -1 or the end of the buffer
	NonLeafData* start = this->getStartOfBuffer();
	int count = 0;
	PageId start_pid = this->getStartPid();
	
	if(start_pid == -1)
	{
		return 0;
	}
	while(start->right_pid != -1 && (char*) start < data + PageFile::PAGE_SIZE) {
		count++;
		//This should increment the size of NonLeafData struct
		start++;
	}
	return count;}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
	//Make sure that we have enough room to insert another (key,pid) pair.
	int MAX_PAIR_COUNT = MAX_NONLEAFDATA;
	if(this->getKeyCount() >= MAX_PAIR_COUNT)
		return RC_NODE_FULL;
		
	//Go though the page and point to where 
	//find the position to insert the pid
	
	NonLeafData* start = this->getStartOfBuffer();
	int count = 0;
	while(start->right_pid != -1 && (char*) start < data + PageFile::PAGE_SIZE && start->key <key)
	{
		//this should increment the size of NonLeafData Struct
		start++;
		count++;
	}
	//shift everything down if necessary.
	if(start->right_pid != -1) {
		int num_elements = MAX_NONLEAFDATA - count;
		shift( (char*)start, num_elements);
	}
	//insert
	
	start->right_pid = pid;
	start->key = key;
	
	return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ 
	//insert the pair.
	RC insert_status = this->insert(key, pid);
	//go to the median value
	int num_keys = this->getKeyCount();
	int median_pos = num_keys/2;
	
	NonLeafData* start = this->getStartOfBuffer();
	//Insert everything on the right into sibling node
	//median_pos should be bounded via getKeyCount
	//except for the special case where there are zero pairs
	
	//Take the median value out, then insert its pid pointer as the start_pid for the sibling
	
	NonLeafData* median_pair = start + median_pos;
	midKey = median_pair->key;
	sibling.setStartPid(median_pair->right_pid);
	//Pointer to the first NonLeafData to start copying
	NonLeafData* start_copy = start + median_pos + 1;
	for(int i = 0; i < num_keys; i++)
	{
		//insert into the sibling
		int current_pid = (start_copy + i)->right_pid;
		int current_key = (start_copy + i)->key;
		
		RC insert_status = sibling.insert(key, pid);
		if(insert_status != 0) return insert_status;
		//Clear the current slot's value
		(start_copy+i)->right_pid = -1;
		(start_copy+i)->key = -1;
		
	}
	return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	//Look through the keys till we get a match
	NonLeafData* start = this->getStartOfBuffer();
	int i = 0;
	for(; i < this->getKeyCount(); i++)
	{
		//Search the key, if we find a match, record
		//the pid and then stop looking
		if( (start+i)->key > searchKey) {
			//needs to return the previous pid because the pid are attached to the right.
			//if we are the beginning of the list, we will return the start_pid
			if(i == 0){
				pid = this->getStartPid();
			}
			else
			{
				pid = (start + i - 1)->right_pid;
			}
			return 0;
		}
	}
	pid = (start + i -1)->right_pid;
	return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	NonLeafData* start = this->getStartOfBuffer();
	//Init the buffer(page) with default values. if pid = -1, it means that
	//this is the end of the buffer. Therefore we will set everything to -1;
	for(int i = 0; i < MAX_NONLEAFDATA; i++){
		(start+i)->right_pid = -1;
		(start+i)->key = -1;
	}
	//Add the values
	///Add the first pid value
	PageId* first_pid = (PageId*)data;
	*first_pid = pid1;
	start->right_pid = pid2;
	start->key = key;
	return 0; }

void BTNonLeafNode::shift(const char* buffer, unsigned int size) {
	NonLeafData* start = (NonLeafData*)buffer;
	//start with the end
	unsigned int STOP = 0;
	for(unsigned int i = size - 1; i > STOP; i--)
	{
		NonLeafData* end = ((NonLeafData*) buffer)+ i;
		NonLeafData* head = ((NonLeafData*)buffer) +i - 1;
		//copy the values
		end->right_pid = head->right_pid;
		end->key = head->key;
	}
}
