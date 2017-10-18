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
        //printf("Quantum: %d ms\n", quantum);
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
                //printf("Doing FIFO Algorithm.\n");
                alg = 0;
                strcpy(szAlg, "FIFO\n");
                break;
            }
        case 'S':
            if (argv[2][1] == 'J' && argv[2][2] == 'F'){
                //printf("Doing SJF Algorithm.\n");
                alg = 1;
                strcpy(szAlg,"SJF\n");
                break;
                }
        case 'P':
            if (argv[2][1] == 'R'){
                //printf("Doing PR Algorithm.\n");
                alg = 2;
                strcpy(szAlg,"PR\n");
                break;
                }
        case 'R':
            if (argv[2][1] == 'R'){
                //printf("Doing RR Algorithm.\n");
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
        SJF_Scheduling();
    } else if (alg == 2){
        PR_Scheduling();
    } else {
        RR_Scheduling(&quantum);
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
        printf("Process %d is completed at %d ms.\n", working->procId, clock);
        tmp = working;
        working = working->next;
        free(tmp);
    }
     printf("\nAverage Waiting time = %.2f ms\t(%d/%d)\n", ((double) total_waiting_time/(double) total_job), total_waiting_time, total_job);
     printf("Average Turnaround time = %.2f ms\t(%d/%d)\n", ((double)total_turnaround_time/(double)total_job), total_turnaround_time, total_job);
     printf("Throughput = %.2f jobs per ms\t(%d/%d)\n", ((double)total_job/(double)clock), total_job, clock);

    
}

void SJF_Scheduling(){
    int minCPU = -1;
    struct PCB *shortest, *previous, *current, *prevToShort;
    
    while(head != NULL){
            
        current = head;

        while(current != NULL){
            if (minCPU == -1){
                minCPU = current->cpuBurst;
                shortest = current;
            }

            if (current->cpuBurst < minCPU){
                minCPU = current->cpuBurst;
                shortest = current;
                prevToShort = previous;
            } else if (current->cpuBurst == minCPU){
                //If the id of the process is lower, give preference.
                if (shortest->procId > current->procId){
                    shortest = current;
                    prevToShort=previous;
                }
            }
        
            previous = current;
            current = current->next;
        }

        for (int i = 0; i<8; i++){
             cpuReg[i] = shortest->reg[i];
             cpuReg[i] += 1;
             shortest->reg[i] = cpuReg[i];
         }
         //Update Values
         shortest->waitingTime = shortest->waitingTime + clock - shortest->queueEnterClock;
         total_waiting_time += shortest->waitingTime;
         clock = clock + shortest->cpuBurst;
         total_turnaround_time += clock;
         total_job += 1;
         printf("Process %d is completed at %d ms.\n", shortest->procId, clock);

         if (head == shortest){
             head = head->next;
         } else if (tail == shortest) {
             tail = prevToShort;
         } else {
             prevToShort->next = shortest->next;
         }
         free(shortest);
         minCPU = -1;
    }
     printf("\nAverage Waiting time = %.2f ms\t(%d/%d)\n", ((double) total_waiting_time/(double) total_job), total_waiting_time, total_job);
     printf("Average Turnaround time = %.2f ms\t(%d/%d)\n", ((double)total_turnaround_time/(double)total_job), total_turnaround_time, total_job);
     printf("Throughput = %.2f jobs per ms\t(%d/%d)\n", ((double)total_job/(double)clock), total_job, clock);



}

void PR_Scheduling(){
     int max = -1;
     struct PCB *maxPR, *previous, *current, *prevToMax;
 
     while(head != NULL){
 
         current = head;
 
         while(current != NULL){
             if (max == -1){
                 max = current->procPR;
                 maxPR = current;
             }
 
             if (current->procPR > max){
                 max = current->procPR;
                 maxPR= current;
                 prevToMax = previous;
             } else if (current->procPR == max){
                 //If the id of the process is lower, give preference.
                 if (maxPR->procId > current->procId){
                     maxPR = current;
                     prevToMax=previous;
                 }
             }
 
             previous = current;
             current = current->next;
         }
 
         for (int i = 0; i<8; i++){
              cpuReg[i] = maxPR->reg[i];
              cpuReg[i] += 1;
              maxPR->reg[i] = cpuReg[i];
          }
          //Update Values
          maxPR->waitingTime = maxPR->waitingTime + clock - maxPR->queueEnterClock;
          total_waiting_time += maxPR->waitingTime;
          clock = clock + maxPR->cpuBurst;
          total_turnaround_time += clock;
          total_job += 1;
          printf("Process %d is completed at %d ms.\n", maxPR->procId, clock);

          if (head == maxPR){
              head = head->next;
          } else if (tail == maxPR) {
              tail = prevToMax;
          } else {
              prevToMax->next = maxPR->next;
          }
          free(maxPR);
          max = -1;
     }
      printf("\nAverage Waiting time = %.2f ms\t(%d/%d)\n", ((double) total_waiting_time/(double) total_job), total_waiting_time, total_job);
      printf("Average Turnaround time = %.2f ms\t(%d/%d)\n", ((double)total_turnaround_time/(double)total_job), total_turnaround_time, total_job);
      printf("Throughput = %.2f jobs per ms\t(%d/%d)\n", ((double)total_job/(double)clock), total_job, clock);
}

void RR_Scheduling(int *q){
    struct PCB *current, *previous, *tmp;
    int quantum = *q;
    current = head;
    while(head != NULL){
        //Do Context Switching
        for (int i = 0; i<8; i++){
             cpuReg[i] = current->reg[i];
             cpuReg[i] += 1;
             current->reg[i] = cpuReg[i];
         }
        //Q is >= CPU Burst
        if (current->cpuBurst <= quantum){
            current->waitingTime = current->waitingTime + clock - current->queueEnterClock;
            total_waiting_time = total_waiting_time + current->waitingTime;
            clock = clock + current->cpuBurst;
            total_turnaround_time = total_turnaround_time + clock;
            total_job += 1;
            head = head->next;
            tmp = current;
            printf("Process %d is completed at %d ms.\n", current->procId, clock);
            current = current->next;
            free(tmp);
        } else {
            current->waitingTime = current->waitingTime + clock - current->queueEnterClock;
            clock = clock + quantum;
            current->cpuBurst = current->cpuBurst - quantum;
            current->queueEnterClock = clock;
            tmp = current;
            while(tmp->next != NULL){
                tmp = tmp->next;
            }
            tmp->next = current;
            head = head->next;
            tail = current;
            current->next = NULL;
            current = head;
        }

    }
    printf("\nAverage Waiting time = %.2f ms\t(%d/%d)\n", ((double) total_waiting_time/(double) total_job), total_waiting_time, total_job);
    printf("Average Turnaround time = %.2f ms\t(%d/%d)\n", ((double)total_turnaround_time/(double)total_job), total_turnaround_time, total_job);
    printf("Throughput = %.2f jobs per ms\t(%d/%d)\n", ((double)total_job/(double)clock), total_job, clock);
}

