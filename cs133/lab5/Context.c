
   // ***** AUTO-GENERATED FILE FROM smith_waterman.cnc, DO NOT MODIFY!  *****//

#include "cnc_mm.h"
#include "Context.h"

Context* initGraph()
{
	int i;
	Context* CnCGraph = (Context*) cnc_malloc (1 * sizeof(Context));


	CnCGraph->file_name_2 = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));
	CnCGraph->string_2 = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));
	CnCGraph->file_name_1 = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));
	CnCGraph->string_1 = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));
	CnCGraph->alignment_score_matrix = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));
	CnCGraph->score = (ItemCollectionEntry**) cnc_malloc (TABLE_SIZE * sizeof(ItemCollectionEntry*));

	for(i=0; i<TABLE_SIZE; i++){
		CnCGraph->file_name_2[i] = CnCGraph->string_2[i] = CnCGraph->file_name_1[i] = CnCGraph->string_1[i] = CnCGraph->alignment_score_matrix[i] = CnCGraph->score[i] =  NULL;
	}
	return CnCGraph;
}

void deleteGraph(Context* CnCGraph)
{
	cnc_free(CnCGraph->file_name_2);
	cnc_free(CnCGraph->string_2);
	cnc_free(CnCGraph->file_name_1);
	cnc_free(CnCGraph->string_1);
	cnc_free(CnCGraph->alignment_score_matrix);
	cnc_free(CnCGraph->score);

	cnc_free(CnCGraph);
}

