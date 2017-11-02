#include "assign3part2.h"

pageEntry **table;
int maxPages;

int initTable(int pages){
    table = malloc(pages*sizeof(*table)); 
    if (table == NULL){
        perror("Page Table Allocation Error");
        return -1;
    } else {
        maxPages = pages;
        for (int i = 0; i<pages; i++){
            table[i] = malloc(sizeof(**table));
            table[i]->valid = 0;
        }
        return 0;
    }
    return 1;
}


int pageTable(unsigned long virtAddr, unsigned long *phyResult){
    unsigned long tmp = virtAddr >> 7;
    unsigned long result;
    int count = 0;
    unsigned long frame = 0;
    int bFault = 0;

    unsigned long offset = virtAddr;
    offset = offset & 0x7F;

    //printf("Page: 0x%08X Offset: 0x%07X\n",tmp,offset);

    //Page is in Memory
    if (table[tmp]->valid == 1){
        //printf("PageTable: Page in Memory\n");
        *phyResult = phyTranslate(virtAddr, table[tmp]->frame);       
        return 0;
    }
    //Page is not in Memory
    if (table[tmp]->valid == 0){
        //printf("PageTable: Page not in Memory\n");
        //Count if there are any free frames
        for (int i = 0; i < maxPages; i++){
            if (count >= 7){
                 //printf("PageTable: Running LRU\n");
                 frame = runLRU();
                 //printf("Frame Value from LRU: %d\n",frame);
                 for (int j = 0;j<maxPages;j++){
                     if (table[j]->frame == frame){
                         table[j]->valid = 0;
                     }
                 }
                 break;
             }
     
            if (table[i]->valid == 1){
                count++;
            } 
            //printf("Current Count: %d\n",count);
        }
        
        if (frame == 0){
            if (count >= 7)
                frame = 7;
            else
                frame = count+1;
        }
        
        //printf("PageTable: Assigning Frame %d\n", frame);
        result = phyTranslate(virtAddr, frame);
        table[tmp]->valid=1;
        table[tmp]->frame=frame;
        *phyResult = result;
        //printf("Assigned phyresult: 0x%08X\n", *phyResult);

        return 1;

   }


}
