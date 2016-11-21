struct message{
    int rand;
    pid_t pid;
    int index;
}msg;

int main(void){

int fd[33][2];
for (i = 0; i<33; i++){
    pipe(fd[i]);
}
for (index = 0; index < 32; index++){
    pid = fork();
    if (pid == -1){
        perror();
        exit(1);
    }
    if (pid != 0)
        break;
}

if (index == 0){
    close(fd[0][1]);
    for (i = 1; i<33; i++){
        close(fd[i][0]);
        }
    for (i = 1; i<33; i++){
        msg.rand = rand();
        write(fd[i][1], &message.rand, sizeof(int));
        close(fd[i][1]);
    }
}

while(read(fd[0][0], &msg, sizeof(message)) == sizeof(message)){
    printf("Original Process 0 read random number from %d from process %d index %d", msg.rand, msg.pid, msg.index);
}
close(fd[0][0]);

else{
    for(i=1; i<33; i++){
        close(fd[i][1]);
    }
    for (i=0; i<33; i++){
        if (i != index){
            close(fd[i][0]);
        }
    }

    read(fd[index][0],&msg.rand, sizeof(int));
    if(msg.rand%2 == 0){
        msg.pid = getpid();
        msg.index = index;
        write(fd[0][1], &msg, sizeof(message));
    }
    close(fd[index]);
    close(fd[0][1]);
}
}
