CC=gcc
DEPS = prog.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prog: prog.o 
	gcc -o prog prog.o

clean:
	rm prog.o prog
