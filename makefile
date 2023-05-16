# Makefile for sort -- at least during development.
#
DEPENDENCIES = options.o io.o sort.o

all: sort ${DEPENDENCIES}

sort.o: io.o

io.o: options.o


