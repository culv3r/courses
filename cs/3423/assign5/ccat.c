#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MAX_BUF_SIZE 1024


int main (int argc, char *argv[]){
    int fd1; //file descriptor
    int fd2; //file descriptor
    int wfd = -1; //file descriptor for write
    int bInput1 = 0; //Boolean input flag for whether inputs are Stdin or not.
    int bInput2 = 0;
    char szBuf[MAX_BUF_SIZE];
    char szBuf2[MAX_BUF_SIZE];
    ssize_t size;
    ssize_t size2;
    int iWrite;

    if (argc < 3 || ( argc > 4 && ( strcmp("-",argv[1]) != 0 || strcmp("-",argv[2]) != 0 ))){
        //Arguments are outside of specifications, kill the program and list usage.
        printf("Invalid Invocation! Format of program is: ccat < '-' | file1.in > < '-' | file2.in > [ file3.out ]\n");
        return -1;
    }
    if ( strcmp("-",argv[1]) == 0 && strcmp("-",argv[2]) == 0 ){
        printf("Invalid Invocation! One argument MUST be a file.\n");
        return -1;
    }
    int i = 0;
    //Iterate through arguments and set up proper I/O handling
    for (i = 1; i<=argc-1; i++){
        //If arg in question is a file, open file descriptor
        if (strcmp("-",argv[i]) != 0 && i <= 2){
            if (i == 1){
                fd1 = open(argv[i], O_RDONLY);
                printf("File Descriptor 1 in Use.\n");
            }
            if (i == 2){
                fd2 = open(argv[i], O_RDONLY);
                printf("File Descriptor 2 in Use.\n");
            }
            //Error handling
            if (fd1 == -1){
                perror(argv[i]);
                return 1;
            }
            if (fd2 == -1){
                perror(argv[i]);
                return 1;
            }
        }
        //If arg in question is switch for Stdin, create input buffer and write.
        if (strcmp("-",argv[i]) == 0){
            if (i == 1){
                bInput1 = 1;
            }
            if (i == 2){
                bInput2 = 1;
            }
        }
        if (i == 3){
            wfd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            printf("Write file opened, %s\n", argv[i]);
            if (wfd == -1){
                perror(argv[3]);
                return 1;
            }

        }
    }
    //Setup I/O transactions
    if ((fd1 != 0 && fd2 != 0)){
        while ((size = read(fd1, szBuf, sizeof szBuf)) > 0){
            if (wfd == -1){
                write(STDOUT_FILENO,szBuf,size);
            }
            if (wfd != -1){
                iWrite = write(wfd,&szBuf,size);
                if (iWrite < 0)
                    return 0;
            }
        }
        while ((size2 = read(fd2, szBuf2, sizeof szBuf2)) > 0){
            if (wfd == -1){
                write(STDOUT_FILENO,szBuf2,size2);
            }
            if (wfd != -1){
                iWrite = write(wfd,szBuf2,size2);
                if (iWrite < 0)
                    return 0;
            }
        }

    }
    //Stdin for first input, read file for second input.
    if ((bInput1 && fd2 != 0)){
        while((size = read(STDIN_FILENO, szBuf, sizeof szBuf)) > 0){
            if (wfd == -1){
                if (szBuf[0] != '\0')
                    write(STDOUT_FILENO, szBuf, size);
            }
            else{
                if (szBuf[0] != '\0')
                    iWrite = write(wfd,szBuf,size); 
                if (iWrite < 0)
                    return 0;
            }
        }
        while ((size2 = read(fd2, szBuf2, sizeof szBuf2)) > 0){
            if (wfd == -1){
                write(STDOUT_FILENO,szBuf2,size2);
            }
            else{
                iWrite = write(wfd,szBuf2,size2);
                if (iWrite < 0)
                    return 0;
            }
        }
    }
    
    //Stdin for second input...
    if ((fd1 != 0 && bInput2)){
        while((size = read(fd1, szBuf, sizeof szBuf)) > 0){
            if (wfd == -1){
                write(STDOUT_FILENO,szBuf,size);
            }
            if (wfd != -1){
                iWrite = write(wfd,szBuf,size);
                if (iWrite < 0)
                    return 0;
            }
        }
        while((size2 = read(STDIN_FILENO, szBuf2, sizeof szBuf2)) > 0){
            if (wfd == -1){
                write(STDOUT_FILENO,szBuf2,size2);
            }
            if (wfd != -1){
                iWrite = write(wfd,szBuf2,size2);
                if (iWrite < 0)
                    return 0;
            }
        }
    }
    close(fd1);
    close(fd2);
    close(wfd);
    return 1;
}
