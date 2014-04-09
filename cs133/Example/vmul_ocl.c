
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <CL/cl.h>

#include "ocl_util.h"

#define A(i, j) A[(i)*n+(j)]

int main(int argc, char** argv) {

    struct	timeval t1, t2, tr;
    double	elapsedTime = 0;
	int		n = 256;
	int		i, j;

	if (argc>1)
		n = atoi(argv[1]);

	float *A = (float*)malloc(n*n*sizeof(float));
	float *x = (float*)malloc(n*sizeof(float));
	float *y = (float*)malloc(n*sizeof(float));
	float *y_cl = (float*)malloc(n*sizeof(float));

	srand(time(NULL));

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			A(i, j) = (float)rand()/RAND_MAX;
		}
		x[i] = (float)rand()/RAND_MAX;
	}


	for (i=0; i<n; i++) {
		y[i] = 0;
		for (j=0; j<n; j++) {
			y[i] += A(i, j)*x[j];
		}
	}

	// OpenCL
	// Use this to check the output of each API call
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

	/*
    // Create a program with source code
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&vmul_cl, NULL, &status);

    // Build (compile) the program for the device
    status = clBuildProgram(program, numDevices, devices, 
        NULL, NULL, NULL);
	*/

	cl_program program;

#ifndef USE_BIN
	status = utilProgramFromFile("vmul.cl", context, numDevices, devices, &program);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	status = utilProgramToBinary("vmul_cl.bin", &program);
#else
	status = utilProgramFromBinary("vmul_cl.bin", context, numDevices, devices, &program);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from binary, code %d\n", status);
		return -1;
	}
#endif

	// Create the vector addition kernel
    cl_kernel kernel;
    kernel = clCreateKernel(program, "vmul", &status);

	printf("got kernel\n");

	// Create OpenCL buffers
    cl_mem bufA;
    cl_mem bufx;
    cl_mem bufy;

    bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, n*n*sizeof(float), NULL, &status);
    bufx = clCreateBuffer(context, CL_MEM_READ_ONLY, n*sizeof(float), NULL, &status);
    bufy = clCreateBuffer(context, CL_MEM_WRITE_ONLY, n*sizeof(float), NULL, &status);

    // Associate the input and output buffers with the kernel 
    status = clSetKernelArg(kernel, 0, sizeof(int), &n);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufA);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufx);
    status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &bufy);

    status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0, n*n*sizeof(float), A, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, bufx, CL_FALSE, 0, n*sizeof(float), x, 0, NULL, NULL);

	size_t work_size[1] = {0};
	size_t group_size[1] = {256};

	// !!!
	work_size[0] = 256*256;

    gettimeofday(&t1, NULL);

	// Execute the kernel for execution
    status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, work_size, group_size, 0, NULL, NULL);

	if (status != CL_SUCCESS) {
		printf("Error in clEnqueue, code %d\n", status);
		return -1;
	}

    // Read the device output buffer to the host output array
    clEnqueueReadBuffer(cmdQueue, bufy, CL_TRUE, 0, n*sizeof(float), y_cl, 0, NULL, NULL);

    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);

	double diff = 0.0;

	for (i=0; i<n; i++) {
		diff += abs(y[i]-y_cl[i]); 	
	}

	printf("Diff = %f\n", diff);
    printf("Execute time: %.5f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	free(A);
	free(x);
	free(y);

	return 0;
}
