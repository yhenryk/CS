
   // ***** AUTO-GENERATED FILE FROM smith_waterman.cnc, DO NOT MODIFY!  *****//

#ifndef _CNC_STEPS_H
#define _CNC_STEPS_H

#include "cnc_mm.h"
#include "Context.h"

#define Step_smith_waterman 0
#define Step_read_file2 1
#define Step_read_file1 2
#define Step_score_gen 3

#pragma hc continuable
void * smith_waterman_gets(char * tag, Context * context, Step * step);
#ifdef _DATA_DRIVEN
void smith_waterman_dependencies(char * tag, Context * context, Step * step);
#endif
#pragma hc continuable
void * read_file2_gets(char * tag, Context * context, Step * step);
#ifdef _DATA_DRIVEN
void read_file2_dependencies(char * tag, Context * context, Step * step);
#endif
#pragma hc continuable
void * read_file1_gets(char * tag, Context * context, Step * step);
#ifdef _DATA_DRIVEN
void read_file1_dependencies(char * tag, Context * context, Step * step);
#endif
#pragma hc continuable
void * score_gen_gets(char * tag, Context * context, Step * step);
#ifdef _DATA_DRIVEN
void score_gen_dependencies(char * tag, Context * context, Step * step);
#endif

#pragma hc continuable
void prescribeStep(char* stepName, char* stepTag, Context* context);

#endif /*_CNC_STEPS_H*/

