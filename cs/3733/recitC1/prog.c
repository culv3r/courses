#include "prog.h"

/* GLOBAL VARS */

int cpuReg[8] = {0};
struct PCB *head = NULL;
struct PCB *tail = NULL;
int clock=0;
int total_waiting_time=0;
int total_turnaround_time=0;
int total_job=0;


int main (int argc, char *argv[]){

    char buf[4096];
    
    int iId;
    int iPr;
    int iBurst;
    int quantum;
    int alg;
    int bHasQuantum = 0;
    char szAlg[7];
    char *filename;

    int cpuReg[8] = {0};

    struct PCB *head = NULL;
    struct PCB *tail = NULL;
    int clock=0;
    int total_waiting_time=0;
    int total_turnaround_time=0;
    int total_job=0;

    if (argc < 5){
        perror("Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
        exit(-1);
    }

    if (strcmp(argv[1],"-alg") != 0){
        perror("Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
        exit(-1);
    }

    if (strcmp(argv[3], "-quantum") == 0){
        bHasQuantum = 1;
        quantum = atoi(argv[4]);
        if (strcmp(argv[5], "-input") == 0){
            filename = (char *) malloc (strlen(argv[6])+1);
            strcpy(filename,argv[6]);
        }
    } else if (strcmp(argv[3], "-input") == 0){
        filename = (char *) malloc (strlen(argv[6])+1);
        strcpy(filename,argv[4]);
    }


    switch(argv[2][0]){
        case 'F':
            if (argv[2][1] == 'I' && argv[2][2] == 'F' && argv[2][3] == 'O'){
                printf("Doing FIFO Algorithm.\n");
                alg = 0;
                strcpy(szAlg, "FIFO\n");
                break;
            }
        case 'S':
            if (argv[2][1] == 'J' && argv[2][2] == 'F'){
                printf("Doing SJF Algorithm.\n");
                alg = 1;
                strcpy(szAlg,"SJF\n");
                break;
                }
        case 'P':
            if (argv[2][1] == 'R'){
                printf("Doing PR Algorithm.\n");
                alg = 2;
                strcpy(szAlg,"PR\n");
                break;
                }
        case 'R':
            if (argv[2][1] == 'R'){
                printf("Doing RR Algorithm.\n");
                alg = 3;
                strcpy(szAlg,"RR\n");
                break;
                }
        default:
            perror("Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [input_file_name.txt]\n");
            exit(-1);
    }


    
    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
        perror("Error opening file.\n");
        exit(-1);
    }

    while(fscanf(fp, "%d %d %d\n", &iId, &iPr, &iBurst) != EOF){

        if (head == NULL){
            head = initHead(iId, iPr, iBurst);
            tail = initTail(tail, head);
        } else {
            tail = push(iId, iPr, iBurst);
        }
        
    }

    fclose(fp);

    printf("Student Name: Daniel Larsen\n");
    printf("Input File Name: %s\n", filename);
    printf("CPU Scheduling Alg: %s\n", szAlg);
    if (alg == 0){
        FIFO_Scheduling();
    } else if (alg == 1){
        //DO SJF
    } else if (alg == 2){
        //DO PR
    } else {
        //DO RR
    }
    

    free(filename);
    

    return 1;
}

struct PCB *initHead(int iId, int iPr, int iBurst){
    head = malloc(sizeof(struct PCB));
    if (head == NULL){
        perror("Error allocating memory.\n");
        exit(-1);
    }
    head->procId = iId;
    head->procPR = iPr;
    head->cpuBurst = iBurst;
    for (int i = 0; i<8; i++){
        head->reg[i] = iId;
    }
    head->queueEnterClock = 0;
    head->waitingTime = 0;
    head->next = NULL;
    return head;
}

struct PCB *initTail(struct PCB *tail, struct PCB *head){
    tail = head;
    return tail;
}

struct PCB *push(int iId, int iPr, int iBurst){
    struct PCB * current = head;
    
    while (current->next != NULL){
        current = current->next;
    }

    current->next = malloc(sizeof(struct PCB));
    current->next->procId = iId;
    current->next->procPR = iPr;
    current->next->cpuBurst = iBurst;
    for (int i = 0; i<8; i++){
        current->next->reg[i] = iId;
    }
    current->next->queueEnterClock = 0;
    current->next->waitingTime = 0;
    current->next->next = NULL;

    return current->next;
}

void FIFO_Scheduling(){
    struct PCB *working, *tmp;

    working = head;
    while(working != NULL){
        for (int i = 0; i<8; i++){
            cpuReg[i] = working->reg[i];
            cpuReg[i] += 1;
            working->reg[i] = cpuReg[i];
        }
        //Update Values
        working->waitingTime = working->waitingTime + clock - working->queueEnterClock;
        total_waiting_time += working->waitingTime;
        clock = clock + working->cpuBurst;
        total_turnaround_time += clock;
        total_job += 1;
        printf("Process %d is completed at %d ms.\n", total_job, clock);
        tmp = working;
        working = working->next;
        free(tmp);
    }
    printf("\nAverage Waiting time = %.2f ms\t(%d/%d)\n", ((double) total_waiting_time/(double) total_job), total_waiting_time, total_job);
    printf("Average Turnaround time = %.2f ms\t(%d/%d)\n", ((double)total_turnaround_time/(double)total_job), total_turnaround_time, total_job);
    printf("Throughput = %.2f jobs per ms\t(%d/%d)\n", ((double)total_job/(double)clock), total_job, clock);

        
}

