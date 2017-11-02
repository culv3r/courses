#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        perror("Usage: customer <atm1|atm2>\n");
        exit(-1);
    }

    char input[256];

    int fd1 = open(argv[1], O_WRONLY);

    if (fd1 == -1){
        perror("Open Error");
    }

    while (1) {

        printf("Please send your transaction:\n");
        fgets(input, 256, stdin);
        if (input[strlen(input)-1]=='\n') {
            input[strlen(input)-1]='\0';
        }

        if (write(fd1, input, strlen(input))==-1) {
            perror("write error");
        }
    }
}
