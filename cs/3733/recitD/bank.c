#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main (int argc, char *argv[]){
    int total = 0;
    int fd1,fd2,n,result;
    fd_set readfds;
    char buf[256];
    struct timeval tv;

    char * atm1 = "atm1";
    char * atm2 = "atm2";

    mkfifo(atm1, 0666);
    mkfifo(atm2, 0666);
    fd1 = open(atm1,O_RDONLY|O_NONBLOCK);
    fd2 = open(atm2,O_RDONLY|O_NONBLOCK);

    if (fd1 == -1 || fd2 == -1){
        perror("FIFO Pipe Error\n");
        exit(-1);
    }

        
    
    while(1){
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);   

        n = fd1 > fd2 ? fd1 : fd2;

        select(n+1, &readfds, NULL, NULL, NULL);
        
        // one or both of the descriptors have data
        if (FD_ISSET(fd1, &readfds)) {
            read(fd1, buf, sizeof(buf));total += atoi(buf);
        }
        if (FD_ISSET(fd2, &readfds)) {
            read(fd2, buf, sizeof(buf));total += atoi(buf);
        }
         

        printf("Balance is: %d\n", total);
    }
    
    return 1;
}
