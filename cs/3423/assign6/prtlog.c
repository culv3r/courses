#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"

int main (int argc, char *argv[]){
    int fd;
    int iBufPos = 0;
    int iCount = 0;
    int firsttime = 1;
    char *szBuf[BUFSIZ];
    struct pcap_file_header pcap;
    struct my_pkthdr phdr;
    struct eth_hdr ethhdr;
    struct arp_hdr arphdr;
    struct ip_hdr iphdr;
    size_t size;
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
        read(fd,&ethhdr,sizeof ethhdr);
        size = (size_t)phdr.len - sizeof ethhdr;
        printf("Ethernet Header\n");
        switch(ntohs(ethhdr.eth_type)){
            case ETH_TYPE_PUP :
                printf("  PUP\n");
                break;
            case ETH_TYPE_IP :
                printf("  IP\n");
                break;
            case ETH_TYPE_ARP :
                printf("  ARP\n");
                break;
            case ETH_TYPE_REVARP :
                printf("  REVARP\n");
                break;
            case ETH_TYPE_8021Q :
                printf("  802.1Q VLAN\n");
                break;
            case ETH_TYPE_IPV6 :
                printf("  IPv6\n");
                break;
            case ETH_TYPE_MPLS :
                printf("  MPLS\n");
                break;
            case ETH_TYPE_MPLS_MCAST :
                printf("  MPLS Multicast\n");
                break;
            case ETH_TYPE_PPPOEDISC :
                printf("  PPPOE Discovery\n");
                break;
            case ETH_TYPE_PPPOE :
                printf("  PPPOE\n");
                break;
            case ETH_TYPE_LOOPBACK :
                printf("  LO\n");
                break;
            default :
                printf("  UNRECOGNIZED\n");
        }

        if ( ntohs(ethhdr.eth_type) == ETH_TYPE_IP){
            read(fd,&iphdr,sizeof iphdr);
            switch(iphdr.ip_p){
                case IP_PROTO_ICMP :
                    printf("    ICMP\n");
                    break;
                case IP_PROTO_IGMP :
                    printf("    IGMP\n");
                    break;
                case IP_PROTO_TCP :
                    printf("    TCP\n");
                    break;
                case IP_PROTO_UDP :
                    printf("    UDP\n");
                    break;
                default :
                    printf("    UNRECOGNIZED\n");
            }
            size -= sizeof iphdr;
        }
        if ( ntohs(ethhdr.eth_type) == ETH_TYPE_ARP){
            read(fd,&arphdr,sizeof arphdr);
            switch(ntohs(arphdr.ar_op)){
                case ARP_OP_REQUEST :
                    printf("    arp operation = Arp Request\n");
                    break;
                case ARP_OP_REPLY :
                    printf("    arp operation = Arp Reply\n");
                    break;
                case ARP_OP_REVREQUEST :
                    printf("    arp operation = Reverse Arp Request\n");
                    break;
                case ARP_OP_REVREPLY :
                    printf("    arp operation = Reverse Arp Reply\n");
                    break;
                default :
                    printf("    UNRECOGNIZED\n");
                    }
            size -= sizeof arphdr;
        }
        printf("\n");
        read(fd,szBuf,size);
        
        
    }
    close(fd);
    return 0;

}
