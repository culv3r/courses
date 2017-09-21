all: assign1-0 assign1

assign1-0: assign1-0.c pslibrary.c
	cc -o assign1-0 assign1-0.c pslibrary.c

assign1: assign1.c pslibrary.c
	cc -o assign1 assign1.c pslibrary.c

clean:
	rm -f assign1-0 assign1 *.o

run0:
	./assign1-0 0 1 2 3 4 5 6
runa:
	./assign1 0 2 5 9 4 8 7
runb:
	./assign1 0 4 11 5 6 3 7
runc:
	./assign1 0 4 9 5 6 3 7
run1:
	./assign1 3 8 7 3 6 3 2
run2:
	./assign1 3 8 7 3 6 7 2
run3:
	./assign1 4 8 7 3 6 1 2
run4:
	./assign1 3 3 3 4 2 1 2
run5:
	./assign1 3 3 2 3 2 1 2
run6:
	./assign1 3 5 2 5 4 1 1
