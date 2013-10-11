# Makefile for PPS 

CC=cc
CFLAGS=-O2

all: PPS COMM DERIV


PPS: src/PPS.c include/PPS_parameters.h src/amdahl.c
	$(CC) $(CFLAGS) -o bin/PPS src/PPS.c src/amdahl.c  -lm -I./include

COMM: src/commperf.c 
	$(CC) -o bin/commperf src/commperf.c -lm

DERIV: src/derivative.c 
	$(CC) -o bin/derivative src/derivative.c -lm

clean: 
	rm bin/*
