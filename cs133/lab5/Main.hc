
   /***** AUTO-GENERATED FILE from file smith_waterman.cnc - only generated if file does not exist (on running cncc_t the first time) - feel free to edit *****/

#include "Dispatch.h"
#include <string.h>

int main(int argc, char** argv)
{

	Context* context = initGraph();
	//Note: always put pointers to the values you intend o use; get these values in a variable of type pointer to the type of the item
	finish{
		/* Suggested code for writting items from environment:
		int * val = malloc(sizeof(int)); val[0] = 0;

		char * in_tag = createTag(3, i, j, k);

		Put((void*) val, in_tag, context->Ai);
		prescribeStep("stepName", in_tag, context);
		*/
	}

	//get results - example of getting a tile of type int*:
	//CNC_GET((void*) & (val), in_tag, context->Ai, NULL);

	deleteGraph(context);
}

