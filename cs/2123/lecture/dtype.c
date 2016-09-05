#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]){

	printf("Storage size for int    : %lu \n", sizeof(int));
	printf("Storage size for char   : %lu \n", sizeof(char));
	printf("Storage size for float  : %lu \n", sizeof(float));
	printf("Storage size for double : %lu \n", sizeof(double));

	return 0;
}
