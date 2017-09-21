CC=gcc
CFLAGS=-I.
DEPS = myio.h
OBJ = driver1.o myio.o 
OBJ2 = driver2.o myio.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: driver1 driver2

driver1: $(OBJ)
	gcc -g -o $@ $^ $(CFLAGS)

driver2: $(OBJ2)
	gcc -g -o $@ $^ $(CFLAGS)
