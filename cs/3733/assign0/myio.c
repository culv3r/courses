#include "myio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int ReadInteger(void){
    int bInt = 0;
    int iChar;
    int iNum;
    int result;

    printf("Please enter an integer: ");
    result = scanf("%d",&iNum);
    while(!bInt){
        if(result == 0){
            printf("Integer not found, Please enter an integer: ");
            while(getchar() != '\n');
            result = scanf("%d", &iNum);
        } else {
            if (getchar() != '\n'){
                printf("Malformed Input, Please provide an integer: ");
                while(getchar() != '\n');
                result = scanf("%d",&iNum);
            } else {
            bInt = 1;
            }
        }
    }
    fflush(stdin);
    return iNum;
}

double ReadDouble(void){
    int bDouble = 0;
    double dNum;
    int result;

    printf("Please enter a double: ");
    result = scanf("%lf",&dNum);
    while(!bDouble){
        if(result == 0){
            printf("Double not found, please enter a double: ");
            while(getchar() != '\n');
            result = scanf("%lf",&dNum);
        } else {
            if(getchar() != '\n'){
                printf("Malformed input, please provide a double: ");
                while(getchar() != '\n');
                result = scanf("%lf",&dNum);
            } else  {
            bDouble = 1;
            }
        }
    }
    fflush(stdin);
    return dNum;
}

char *ReadLine(void){
    size_t len;
    ssize_t nread;
    char *szLine = NULL;
    char *szStr;


    printf("Please enter your string: ");
    szStr = (char *) malloc (1);
    nread = getline(&szLine,&len,stdin);
    
    //Searches for newline and replaces with null terminator.
    char *pch = strstr(szLine, "\n");
    if(pch != NULL)
    strncpy(pch, "\0", 1);
    szStr = ( char * ) malloc (strlen(szLine));
    strcpy(szStr,szLine);
    free(szLine);

    return szStr;
}

char *ReadLineFile(FILE *infile){
    int i;
    int j;
    int length;
    int pos;
    size_t len;
    ssize_t nread;
    char *szLine = NULL;
    char *szStr;
    
    if (infile == NULL)
        exit(EXIT_FAILURE);
    
    szStr = ( char * ) malloc (len + 1);
    while ((nread = getline(&szLine,&len,infile)) != -1){
        
        i=0;
        j=0;

        length = len;
        szStr = ( char * ) realloc (szStr,(pos + length + 1));
        
        while(*(szLine+i) != '\0') {
            if(*(szLine+i) == ' ') {
                int tmp = i + 1;
                if (*(szLine+tmp) != '\0') {
                    while(*(szLine + tmp) == ' ' && *(szLine+tmp) != '\0'){
                        if (*(szLine+tmp) == ' ') {
                            i++;
                        }
                        tmp++;
                    }
                }
            }
            *(szStr + j + pos) = *(szLine+i);
            i++;
            j++;
        }
        pos += j;
    }
    *(szStr + j + pos) = '\0';
    free(szLine);
 
    return szStr;
}
