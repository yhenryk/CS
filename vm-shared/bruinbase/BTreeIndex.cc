/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
	treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
    RC rc = pf.open(indexname, mode);
	return rc;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    RC rc = pf.close();
	return rc;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	//return value
	RC rc;
	//special case: empty tree
	if(treeHeight == 0)
	{
		//create an empty node
		BTLeafNode ln;
		ln.initialize();
		//insert the entry
		ln.insert(key, rid);
		//figure out where to write to
		//page 0 is for metadata
		rootPid = pf.endPid();
		if(rootPid == BTreeIndex::METADATA_PAGE)
		{
			rootPid++;
		}
		//write the entry to the page file
		rc = ln.write(rootPid, pf);
	} else
	{
		PageId retPid;
		int retKey;
		rc = insertHelper(key, rid, rootPid, 1, retPid, retKey);
	}
	if(smallestKey == -1 || key < smallestKey)
	{
		smallestKey = key;
	}
    return rc;
}

RC BTreeIndex::insertHelper(int key, const RecordId& rid, PageId cpid, int level, PageId& spid, int& spk)
{
	RC rc;
	//base case: Leaf node
	if(level == treeHeight)
	{
		//read the node information
		BTLeafNode ln;
		ln.initialize();
		ln.read(cpid, pf);
		//attempt to insert
		rc = ln.insert(key, rid);
		//Successful insert(node not full)
		if(rc == 0)
		{
			//write to page
			ln.write(cpid, pf);
			return rc;
		} 
		else //node full
		{
			//create a sibling node
			BTLeafNode sn;
			sn.initialize();
			//return key from insertAndSplit
			int siblingKey;
			//insert and split
			ln.insertAndSplit(key, rid, sn, siblingKey);
			//write the sibling node
			PageId sib = pf.endPid();
			sn.write(sib, pf);
			//Set the leaf node's pointer
			ln.setNextNodePtr(sib);
			//write the leaf node
			ln.write(cpid, pf);
			//set the return information
			spid = sib;
			spk = siblingKey;
			//If this was a root, we now have to create a new root
			if(level == 1)
			{
				rc = this->newRoot(siblingKey, cpid, sib);
			}
			return rc;
		}
		
	}
	else //recursive case: Non leaf node
	{
		//Initialize and read
		BTNonLeafNode n;
		n.initializeRoot(-1, -1, -1);
		n.read(cpid, pf);
		//splitPid and splitKey are the return values for the upcoming
		//recursive call. npid is the return value for locateChildPtr
		PageId npid, splitPid;
		int splitKey;
		n.locateChildPtr(key, npid);
		//Recursive Iteration
		rc = this->insertHelper(key, rid, npid, level+1, splitPid, splitKey);
		//If helper returned node full
		if(rc == RC_NODE_FULL)
		{
			//Attempt to insert using the return information
			rc = n.insert(splitKey, splitPid);
			//Success
			if(rc == 0)
			{
				//Write the non leaf node and we're done
				n.write(cpid, pf);
				return rc;
			} 
			else //Full Node
			{
				//create sibling and initialize
				RC rc = RC_NODE_FULL;
				BTNonLeafNode sn;
				sn.initializeRoot(-1,-1,-1);
				//Insert and split
				int midKey;
				n.insertAndSplit(splitKey, splitPid, sn, midKey);
				//write the sibling to endPid
				PageId sibId = pf.endPid();
				sn.write(sibId, pf);
				//if this is a root node
				if(level == 1)
				{
					rc = this->newRoot(midKey, cpid, sibId);
				}
				//set return info
				spid = sibId;
				spk = midKey;
				return rc;
			} 
		}
		else //inserted properly
		{
			return rc;
		}
	}
}

