CC = gcc
DEPS = assign3part2.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: assign3part1 assign3part2
 
assign3part1: assign3part1.c
	$(CC) -o assign3part1 assign3part1.c
 
assign3part2: assign3part2.c pagetable.c phypages.c
	$(CC) -o assign3part2 assign3part2.c pagetable.c phypages.c
 
clean:
	rm -f assign3part1 assign3part2 *.o

