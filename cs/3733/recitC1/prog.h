#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PCB {
    int procId;
    int procPR;
    int cpuBurst;
    int reg[8];
    int queueEnterClock, waitingTime;
    /* other info */
    struct PCB *next;
};

/* Prototypes */
struct PCB *push(int iId, int iPr, int iBurst);
struct PCB *initHead(int iId, int iPr, int iBurst);
struct PCB *initTail(struct PCB *tail, struct PCB *head);
void FIFO_Scheduling();
