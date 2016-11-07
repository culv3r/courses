#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#define PROCESS_IDS 8

int main (int argc, char *argv[]){
    int i,j,k;
    pid_t pids[PROCESS_IDS+1];
    int fd[PROCESS_IDS+1][2];
    int iProcess;
    for (k = 0; k<PROCESS_IDS; k++){
        pipe(fd[k]);
    }

    for (i = 0; i < PROCESS_IDS; i++){
        if ((pids[i] = fork()) < 0) {
                perror("fork");
                exit(1);
            } else if (pids[i] == 0) {
                iProcess = i;
                for (j = 0; j < PROCESS_IDS; j++){
                    if (j != iProcess){
                        close(fd[j][0]);
                    }else{
                    close(fd[j][1]);
                }
            }   

            exit(0);
        }
    }
    return 0;
}

