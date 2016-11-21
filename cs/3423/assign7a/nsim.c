#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#define PROCESS_IDS 8


int main (int argc, char *argv[]){
    int i,j,n,ndx,num,rng;
    pid_t child;
    int fd[9][2];
    char buf[9], recBuf[9];
    size_t nbytes;

    //Create Pipes
    for (i=0; i<=PROCESS_IDS; i++){
        pipe(fd[i]);
    }

    //Fork Processes
    for (i = 1; i<=PROCESS_IDS; i++){
        if (child = fork()){
            printf("I am the parent process.\n");         
            ndx = 0;
            srand(1313317*0);
            break;
        }

        if (child == -1){
            perror("Fork error");
            exit(-1);
        }
        if (child == 0){
            ndx = i;
            printf("My index is %d\n", ndx);
            srand(1313317*i);
        }
    }

    //Close Proper Communication Channels
    //0 = Input (Write)
    //1 = Output (Read)
    if (child == 0){
        for (j = 0;j<=PROCESS_IDS;j++){
            if (j == ndx){
                close(fd[j][0]);
            }
            if (j != ndx){
                close(fd[j][1]);
            }
        }
    }else{
        for (j = 0; j<=PROCESS_IDS;j++){
            if (j == ndx){
                close(fd[j][0]);
            }
            if (j != ndx){
                close(fd[j][1]);
            }
        }
    }

    //Random Number Generation
    rng = RAND_MAX/9;
    rng *= 9;
    for (n=0; n < 13; n++){
        do {
            num = rand();
            j = num%9;
        }
        while (num >= rng || ndx == j);
        sprintf(buf,"process%d",ndx);
        nbytes = write(fd[j][1], buf, sizeof buf);
        if (nbytes != sizeof buf){
            perror("Write error");
            exit(-1);
        }

    }

    //Close Pipes
    for (j = 0;j<=PROCESS_IDS;j++){
        if (j != ndx){
            close(fd[j][0]);
        }
    }

    //Read Messages
   while ((nbytes = read(fd[ndx][0],buf,sizeof buf)) > 0){
            printf("process%d has received a message from %s\n",ndx,buf);
    }

        
}

