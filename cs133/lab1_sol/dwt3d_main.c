#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>
#include <numa.h>
#include <sched.h>
#include <numaif.h>

#include "cdf97.h"

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
	
#pragma omp parallel for schedule(static, 4)
	for (i = 0; i < p; i++)
	{
		FILE* fin = fopen(fname, "rb");
		fseek (fin, 12 + sizeof(float) * n * m * i, SEEK_SET);
		fread (img + i * n * m, n * m, sizeof(float), fin);
		fclose(fin);
	}
	gettimeofday(&t1, NULL);

	// 3D DWT transform, which is 2D DWT for each image slices
#pragma omp parallel for schedule(static, 4)
	for (i=0; i<p; i++)
	{
		cdf97(img+i*n*m, tmp+i*n*m, m, n, level);
	}

	gettimeofday(&t2, NULL);
	timersub(&t1, &t2, &tr);
	printf("Execute time: %.3f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	free(img);
	free(tmp);

	return 0;
}
