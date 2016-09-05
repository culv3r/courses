#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	// Check number of command line arguments
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <a string>\n", argv[0]);
		exit(-1);
	}
	
	printf("Hello %s\n", argv[1]);

	exit(0);
}
