#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>

#include "cdf97.h"
#include "util.h"
/*
const char* cl_code = 
"#define a1 (-1.586134342)\n"
"#define a2 (-0.05298011854)\n"
"#define a3 (0.8829110762)\n"
"#define a4 (0.4435068522)\n"
"\n"
"#define k1 1.1496043988602418\n"
"#define k2 (1/1.1496043988602418)\n"
"\n"
"#define x(i, j) x[(i)*M+(j)]\n"
"#define tempbank(i, j) tempbank[(i)*M+(j)]\n"
"\n"
"/**\n"
"* predict and update\n"
"*\n"
"* computation is an even-odd update:\n"
"* first update all odd columns based on even columns, then update even columns based on odd columns\n"
"*//*\n"
"void fwt97_pd(\n"
"__global float *x,\n"
"__global float *tempbank,\n"
"int n, int m,\n"
"int N, int M)\n"
"{\n"
"int i, j;\n"
"\n"
"for (i=0; i<n; i++) {\n"
"// Predict 1\n"
"for (j=1; j<m-2; j+=2) {\n"
"x(i, j) += a1*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, m-1) += 2*a1*x(i, m-2);\n"
"// Update 1\n"
"for (j=2; j<m; j+=2) {\n"
"x(i, j) += a2*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, 0) += 2*a2*x(i, 1);\n"
"// Predict 2\n"
"for (j=1; j<m-2; j+=2) {\n"
"x(i, j) += a3*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, m-1) += 2*a3*x(i, m-2);\n"
"\n"
"// Update 2\n"
"for (j=2; j<m; j+=2) {\n"
"x(i, j) += a4*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, 0) += 2*a4*x(i, 1);\n"
"\n"
"for (j=0; j<m; j++) {\n"
"if (j%2==0)\n"
"tempbank(i, j/2) = k1*x(i, j);\n"
"else\n"
"tempbank(i, j/2+m/2) = k2*x(i, j);\n"
"}\n"
"\n"
"}\n"
"}\n"
"\n"
"/**\n"
"* de-interleave with transpose\n"
"*\n"
"* two different function (foo and bar) are used to move data\n"
"* between x and tempbank\n"
"* the reason for this design is to avoid 'spilling' in x when data\n"
"* is not square (n!=m)\n"
"*\n"
"**//*\n"
"void fwt97_dl_foo (\n"
"__global float *x,\n"
"__global float *tempbank,\n"
"int n, int m,\n"
"int N, int M)\n"
"{\n"
"int i, j, ii, jj;\n"
"// de-interleave and transpose\n"
"int tile_size = 8;\n"
"int tile_count = M/tile_size;\n"
"for (i=0; i<n; i+=tile_size) {\n"
"for (j=0; j<m; j+=tile_size) {\n"
"for (ii = 0; ii < tile_size; ii++){\n"
"for (jj = 0; jj < tile_size; jj++){\n"
"int new_i = i + ii;\n"
"int new_j = j + jj;\n"
"tempbank(new_j, new_i) = x(new_i, new_j);\n"
"}\n"
"}\n"
"}\n"
"}\n"
"}\n"
"\n"
"/**\n"
"* CDF9/7 wavelet transform\n"
"*\n"
"* this version assumes a column-based memory layout (Matlab default layout)\n"
"* so the wavelet along columns of the original matrix would be the transform\n"
"* along row in this code.\n"
"*\n"
"* input matrix: nxm (n cols, m rows)\n"
"*\n"
"**//*\n"
"void cdf97(\n"
"__global float *x,\n"
"__global float *tempbank,\n"
"int n, int m,\n"
"int level\n"
")\n"
"{\n"
"\n"
"int i;\n"
"int nlevel = (level < 0 ? -level : level);\n"
"\n"
"int N = n;\n"
"int M = m;\n"
"\n"
"int plevel = 1<<nlevel-1;\n"
"\n"
"// this means image is not big enough for transformation\n"
"// which we assume will not happen\n"
"if (N/plevel < 2 || M/plevel < 2)\n"
"return;\n"
"\n"
"if (level > 0) {\n"
"// forward DWT (1. col; 2. row)\n"
"for (i=0; i<nlevel; i++) {\n"
"\n"
"// Stage 1\n"
"fwt97_pd(x, tempbank, m, n, M, N);\n"
"// Stage 2\n"
"fwt97_dl_foo(tempbank, x, m, n, M, N);\n"
"\n"
"// Stage 3\n"
"fwt97_pd(x, tempbank, n, m, n, m);\n"
"\n"
"// Stage 4\n"
"fwt97_dl_foo(tempbank, x, n, m, N, M);\n"
"\n"
"n /= 2;\n"
"m /= 2;\n"
"}\n"
"}\n"
"else {\n"
"// backward DWT, omitted for the project\n"
";\n"
"}\n"
"}\n"
"\n"
"__kernel\n"
"void depth(\n"
"__global float *img,\n"
"__global float *tmp,\n"
"int n, int m,\n"
"int level) {\n"
"const int i = get_global_id(0);\n"
"cdf97(img+i*n*m, tmp+i*n*m, m, n, level);\n"
"\n"
"}\n"
;
*/
/*
static void cl_check(cl_int result)
{
	if (result == CL_SUCCESS)
		return;
	fprintf(stderr, "Error: %d\n", result);
	exit(result);
}
*/
int main(int argc, char *argv[]) {

    struct	timeval t1, t2, tr;
    double	elapsedTime = 0;

	if(argc<2) {
		printf("./dwt3d input.dat [level]");
		return -1;
	}

	char* fname = argv[1];
	char fname_out[50] = "dwt3d_out.dat";

	int level = 1;

	if (argc>2)
		level = atoi(argv[2]);

	int m, n, p;
	int i;
	float *img;
	float *tmp;

	// read the 3D image binary
	FILE* fin = fopen(fname, "rb");
	fread(&m, 1, sizeof(int), fin);
	fread(&n, 1, sizeof(int), fin);
	fread(&p, 1, sizeof(int), fin);

	img = (float*)calloc(n*m*p, sizeof(float));
	tmp = (float*)calloc(n*m*p, sizeof(float));

	fread (img, n*m*p, sizeof(float), fin);
	fclose(fin);
	//-------------------------------------------------------// OpenCL
	// Use this to check the output of each API call
	/*
    cl_int status;  
     
    // Retrieve the number of platforms
    cl_uint numPlatforms = 0;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
	
	 // Allocate enough space for each platform
    cl_platform_id *platforms = NULL;
    platforms = (cl_platform_id*)malloc(
        numPlatforms*sizeof(cl_platform_id));
 
    // Fill in the platforms
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	cl_int platform_index = -1;
	char cBuffer[1024];
	for (i=0; i<numPlatforms; i++) {
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
		if (strstr(cBuffer, "NVIDIA") != NULL) {
			platform_index = i;
			break;
		}
	}
	
	if (platform_index < 0) {
		printf("Cannot find platforms support OpenCL.\n");
		return -1;
	}
	else {
		printf("Selected platform '%s'. %d\n", cBuffer, platform_index);
	}
	
	// Retrieve the number of devices
    cl_uint numDevices = 0;
    status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_GPU, 0, 
        NULL, &numDevices);

    // Allocate enough space for each device
    cl_device_id *devices;
    devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));

	// Fill in the devices 
    status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

    // Create a context and associate it with the devices
    cl_context context;
    context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);	
	
	 // Create a command queue and associate it with the device 
    cl_command_queue cmdQueue;
    cmdQueue = clCreateCommandQueue(context, devices[0], 0, &status);
	
	cl_program program;
	
	program = clCreateProgramWithSource(context, 1,
		&cl_code,
		NULL, NULL);
	if (clBuildProgram(program, numDevices, devices, "", NULL, NULL) != CL_SUCCESS)
	{
		size_t size;
		char *log;
		cl_check(clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &size));
		log = (char*)malloc(size + 1);
		cl_check(clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, size, log, NULL));
		log[size] = 0;
		fprintf(stderr, "Build error: %s\n", log);
		free(log);
		exit(-1);
	}
	*/
	/*#ifndef USE_BIN
	status = utilProgramFromFile("ocd.cl", context, numDevices, devices, &program);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	/*
	status = utilProgramToBinary("ocd_cl.bin", &program);
#else
	status = utilProgramFromBinary("ocd_cl.bin", context, numDevices, devices, &program);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from binary, code %d\n", status);
		return -1;
	}
#endif
*/
/*
	cl_kernel kernel;
	kernel = clCreateKernel(program, "ocd", &status);
	printf("Got Kernel\n");
	
	//Create OpenCL buffers
	cl_mem cl_tmp;
	cl_mem cl_img;
	
	cl_img = clCreateBuffer(context, CL_MEM_READ_ONLY, n*m*sizeof(float), NULL, &status);
	cl_tmp = clCreateBuffer(context, CL_MEM_READ_ONLY, n*m*sizeof(float), NULL, &status);
	
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_img);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_tmp);
	status = clSetKernelArg(kernel, 2, sizeof(int), &n);
	status = clSetKernelArg(kernel, 3, sizeof(int), &m);
	status = clSetKernelArg(kernel, 4, sizeof(int), &level);
	
	status = clEnqueueWriteBuffer(cmdQueue, cl_img, CL_FALSE, 0, n*m*sizeof(float), img, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, cl_tmp, CL_FALSE, 0, n*m*sizeof(float), tmp, 0, NULL, NULL);
	
	size_t work_size[1] = {0};
	size_t group_size[1] = {1};
	
	work_size[0] = 1*p;
	*/
  
	/*
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, work_size, group_size, 0, NULL, NULL);
*/
    gettimeofday(&t1, NULL);

	// 3D DWT transform, which is 2D DWT for each image slices
	for (i=0; i<p; i++)
		cdf97(img+i*n*m, tmp+i*n*m, m, n, level);

    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);
    printf("Execute time: %.2f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	FILE* fout = fopen(fname_out, "wb+");
	fwrite(&m,	 sizeof(int), 1, fout);
	fwrite(&n, 	 sizeof(int), 1, fout);
	fwrite(&p, 	 sizeof(int), 1, fout);
	fwrite(img,  sizeof(float), n*m*p, fout);
	fclose(fout);

	free(img);
	free(tmp);

	return 0;
}
