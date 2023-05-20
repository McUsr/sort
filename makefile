# Makefile for sort -- at least during development.

# CFLAGS = -g

DEPENDENCIES = dequeue.o options.o ioN.o io0.o sort.o

all: sort

sort: $(DEPENDENCIES) 
	cc -o sort sort.o options.o dequeue.o ioN.o io0.o 

sort.o: sort.h ioN.h io0.h options.h dequeue.h

options.o: sort.h options.h dequeue.o dequeue.h

dequeue.o: dequeue.h
