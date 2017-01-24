#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    char *cmd = "/usr/bin/ls *.c";
    char buf[BUFSIZ];
    FILE *fp;

    if ((fp = popen(cmd,"r")) != NULL){
        while (fgets(buf,BUFSIZ,fp) != null){
            printf ("%s", buf);
        }
    }
    exit(0);
}
