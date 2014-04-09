
   // ***** AUTO-GENERATED FILE FROM smith_waterman.cnc, DO NOT MODIFY!  *****//

#ifndef _CONTEXT
#define _CONTEXT

#include "DataDriven.h"
#include <hc.h>

struct Context
{
	ItemCollectionEntry ** file_name_2;
	ItemCollectionEntry ** string_2;
	ItemCollectionEntry ** file_name_1;
	ItemCollectionEntry ** string_1;
	ItemCollectionEntry ** alignment_score_matrix;
	ItemCollectionEntry ** score;
};
typedef struct Context Context;

struct Context* initGraph();
void deleteGraph(Context* CnCGraph);

#endif /*_CONTEXT*/

