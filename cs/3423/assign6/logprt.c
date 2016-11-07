#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "logprt.h"

int main (int argc, char *argv[]){
    int fd;
    int iBufPos = 0;
    int iCount = 0;
    int firsttime = 1;
    char *szBuf[BUFSIZ];
    struct pcap_file_header pcap;
    struct my_pkthdr phdr;
    unsigned int b_sec,c_sec;
    int b_usec,c_usec;

    if (argc < 1){
        printf("You must include a tcpdump file.\nUsage: logprt tcpdmp.file\n");
        exit (-1);
    }

    fd = open(argv[1],O_RDONLY);

    if (fd == -1){
        perror(argv[1]);
        exit (-1);
    }

    read(fd,&pcap,sizeof pcap);
  
    if (pcap.magic == PCAP_MAGIC){
        printf("PCAP_MAGIC\n");
    }
    if (pcap.magic == PCAP_SWAPPED_MAGIC){
        printf("PCAP_SWAPPED_MAGIC\n");
    }
    if (pcap.magic == PCAP_MODIFIED_MAGIC){
        printf("PCAP_MODIFIEID_MAGIC\n");
    }
    if (pcap.magic == PCAP_SWAPPED_MODIFIED_MAGIC){
        printf("PCAP_SWAPPED_MODIFIED_MAGIC\n");
    }
    
    printf("Version major number = %u\nVersion minor number = %u\nGMT to local correction = %u\nTimestamp accuracy = %u\nSnaplen = %u\nLinktype = %u\n\n",pcap.version_major,pcap.version_minor,pcap.thiszone,pcap.sigfigs,pcap.snaplen,pcap.linktype);

    while(read(fd,&phdr,sizeof phdr) > 0){
        if ( firsttime ){
            firsttime = 0;
            b_sec = phdr.ts.tv_sec;
            b_usec = phdr.ts.tv_usec;
        }
        c_sec = (unsigned)phdr.ts.tv_sec - b_sec;
        c_usec = (unsigned)phdr.ts.tv_usec - b_usec;
        while (c_usec < 0){
            c_usec += 1000000;
            c_sec--;
        }
        printf("Packet %d\n%05u.%06u\nCaptured Packet Length = %u\nActual Packet Length = %u\n",iCount,(unsigned)c_sec,(unsigned)c_usec,phdr.caplen,phdr.len);        
        iCount++;
        read(fd,szBuf,phdr.len);
    }
    close(fd);
    return 0;

}
