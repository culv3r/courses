#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"

int main (int argc, const char * argv[]){
    int a;
    double b;
    char *c = NULL;
    int maxInt = 0;
    double maxDouble = 0;
    char *maxStr = "";
    int x;
    int y;
    int z;

    if (argc !=4){
        printf("Usage: driver1 <#Ints> <#Doubles> <#Strings>\n");
        exit(-1);
    } else {
    //printf("Entering loop sequence.\n");    
    for(int i = 1;i<argc;i++){
        //printf("Setting x to be equal to %d with argc of %d\n",atoi(argv[1]),argc);
        x = atoi(argv[i]);
        //printf("Variable successfully set, entering loops\n");
        for(int j = 0;j<x;j++){
            if(i==1){
                //printf("Reading in an integer\n");
                //Read in integers
                a = ReadInteger();
                if (a > maxInt)
                    maxInt = a;
                printf("You entered the integer: %d, largest integer so far is: %d.\n",a,maxInt);
            }
            if(i==2){
                //printf("Reading in a Double\n");
                //Read in doubles
                b = ReadDouble();
                if (b > maxDouble)
                    maxDouble = b;
                printf("You entered the double: %f, largest double so far is: %f.\n",b,maxDouble);
            }
            if (i==3){
                //printf("Reading in a String\n");
                //Read in strings
                c = ReadLine();
                //printf("C: %s\n",c);
                if (strcmp("",maxStr) == 0){
                    maxStr = (char *) malloc (sizeof(c));
                }
                if (strcmp(c,maxStr) > 0){
                    maxStr = (char *) realloc (maxStr, (sizeof(c)));
                            }
                if (strlen(c)>strlen(maxStr)){
                    strcpy(maxStr,c);
                }
                printf("You entered the String: \"%s\", largest string so far is:\"%s\".\n",c,maxStr);
                c=NULL;
            }
        }
     }
        printf("Your largest integer was: %d\nYour largest double was: %f\nYour largest string was:\"%s\".\n",maxInt,maxDouble,maxStr);
    }
} 
