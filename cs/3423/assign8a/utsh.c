#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
    int i;
    int bExit = 0;
    printf("\n>> ");
    while( (fgets(buf,1024,stdin) != NULL) ){
        while(!bExit){
            if (cmdscan(buf,&command)){
                    printf("Illegal Format: \n");
                    continue;
            }
            if (strcmp(command.argv1[0], "exit") == 0){
                    bExit = 1;
                    break;
            }

            if (command.piping){
                int fd[2];
            }

            if (command.redirect_in){
            }

            if (command.redirect_out){
                if (command.redirect_append){
                }
            }
            
            
            //Program 1 Fork

            switch (fork()){
                case -1:
                    perror("fork error");
                    exit(-1);
                case 0:     //child
                    if (command.background){
                        setpgid(0,0);
                        printf("%d\n",getpid());
                    }
                    execvp(command.argv1[0], command.argv1);
                    perror("exec error");
                    exit(-1);
                default:
                    if(!command.background){
                        wait(NULL);
                    }
            }
            printf("\n>> ");
            fgets(buf, 1024, stdin);
        }
    exit(0);
    }
}
                    
