

CC = gcc
CFLAGS = -O3


all: gnuplot_pipes.o anim example
clean:
	rm -f gnuplot_pipes.o anim example

gnuplot_pipes.o: gnuplot_pipes.c gnuplot_pipes.h
	$(CC) $(CFLAGS) -c -o gnuplot_pipes.o gnuplot_pipes.c -I.

anim: anim.c gnuplot_pipes.o
	$(CC) $(CFLAGS) -o anim anim.c gnuplot_pipes.o

example: example.c gnuplot_pipes.o
	$(CC) $(CFLAGS) -o example example.c gnuplot_pipes.o
