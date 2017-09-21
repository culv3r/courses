#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"

int main(int argc, char * argv[]){
    char * infile;
    char * outfile;
    FILE * fIn;
    FILE * fOut;
    char * szOut;
    
    if (argc != 3){
        printf("Usage: driver2 <file1> <file2>\n");
    } else {
        infile = argv[1];
        outfile = argv[2];
    }

    fIn = fopen(infile, "r");
    fOut = fopen(outfile, "w");

    szOut = ReadLineFile(fIn);
    fputs(szOut,fOut);
    fclose(fIn);
    fclose(fOut);
    free(szOut);


}
