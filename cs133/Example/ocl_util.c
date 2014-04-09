
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

cl_int utilProgramFromFile(
		const char* filename, 
		cl_context context,
		int numDevices,
		cl_device_id *devices,
		cl_program *program
		) 
{
	FILE* fin = fopen(filename, "r");

	if (!fin) {
		printf("file %s doesn't exist\n", filename);
		return -1;	
	}

	fseek(fin, 0L, SEEK_END);
	size_t sz = ftell(fin);
	rewind(fin);

	char *prog_src = (char*)malloc(sizeof(char)*sz+1);
	fread(prog_src, sizeof(char), sz, fin);

	fclose(fin);

	cl_int status;

    *program = clCreateProgramWithSource(context, 1, (const char**)&prog_src, NULL, &status); 
	status = clBuildProgram(*program, numDevices, devices, NULL, NULL, NULL);

	free(prog_src);
	return status;
}

cl_int utilProgramToBinary(
		const char* filename,
		cl_program *program
		)
{
	cl_int status;

	FILE* fout = fopen(filename, "wb+");

	cl_uint bin_cnt = 0;
	status = clGetProgramInfo(*program, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &bin_cnt, NULL);
	printf("%d devices compiled with program\n", bin_cnt);

	if (status != CL_SUCCESS) {
		printf("get program num_devices error\n");
		return status;
	}
	else if (bin_cnt < 1) {
		printf("no available program\n");
		return -1;
	}

	size_t* bin_sizes = (size_t*)malloc(bin_cnt*sizeof(size_t));
	status = clGetProgramInfo(*program, CL_PROGRAM_BINARY_SIZES, bin_cnt*sizeof(size_t), bin_sizes, NULL);
	if (status != CL_SUCCESS) {
		printf("get program bin_sizes error\n");
		return status;
	}

	char** binaries = (char**)malloc(bin_cnt*sizeof(char*));
	int i;
	for (i=0; i<bin_cnt; i++) {
		binaries[i] = (char*)malloc(bin_sizes[i]*sizeof(char));
	}
	// ? how to specify the param_value_length
	status = clGetProgramInfo(*program, CL_PROGRAM_BINARIES, 0, binaries, NULL);
	if (status != CL_SUCCESS) {
		printf("get program binaries error\n");
		return status;
	}

	// start writing program binaries
	size_t ferr = 0;
	fwrite(&bin_cnt, sizeof(bin_cnt), 1, fout);
	for (i=0; i<bin_cnt; i++) {
		ferr = fwrite(&bin_sizes[i], sizeof(size_t), 1, fout);
		ferr = fwrite(binaries[i], 1, bin_sizes[i], fout);
	}
	fclose(fout);

	for (i=0; i<bin_cnt; i++) {
		free(binaries[i]);
	}
	free(binaries);
	free(bin_sizes);
}

cl_int utilProgramFromBinary(
		const char* filename,		
		cl_context context,
		int numDevices,
		cl_device_id *devices,
		cl_program *program
		)
{
	cl_uint bin_cnt = 0;
	size_t*	bin_sizes;
	unsigned char**	binaries;

	FILE* fin = fopen(filename, "rb");
	if (!fin) {
		printf("file %s doesn't exist\n", filename);
		return -1;	
	}

	fread(&bin_cnt, sizeof(cl_uint), 1, fin);

	if (bin_cnt != numDevices) {
		// Panic
		printf("this should not happen...\n");
		return -1;
	}
	bin_sizes	= (size_t*)malloc(bin_cnt*sizeof(size_t));
	binaries	= (unsigned char**)malloc(bin_cnt*sizeof(unsigned char*));

	int i;
	size_t ferr = 0;
	for (i=0; i<bin_cnt; i++) {
		ferr = fread(&bin_sizes[i], sizeof(size_t), 1, fin);
		binaries[i] = (unsigned char*)malloc((bin_sizes[i]+1)*sizeof(unsigned char));
		ferr = fread(binaries[i], 1, bin_sizes[i], fin);
	}
	fclose(fin);
	
	cl_int status = CL_SUCCESS;

	*program = clCreateProgramWithBinary(context, numDevices, devices, bin_sizes, (const unsigned char**)binaries, NULL, &status);

	if (status != CL_SUCCESS) {
		printf("create program from binaries error\n");
		return status;
	}
	status = clBuildProgram(*program, numDevices, devices, NULL, NULL, NULL);

	if (status != CL_SUCCESS) {
		printf("get build program error\n");
		return status;
	}

	for (i=0; i<bin_cnt; i++) {
		free(binaries[i]);
	}
	free(binaries);
	free(bin_sizes);

	return status;
}

