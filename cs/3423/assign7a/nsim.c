#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#define PROCESS_IDS 8


int main (int argc, char *argv[]){
    int i,j,n,ndx,num,rng,result;
    pid_t child;
    int fd[9][2];
    char buf[9], msg[9];
    ssize_t nbytes;

    //Create Pipes
    for (i=0; i<=PROCESS_IDS; i++){
        if((result = pipe(fd[i]))!=0){
            perror("pipe error");
            exit(-1);
        }
    }

    //Fork Processes
    for (i = 0; i<=PROCESS_IDS; i++){
        if (i == 0){
            continue;
        }
        if ((child = fork())<=0){ 
            //printf("My index is %d\n", i);
            ndx = i;
            srand(1313317*i);
            break;
        }
        ndx = 0;
        srand(1313317*0);
    }

    //Close Proper Communication Channels
    //0 = Input (Read)
    //1 = Output (Write)

    for (j = 0;j<=PROCESS_IDS;j++){
        if (j == ndx){
            close(fd[j][1]);
        }
        if (j != ndx){
            close(fd[j][0]);
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
        sprintf(msg, "process%d", ndx);
        nbytes = write(fd[j][1], msg, sizeof msg);
        if (nbytes < 0){
            perror("write error");
            exit(-1);
        }
    }
    
    //Close Pipes
    for (j = 0;j<=PROCESS_IDS;j++){
            close(fd[j][1]);
    }

    //Read Messages
   while ((nbytes = read(fd[ndx][0],buf,sizeof buf)) > 0){
            printf("process%d has received a message from %s\n",ndx,buf);
    }
    
}
