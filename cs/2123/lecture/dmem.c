#include <stdio.h>
#include <stdlib.h>
#define NUM_INTS 10
int main(int argc, char *argv[]) {
int * numbers;
numbers = (int *) malloc(NUM_INTS *sizeof(*numbers));
if(numbers == NULL) {
perror("numbers Alloc");
exit(EXIT_FAILURE);
}
exit(EXIT_SUCCESS);
}
