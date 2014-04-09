#ifndef OCL_UTIL_H
#define OCL_UTIL_H

cl_int utilProgramFromFile( const char*, cl_context, int, cl_device_id *, cl_program *);

cl_int utilProgramToBinary( const char* filename, cl_program *program );

cl_int utilProgramFromBinary( const char* filename,		cl_context context, int numDevices, cl_device_id *devices, cl_program *program);
#endif
