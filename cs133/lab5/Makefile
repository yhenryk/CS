TARGET=smith_waterman.exec
CFLAGS=-g -I$(CNCC_HOME)/include -I$(HC_HOME)/include/hc

compile: pre first second third


#"invoking translator"
pre:
	#cncc_t smith_waterman.cnc

# building auto-generated files
first:
	gcc $(CFLAGS) -c Context.c
	hcc $(CFLAGS) -c Dispatch.hc

# building steps and main
second:
	hcc $(CFLAGS) -c Main.hc Common.hc

# linking - creating the executable
third:
	gcc $(CFLAGS) -L"$(HC_HOME)/lib" \
	       $(CNCC_HOME)/lib/DataDriven.o \
	       Common.o \
	       Dispatch.o Context.o Main.o -o $(TARGET) -lhc -lxml2 -lpthread


clean:
	rm rose_*  *.o $(TARGET) 
	#rm Context.c Context.h Dispatch.h Dispatch.hc
