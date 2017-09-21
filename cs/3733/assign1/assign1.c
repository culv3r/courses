#include <stdlib.h>
#include <stdio.h>
#include "pslibrary.h"

int main (int argc, char *argv[]){
    char *heading;
    char *s1;
    char *s2;
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    int z1 = 0;
    int z2 = 0;
    int size = 0;

    if (argc != 8){
        perror("Usage: assign1-0 int int int int int int int\n");
        exit(-1);
    }

    printf("Assignment 1 program was written by Daniel Larsen.\n");
    printf("inputs: ");
    for (int i = 1; i<argc; i++){
        if (i == argc - 1){
            printf("%d\n", atoi(argv[i]));
        }
        else{
            printf("%d ", atoi(argv[i]));
        }
        if (i > 2)
            size += atoi(argv[i]);
    }

    s1 = (char *) malloc (sizeof(char)*size);
    s2 = (char *) malloc (sizeof(char)*size);
    if (s1 == NULL || s2 == NULL){
        perror("Memory Allocation Error\n");
        exit(-1);
    }

    x1 = atoi(argv[2]);
    y1 = atoi(argv[3]);
    z1 = atoi(argv[4]);
    x2 = atoi(argv[5]);
    y2 = atoi(argv[6]);
    z2 = atoi(argv[7]);

    fcfsa(s1, s2, x1, y1, z1, x2, y2, z2); 
    
    display(heading, s1, s2);
    

    return(0);
}
