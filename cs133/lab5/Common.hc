
	/***** AUTO-GENERATED FILE. Do not modify unless tag functions are missing and the code generated as suggestion is in comment. *****/
	/***** WARNING: this file WILL BE OVERWRITTEN on each call of cncc_t smith_waterman.cnc *****/

#include "Common.h"

/*
smith_waterman dependency adding implementation
*/

void smith_waterman_dependencies(char * tag, Context * context, Step* step){
	int k = getTag(tag, 0);

	char* tagstring_20 = createTag(1, k);
	addDependency( tagstring_20, context->string_2, step);

	char* tagstring_11 = createTag(1, k);
	addDependency( tagstring_11, context->string_1, step);

	char* tagalignment_score_matrix2 = createTag(1, k);
	addDependency( tagalignment_score_matrix2, context->alignment_score_matrix, step);

	char* tagscore3 = createTag(1, k);
	addDependency( tagscore3, context->score, step);
	addDependencyFinished(step);
}

/*
smith_waterman gets implementation
*/

void* smith_waterman_gets(char * tag, Context * context, Step* step){

	int k = getTag(tag, 0);

	char* string_20;
#ifndef _DATA_DRIVEN
	char* tagstring_20 = createTag(1, k);
	CNC_GET((void**) & (string_20), tagstring_20, context->string_2, step);
#else
	getNextData((void**) & (string_20), step);
#endif

	char* string_11;
#ifndef _DATA_DRIVEN
	char* tagstring_11 = createTag(1, k);
	CNC_GET((void**) & (string_11), tagstring_11, context->string_1, step);
#else
	getNextData((void**) & (string_11), step);
#endif

	char* alignment_score_matrixtemp2;
	char alignment_score_matrix2;
#ifndef _DATA_DRIVEN
	char* tagalignment_score_matrix2 = createTag(1, k);
	CNC_GET((void**) & (alignment_score_matrixtemp2), tagalignment_score_matrix2, context->alignment_score_matrix, step);
#else
	getNextData((void**) & (alignment_score_matrixtemp2), step);
#endif
	alignment_score_matrix2 = alignment_score_matrixtemp2[0];

	int** score3;
#ifndef _DATA_DRIVEN
	char* tagscore3 = createTag(1, k);
	CNC_GET((void**) & (score3), tagscore3, context->score, step);
#else
	getNextData((void**) & (score3), step);
#endif
	smith_waterman( k, string_20, string_11, alignment_score_matrix2, score3, context );
	return 0;
}

/*
read_file2 dependency adding implementation
*/

void read_file2_dependencies(char * tag, Context * context, Step* step){
	int i = getTag(tag, 0);

	char* tagfile_name_20 = createTag(1, i);
	addDependency( tagfile_name_20, context->file_name_2, step);
	addDependencyFinished(step);
}

/*
read_file2 gets implementation
*/

void* read_file2_gets(char * tag, Context * context, Step* step){

	int i = getTag(tag, 0);

	char* file_name_20;
#ifndef _DATA_DRIVEN
	char* tagfile_name_20 = createTag(1, i);
	CNC_GET((void**) & (file_name_20), tagfile_name_20, context->file_name_2, step);
#else
	getNextData((void**) & (file_name_20), step);
#endif
	read_file2( i, file_name_20, context );
	return 0;
}

/*
read_file1 dependency adding implementation
*/

void read_file1_dependencies(char * tag, Context * context, Step* step){
	int i = getTag(tag, 0);

	char* tagfile_name_10 = createTag(1, i);
	addDependency( tagfile_name_10, context->file_name_1, step);
	addDependencyFinished(step);
}

/*
read_file1 gets implementation
*/

void* read_file1_gets(char * tag, Context * context, Step* step){

	int i = getTag(tag, 0);

	char* file_name_10;
#ifndef _DATA_DRIVEN
	char* tagfile_name_10 = createTag(1, i);
	CNC_GET((void**) & (file_name_10), tagfile_name_10, context->file_name_1, step);
#else
	getNextData((void**) & (file_name_10), step);
#endif
	read_file1( i, file_name_10, context );
	return 0;
}

/*
score_gen dependency adding implementation
*/

void score_gen_dependencies(char * tag, Context * context, Step* step){
	int length_file2 = getTag(tag, 0);
	addDependencyFinished(step);
}

/*
score_gen gets implementation
*/

void* score_gen_gets(char * tag, Context * context, Step* step){

	int length_file2 = getTag(tag, 0);
	score_gen( length_file2, context );
	return 0;
}


