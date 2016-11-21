#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char buf[1024];
    char *sargv[16];
    int i;
    printf("\n>> ");
    gets(buf);
    while(strcmp(buf,"exit")!=0){
        strtok(buf, "");
        switch (fork()){
            case -1:
                perror("fork error");
                exit(-1);
            case 0:     //child
                i=0;
                sargv[0]=strtok(buf," ");
                while (sargv[i++] != NULL){
                    sargv[i]=strtok(NULL," ");
                }
                execvp(sargv[0], sargv);
                perror("exec error");
                exit(-1);
            default:
                wait(NULL);
        }
        printf("\n>> ");
        gets(buf);
    }
    exit(0);
}
                    
