CFLAGS	= -g -Wall
CC	= g++ $(CFLAGS)

all:
	g++ -fopenmp -o myocean myocean.cpp
	g++ -fopenmp -o myocean_omp myocean_omp.cpp

clean:
	rm -rf myocean