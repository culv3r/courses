#include "assign3part2.h"

phyPage **frame;
struct Stack *head;
struct Stack *tail;
int maxFrames;

int initFrame(int frames){
    frame = malloc(frames*sizeof(*frame));
     if (frame == NULL){
         perror("Page Table Allocation Error");
         return -1;
     } else {
         maxFrames = frames;
         for (int i = 0; i<maxFrames; i++){
             frame[i] = malloc(sizeof(**frame));
         }
         //INIT OS
         frame[0]->page = 99;
         frame[0]->address = 0xFFFFFFFF;
         return 0;
     }
     return 1;
}

unsigned long phyTranslate(unsigned long addr, int frameNum){
    //printf("Variable Initialization\n");
    int count = 0;
    unsigned long page = addr >> 7;
    unsigned long tmp = addr & 0x7F;
    unsigned long phyAddr = (unsigned long) frameNum;
    phyAddr = phyAddr << 7;
    struct Stack *temp,*new;
    phyAddr = phyAddr | tmp;
    
    //if (frameNum > 7){
    //    printf("ERROR: Frame Reference Out of Range\n");
    //    return 0xFFFFFFFF;
    //}
    //Init stack if NULL
    if (head == NULL){
        struct Stack* stack = newNode(page);
        stack->next = head;
        head = stack;
        tail = head;
        //printf("Head Assignment\n");
        //printf("FrameNum: %d, Accessing\n", frameNum);
        frame[frameNum]->page = page;
        frame[frameNum]->address = phyAddr;
        //printf("Assigned Frame Information\n");
        return phyAddr;
    }
    //temp = head;
    //printf("Page Stack[");
    //while (temp != NULL){
        //if (temp->next == NULL)
            //printf("%d",temp->page);
        //else
            //printf("%d,",temp->page);
        //temp = temp->next;
    //}
    //printf("]\n");

    //printf("Frame[");
    //for (int i = 0; i<maxFrames; i++){
        //if (i == maxFrames -1){
            //printf("(%d) %d",i,frame[i]->page);
            //continue;
        //}
        //printf("(%d) %d | ",i,frame[i]->page);
    //}
    //printf("]\n");
            
    
    //Search for existing page on stack
    for (struct Stack *current = head; current->next != NULL; current = current->next){
        if (current->page == page){
            if (current == head){
                frame[frameNum]->page = page;
                frame[frameNum]->address = phyAddr;
                return phyAddr;
            }
            if (current == tail){
                //printf("Found Page at Tail\n");
                tail = current->prev;
                tail->next = NULL;
                current->prev = NULL;
                head->prev = current;
                current->next = head;
                head = current;
                frame[frameNum]->page = page;
                frame[frameNum]->address = phyAddr;
                return phyAddr;
            }
            
            if (current != head && current !=tail){
                //printf("Found page in middle\n");
                temp = current->prev;
                temp->next = current->next;
                current->next->prev = temp;
                current->prev = NULL;
                head->prev = current;
                current->next = head;
                head = current;
                frame[frameNum]->page = page;
                frame[frameNum]->address = phyAddr;
                return phyAddr;
            }

        }
    }
    //printf("Passed Stack Search\n");
                
    //Didn't find matching page, insert new access
    if(getCount(head) != 7){
        struct Stack* stack = newNode(page);
        stack->next = head;
        head->prev = stack;
        head = stack;
        //printf("Assigned new head, FrameNum: %d\n",frameNum);
        frame[frameNum]->page = page;
        frame[frameNum]->address = phyAddr;
    }
    return phyAddr;
}

 struct Stack* newNode(int page) {
     struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
     stack->page=page;
     stack->next=NULL;
     stack->prev=NULL;
     return stack;
 }


int getCount(struct Stack* head)
 {
     int count = 0;  // Initialize count
     struct Stack* current = head;  // Initialize current
     while (current != NULL)
     {
         count++;
         current = current->next;
     }
     return count;
 }


unsigned long runLRU(){
    struct Stack *temp = tail;
    tail = temp->prev;
    tail->next = NULL;
    for(unsigned long i = 1;i<maxFrames;i++){
        if (frame[i]->page == temp->page){
            free(temp);
            //printf("runLRU will free frame: %d\n",i);
            return i;
        }
    }

}


