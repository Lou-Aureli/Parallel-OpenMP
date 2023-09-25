CFLAGS	= -g -Wall
CC	= g++ $(CFLAGS)

all:
	g++ -fopenmp -o myocean myocean.cpp

clean:
	rm -rf myocean