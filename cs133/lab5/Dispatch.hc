
   // ***** AUTO-GENERATED FILE FROM smith_waterman.cnc, DO NOT MODIFY!  *****//

#include "Dispatch.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

void dispatchStep(Step* step){
	switch(step->stepID){
	case Step_smith_waterman: 
	     async  IN(step){ smith_waterman_gets(step->tag, (Context*)step->context, step); };  break;
	case Step_read_file2: 
	     async  IN(step){ read_file2_gets(step->tag, (Context*)step->context, step); };  break;
	case Step_read_file1: 
	     async  IN(step){ read_file1_gets(step->tag, (Context*)step->context, step); };  break;
	case Step_score_gen: 
	     async  IN(step){ score_gen_gets(step->tag, (Context*)step->context, step); };  break;
	default:  CNC_ASSERT(0, "Attempt at executing an unknown step!");
	}
}

void prescribeStep(char* stepName, char* stepTag, Context* context){
	Step* step = cnc_malloc(sizeof(Step));
	step->tag = stepTag;
	step->context = context;
	step->nxt = NULL;
	step->dependencies = NULL;
	step->failed = NULL;
	if(!strncmp(stepName, "smith_waterman\0", 14)){
		step->stepID = Step_smith_waterman;
		#ifndef _DATA_DRIVEN
			dispatchStep(step);
		#else
			smith_waterman_dependencies(step->tag, (Context*)step->context, step);
			int status = checkDependencies(step);
			if(status == CNC_SUCCESS)
				dispatchStep(step);
		#endif
		return;
	}
	if(!strncmp(stepName, "read_file2\0", 10)){
		step->stepID = Step_read_file2;
		#ifndef _DATA_DRIVEN
			dispatchStep(step);
		#else
			read_file2_dependencies(step->tag, (Context*)step->context, step);
			int status = checkDependencies(step);
			if(status == CNC_SUCCESS)
				dispatchStep(step);
		#endif
		return;
	}
	if(!strncmp(stepName, "read_file1\0", 10)){
		step->stepID = Step_read_file1;
		#ifndef _DATA_DRIVEN
			dispatchStep(step);
		#else
			read_file1_dependencies(step->tag, (Context*)step->context, step);
			int status = checkDependencies(step);
			if(status == CNC_SUCCESS)
				dispatchStep(step);
		#endif
		return;
	}
	if(!strncmp(stepName, "score_gen\0", 9)){
		step->stepID = Step_score_gen;
		#ifndef _DATA_DRIVEN
			dispatchStep(step);
		#else
			score_gen_dependencies(step->tag, (Context*)step->context, step);
			int status = checkDependencies(step);
			if(status == CNC_SUCCESS)
				dispatchStep(step);
		#endif
		return;
	}
	printf("Step %s not defined\n", stepName);
	assert(0);
}

