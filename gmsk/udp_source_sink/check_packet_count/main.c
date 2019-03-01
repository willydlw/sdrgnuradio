#include <stdio.h>
#include <stdlib.h>         // atoi

#include "checkdata.h"

int main(int argc, char **argv){

    int packetLength;
    int totalPackets;

    // ensure minimum number arguments passed to main
    if(argc < 4){
        fprintf(stderr, "usage %s filename packetLength totalPackets\n", argv[0]);
        return -1;
    }

    packetLength = atoi(argv[2]);
    totalPackets = atoi(argv[3]);

    packetfrequencycount(argv[1], packetLength, totalPackets);
 
    
    return 0;
}