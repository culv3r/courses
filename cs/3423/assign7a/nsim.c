#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
    int i;
    while (i = 0; i < 8; i++){
        fork();
    }
    return 0;
}

