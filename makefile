# Makefile for sort -- at least during development.

# CFLAGS = -g

DEPENDENCIES = dequeue.o options.o io.o sort.o



all: sort

sort: $(DEPENDENCIES) 
	cc -o sort sort.o options.o dequeue.o io.o 

sort.o: sort.h io.o options.h dequeue.h

options.o: sort.h options.h dequeue.o dequeue.h

dequeue.o: dequeue.h