RC BTreeIndex::newRoot(int key, PageId lpid, PageId rpid)
{
	//initialize a new root node
	BTNonLeafNode n;
	RC rc = n.initializeRoot(lpid, key, rpid);
	//set the new root pid
	rootPid = pf.endPid();
	//write the new root
	n.write(rootPid, pf);
	//set the new tree height
	treeHeight++;
	return rc;
}
/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	BTNonLeafNode nl;
	nl.initializeRoot(0, -1, -1);
	PageId pid_to_read = rootPid;
	//Keet searching until we get to the leaf
	int currentHeight = -1;
	while(currentHeight < treeHeight)
	{
		RC non_leaf_read = nl.read(pid_to_read, pf);
		if(non_leaf_read != 0) {
			fprintf(stderr, "Error in NON leaf_node read. pid = %d\n", pid_to_read);
			return non_leaf_read;
		}
		//look for the next pid to follow, based off the key to find
		RC locate_status = nl.locateChildPtr(searchKey, pid_to_read);
		if(locate_status != 0){
			fprintf(stderr, "Couldn't find key = %d. pid = %d", searchKey, pid_to_read);
			return locate_status;
		}
		currentHeight++;
	}
	BTLeafNode lf;
	lf.initialize();
	//read the pagefile and make sure there are no errors.
	RC leaf_read_status = lf.read(pid_to_read, pf);
	if(leaf_read_status != 0)
	{
		fprintf(stdout, "Error in lf node read. pid =%d", pid_to_read);
		return leaf_read_status;
	}
	//Find the entry whose key value is larger than or equal to searchKey
	int located_eid;
	RC locate_status = lf.locate(searchKey, located_eid);
	//check that we have no errors
	if(locate_status != 0)
	{
		printf("BTreeIndex.locate: couldn't find %d in the leaf node of pid = %d\n", searchKey, pid_to_read);
		return locate_status;
	}
	cursor.pid = pid_to_read;
	cursor.eid = located_eid;
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	BTLeafNode lf;
	lf.initialize();
	
	PageId pid_to_read = cursor.pid;
	int entry_to_read = cursor.eid;
	RC read_status = lf.read(pid_to_read, pf);
	if(read_status != 0)
		return read_status;
	RC read_entry = lf.readEntry(entry_to_read, key, rid);
	if(read_entry != 0)
		return read_entry;
	entry_to_read++;
	if(entry_to_read >= lf.getKeyCount()){
		entry_to_read = BTLeafNode::LEAF_FIRST_ENTRY;
		cursor.pid = lf.getNextNodePtr();
		cursor.eid = entry_to_read;
		return 0;
	}
	else {
		cursor.eid = entry_to_read;
		return 0;
	}
}

RC BTreeIndex::readMetadata()
{
	//the open function must be called prior to this function being called.
	//this function must be called prior to any inserts or look ups
	
	
	//the file is empty
	if(pf.endPid() == 0)
	{
		//reset the rootPid and treeHeight just in case we're reusing this
		rootPid = -1;
		treeHeight = 0;
		smallestKey = -1;
		return 0;
	}
	//buffer for reading
	char buf[PageFile::PAGE_SIZE];
	pf.read(BTreeIndex::METADATA_PAGE, buf);
	int r = BTreeIndex::ROOT_PID_LOC;
	int h = BTreeIndex::HEIGHT_LOC;
	int s = BTreeIndex::SM_KEY_LOC;
	
	char* b = buf;
	int* bint = (int*) b;
	rootPid = *(bint + r/sizeof(int));
	treeHeight = *(bint + h/sizeof(int));
	smallestKey = *(bint+s/sizeof(int));
	
}
RC BTreeIndex::writeMetadata()
{
	//the open function must be called prior to this function being called.
	//this function must be called prior to the close function being called.
	
	char buffer[PageFile::PAGE_SIZE];
	
	int r = BTreeIndex::ROOT_PID_LOC;
	int h = BTreeIndex::HEIGHT_LOC;
	int s = BTreeIndex::SM_KEY_LOC;
	
	char* b = buffer;
	int* bint = (int*) b;
	*(bint + r/sizeof(int)) = rootPid;
	*(bint + h/sizeof(int)) = treeHeight;
	*(bint + s/sizeof(int)) = smallestKey;
	b = (char*) bint;
	
	RC rc = pf.write(BTreeIndex::METADATA_PAGE, buffer);
	return rc;
}
int BTreeIndex::getSmallestKey()
{
	return smallestKey;
}
