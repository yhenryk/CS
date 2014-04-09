
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <CL/cl.h>

#define A(i, j) A[(i)*n+(j)]

int main(int argc, char** argv) {

    struct	timeval t1, t2, tr;
    double	elapsedTime = 0;
	int		n = 64;
	int		i, j;

	if (argc>1)
		n = atoi(argv[1]);

	float *A = (float*)malloc(n*n*sizeof(float));
	float *x = (float*)malloc(n*sizeof(float));
	float *y = (float*)malloc(n*sizeof(float));

	srand(time(NULL));

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			A(i, j) = (float)rand()/RAND_MAX;
		}
		x[i] = (float)rand()/RAND_MAX;
	}

    gettimeofday(&t1, NULL);

	for (i=0; i<n; i++) {
		y[i] = 0;
		for (j=0; j<n; j++) {
			y[i] += A(i, j)*x[j];
		}
	}
    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);

    printf("Execute time: %.2f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	free(A);
	free(x);
	free(y);

	return 0;
}
