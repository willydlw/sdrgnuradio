#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "checkdata.h"

void packetfrequencycount(const char* inputFileName, int packetLength, int totalPackets)
{
    int *frequencyCount = (int*)calloc(totalPackets, sizeof(int));
    int outofRangeCount = 0;
    int missingPacketCount = 0;
    int duplicatePacketCount = 0;
    int i;

    uint16_t packetNumber;
    uint8_t buffer[packetLength];
    size_t bytesRead;
    
    FILE *inptr = fopen(inputFileName,"rb");
    if(inptr == NULL){
        fprintf(stderr, "ERROR opening %s\n", inputFileName);
        return;
    }  

    const char* mFileName = "./reports/packetCountErrors.txt";
    FILE *countptr = fopen(mFileName, "w");
    if(countptr == NULL){
        fprintf(stderr, "ERROR opening %s\n", mFileName);
        return;
    }  

    const char* rFileName = "./reports/outofRangePacketCount.txt";
    FILE *rangeptr = fopen(rFileName, "w");
    if(rangeptr == NULL){
        fprintf(stderr, "ERROR opening %s\n", rFileName);
        return;
    } 

    fprintf(countptr, "Input File: %s\n", inputFileName);
    fprintf(rangeptr, "Input File: %s\n", inputFileName);

    i = 0;
    while(!feof(inptr) && i < totalPackets){
         bytesRead = fread(buffer, 1, packetLength, inptr);

         if(bytesRead >= 2){ // packet count occupies two bytes
            packetNumber = (uint16_t)buffer[0] << 8 | (uint16_t)buffer[1];
            if(packetNumber < totalPackets){
                frequencyCount[packetNumber] += 1;
            }
            else{
                fprintf(rangeptr, "out of range packet number: %hu\n", packetNumber);
                ++outofRangeCount;
            }
         }
        else{
            fprintf(stderr, "warning, loop iteration i: %d, bytes read: %lu\n",i, bytesRead);
        }
        ++i;
    }

    // record totals
    fprintf(rangeptr, "\n\nout of range count: %d\n", outofRangeCount);

    // does i equal total packet count? if not, there was a file error that ended loop
    if(i != totalPackets && !feof(inptr)){
        fprintf(stderr, "\nwarning, not all packets read, i: %hu, totalPackets: %hu\n", i, totalPackets);
    }

    // count missing packets and record data
    fprintf(countptr, "Missing Packet Numbers\n");

    for(i = 0; i < totalPackets; ++i){
        if(frequencyCount[i] == 0){         // missing
            ++missingPacketCount;
            fprintf(countptr, "%6hu ", i);
        }
        if( ((missingPacketCount+1)%10) == 0){                // blank line
            fprintf(countptr, "\n");
        }
    }

    // record total
    fprintf(countptr, "\n\nmissing Packet Count: %d\n", missingPacketCount);

    // record duplicate packets
    fprintf(countptr, "\n\nDuplicate Packet Counts\n");
    for(i = 0; i < totalPackets; ++i){
        if(frequencyCount[i] > 1){  
            ++duplicatePacketCount;       
            fprintf(countptr, "packet number: %6hu, frequency: %6hu ", i, frequencyCount[i]);
        }
        if( ((duplicatePacketCount+1)%10) == 0){                // blank line
            fprintf(countptr, "\n");
        }
    }

    // record totals
    fprintf(countptr, "\n\nduplicate Packet Count: %d\n", duplicatePacketCount);


    // display summary data in terminal
    fprintf(stderr, "\nFile name: %s\n", inputFileName);
    fprintf(stderr, "Out of range packet count: %5d\n", outofRangeCount);
    fprintf(stderr, "duplicate packets count:   %5d\n", duplicatePacketCount);
    fprintf(stderr, "missing packet count:      %5d\n\n", missingPacketCount);

    // clean up
    free(frequencyCount);
    fclose(inptr);
    fclose(countptr);
    fclose(rangeptr);
}





