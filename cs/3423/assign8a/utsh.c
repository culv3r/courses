#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct cmd
   {
    int redirect_in;     /* Any stdin redirection?         */
    int redirect_out;    /* Any stdout redirection?        */
    int redirect_append; /* Append stdout redirection?     */
    int background;      /* Put process in background?     */
    int piping;          /* Pipe prog1 into prog2?         */
    char *infile;        /* Name of stdin redirect file    */
    char *outfile;       /* Name of stdout redirect file   */
    char *argv1[10];     /* First program to execute       */
    char *argv2[10];     /* Second program in pipe         */
   };

int cmdscan(char *cmdbuf, struct cmd *com);

int main(void){
    char buf[1024];
    struct cmd command;
    int i, bExit, fd[2], std_in, std_out, fdin, fdout;
    pid_t pid;
    printf("\n>> ");
    while(fgets(buf, sizeof buf, stdin) != NULL || bExit != 1){
        if (cmdscan(buf, &command)){
            printf("Illegal Format: \n>> ");
            continue;
        }
        if (strcmp(command.argv1[0],"exit") == 0){
            bExit = 1;
            break;
        }
        
        std_in = dup(0);
        std_out = dup(1);

        if (command.redirect_in == 1){
            fdin = open(command.infile, O_RDONLY);
        }

        if (command.redirect_out == 1){
            if (command.redirect_append == 1){
                fdout = open(command.outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
            }else{
                fdout = open(command.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            }
        }
        
        if (command.piping == 1){
            pipe(fd);
        }
        switch (fork()){
            case -1:
                perror("fork error");
                exit(-1);
            case 0:     //child
                if (command.piping == 1){
                    switch(pid = fork()){
                        case -1:
                            perror("fork error");
                            exit(-1);
                        case 0:
                            dup2(fd[1], STDOUT_FILENO);
                            close(fd[1]);
                            execvp(command.argv1[0], command.argv1);
                            perror("exec error");
                            exit(-1);
                            break;
                    }
                    switch(pid = fork()){
                        case -1:
                            perror("fork error");
                            exit(-1);
                        case 0:
                            dup2(fd[0],STDIN_FILENO);
                            close(fd[0]);
                            execvp(command.argv2[0], command.argv2);
                            perror("exec error");
                            exit(-1);
                    }
                }
                if (command.piping != 1){
                execvp(command.argv1[0], command.argv1);
                perror("exec error");
                exit(-1);  
                }
            default:
                if (command.background != 1){
                    wait(NULL);
                }
        }
        printf("\n>> ");
       // fgets(buf, sizeof buf, stdin);
    }
    exit(0);
}
                    
