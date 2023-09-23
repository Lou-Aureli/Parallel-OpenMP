CFLAGS	= -g -Wall
CC	= gcc $(CFLAGS)

all:
	gcc -fopenmp -o myocean myocean.c

clean:
	rm -rf myocean