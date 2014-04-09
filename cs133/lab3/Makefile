LIBRARY= -L${OPENCL_ROOT}/lib64 -lOpenCL
COMPILE= -I${OPENCL_ROOT}/include -O3 -DUSE_BIN

all: dwt3d_ocl

dwt3d_ocl: ./src/cdf97.c ./src/cdf97.h ./src/dwt3d_main.c ./src/util.c ./src/util.h 
	gcc -o ./bin/dwt3d_ocl ./src/cdf97.c ./src/cdf97.h ./src/dwt3d_main.c ./src/util.h ./src/util.c ${LIBRARY} ${COMPILE}

clean:
	rm -rf ./bin/dwt3d_ocl
