#include <stdlib.h>
#include <stdio.h>

struct PCB {
    int ProcId;
    int ProcPR;
    int CPUburst;
    int Reg[8];
    int queueEnterClock, waitingTime;
    /* other info */
    struct PCB *next;
};

int main (int argc, char *argv[]){

    int CPUreg[8] = {0};

    struct PCB *Head = NULL;
    struct PCB *Tail = NULL;
    int CLOCK=0;
    int Total_waiting_time=0;
    int Total_turnaround_time=0;
    int Total_job=0;

    if (argc < 5){
        perror("Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
        exit(-1);
    }

    return 1;
}

