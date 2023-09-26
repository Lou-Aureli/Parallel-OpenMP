CFLAGS	= -g -Wall
CC	= g++ $(CFLAGS)

all:
	gcc -fopenmp -o myocean_c myocean.c
	gcc -fopenmp -o myocean_omp_c myocean_omp.c

clean:
	rm -rf myocean