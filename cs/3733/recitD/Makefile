CC=gcc
DEPS = 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: bank customer

bank: bank.o 
	gcc -o bank bank.o

customer: customer.o
	gcc -o customer customer.o

clean:
	rm bank bank.o customer customer.o
