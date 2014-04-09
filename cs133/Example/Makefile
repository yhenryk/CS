
LIBRARY=-L${OPENCL_ROOT}/lib64 -lOpenCL
COMPILE=-I${OPENCL_ROOT}/include -O3 -DUSE_BIN

all: vmul_gpu

vmul_gpu: vmul_ocl.c ocl_util.* 
	gcc -o vmul_gpu vmul_ocl.c ocl_util.c ${LIBRARY} ${COMPILE} 

clean:
	rm -rf vmul_gpu
