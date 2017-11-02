#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    int pageTable[] = {2,4,1,7,3,5,6};
    int fd[2];
    unsigned long addr,tmp,phys;
    unsigned short index;
    size_t result;

    if (argc != 2){
        perror("Usage: assign3part1 sequencefile\n");
        exit(-1);
    }
    
    fd[0] = open(argv[1], O_RDONLY);
    fd[1] = creat("output-part1", 0666);

    if (fd[0] < 0 | fd[1] < 0){
        perror("Open Error");
        exit(-1);
    }

    while ((read(fd[0], &addr, 8)) > 0){
        //printf("Read in 0x%08X %d\n", (int)addr & 0xFFFFFFFF, addr);
        tmp = addr >> 7;
        index = (short) tmp;
        //printf("Index is Ox%02X\n", index);
        tmp = addr & 0x7F;
        //printf("Data is 0x%02X\n", tmp);
        phys = (unsigned long)pageTable[index];
        phys = phys << 7;
        phys = phys | tmp;
        //printf("Physical Address is 0x%08X\n", phys);
        write(fd[1],&phys,sizeof(phys));

    }

    close(fd[0]);
    close(fd[1]);

    return 1;
}
