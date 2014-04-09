CFLAG=-O3
BIN=.
DATA=./data

all: par

#sw: smith_waterman.c
	#gcc $(CFLAG) -o $(BIN)/smith_waterman_sequential smith_waterman.c
par: smith_waterman_mpi.c
	mpicc -o $(BIN)/smith_waterman_mpi smith_waterman_mpi.c

run: run_sw 

run_sw:
	$(BIN)/smith_waterman_sequential $(DATA)/largeX.txt $(DATA)/largeY.txt
