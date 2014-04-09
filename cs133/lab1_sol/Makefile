CC=gcc

EXEDIR	= .

DWT3_EXEC = dwt3d

LINK = -O3 -lm -fopenmp -lnuma

all: $(EXEDIR)/$(DWT3_EXEC) 

$(EXEDIR)/$(DWT3_EXEC): cdf97.* dwt3d_main.* 
	$(CC) -o $(EXEDIR)/$(DWT3_EXEC) cdf97.c dwt3d_main.c $(LINK)

clean:
	rm -rf *.o
	rm -rf $(EXEDIR)/$(DWT3_EXEC)
