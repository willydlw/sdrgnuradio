#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>             // atoi
#include <string.h>             // memset, memcpy
#include <time.h>               // nanosleep

#include <sys/socket.h>
#include <arpa/inet.h>          // inet_addr
#include <netinet/in.h>         
#include <netdb.h>              // gethostbyname

#include "txrxhelper.h"

int main(int argc, char **argv)
{
    // socket variables
    int udpsocketfd = -1;
    int portNumber;
    struct hostent *server;
    struct sockaddr_in serveraddr;

      // packet variables
    int packetLength;
    uint16_t packetCount = 0;
    uint16_t totalPackets;

    // output file variables
    char *outputFileName = "../recorded_data/packetsTransmitted.bin";
    FILE *outptr = NULL;

    // timing variable
    /* wait 0 sec and 10^8 nanosec */
    struct timespec ts = {0, 100000000L };

    srand(time(NULL));

    // ensure minimum number arguments passed to main
    if(argc < 5){
        fprintf(stderr, "usage %s hostname port packetLength totalPackets\n", argv[0]);
        return -1;
    }

    // open output file
    outptr = fopen(outputFileName, "wb");
    if(outptr == NULL){
        fprintf(stderr, "%s failed to open\n", outputFileName);
        return -1;
    }

    // create a udp socket file destriptor
	if ( (udpsocketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("ERROR: socket creation failed"); 
        exitGracefully(&outptr, &udpsocketfd);
        return -1;
	}

    // populate server, port number, and packet length
    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "ERROR, unknown host: %s\n",argv[1]);
        exitGracefully(&outptr, &udpsocketfd);
        return -1;
    }

    portNumber = atoi(argv[2]);
    packetLength = atoi(argv[3]);
    totalPackets = atoi(argv[4]);

    if(packetLength < 2){
        fprintf(stderr, "packetLength %d too small\n", packetLength);
        exitGracefully(&outptr, &udpsocketfd);
        return -1;
    }

    // create packet buffer now that length is known
    uint8_t packetBuffer[packetLength];
    

    // Filling server information 
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_port = htons(portNumber);; 
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    
    
    while(packetCount < totalPackets){

        formPacket(packetBuffer, packetLength, packetCount);
        // write packet buffer to file to enable checking
        // the data the socket receives is the same as the data sent
        recordData(outptr, packetBuffer, packetLength);

        
        // send data to socket server
        if( udp_send_data(udpsocketfd, packetBuffer, packetLength, &serveraddr) == -1){
            break;
        }
        else{
            // lets user know program is still running and how many packets have been sent
            if( (packetCount % 20) == 0){
                fprintf(stderr, "sent packet number %hu\n", packetCount);
            }
            ++packetCount;
        }
        
        // wait a brief time before sending next packet
        nanosleep (&ts, NULL); 
    }

    
    // send dummy packet
    formDummyPacket(packetBuffer, packetLength);
    recordData(outptr, packetBuffer, packetLength);
     // send data to socket server
    if( udp_send_data(udpsocketfd, packetBuffer, packetLength, &serveraddr) == -1){
        fprintf(stderr, "failed to send dummy packet\n");
    }
    else{
        fprintf(stderr, "sent dummy packet\n");
    }


    // wait a brief time before exiting
    nanosleep (&ts, NULL); 
    fprintf(stderr, "all packets transmitted\n");
    exitGracefully(&outptr, &udpsocketfd);


    return 0;
}