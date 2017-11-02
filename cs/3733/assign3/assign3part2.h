#define PAGE_BYTES 128
#define VMEM_SIZE 4096
#define PHY_SIZE 1024
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int frame;
    int valid;
} pageEntry;

typedef struct {
    int page;
    unsigned long address;
} phyPage;

struct Stack {
    int page;
    struct Stack *next;
    struct Stack *prev;
};

struct Stack* newNode(int page);

/* Page Table Prototypes */

int initTable(int pages);
int pageTable(unsigned long virtAddr, unsigned long *phyResult);

/* Physical Frame Prototypes */

int initFrame(int frames);
int initStack();
unsigned long phyTranslate(unsigned long addr, int frame);
int getCount(struct Stack* head);
unsigned long runLRU();
