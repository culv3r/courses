#include "assign3part2.h"
#include <unistd.h>

int main(int argc, char *argv[]){
    unsigned long addr,*phyAddr;
    int pages,frames,result,fault;
    int fd[2];
    if (argc != 2){
         perror("Usage: assign3part1 sequencefile\n");
         exit(-1);
     }

    pages = VMEM_SIZE/PAGE_BYTES;
    frames = PHY_SIZE/PAGE_BYTES;

    //printf("Making %d pages and %d frames.\n",pages,frames);

    initTable(pages);
    initFrame(frames);

     fd[0] = open(argv[1], O_RDONLY);
     fd[1] = creat("output-part2", 0666);

     if (fd[0] < 0 | fd[1] < 0){
         perror("Open Error");
         exit(-1);
     }

     phyAddr = malloc(sizeof(unsigned long *));
     fault = 0;

     while ((read(fd[0], &addr, 8)) > 0){
        result = pageTable(addr,phyAddr);
        if (result == 1)
            fault++;
        //printf("Virtual: 0x%08X Physical: 0x%08X\n",addr,*phyAddr);
        write(fd[1],phyAddr,sizeof(phyAddr));
     }
     printf("Page Faults: %d\n",fault);

     close(fd[0]);
     close(fd[1]);
     return 1;

}
