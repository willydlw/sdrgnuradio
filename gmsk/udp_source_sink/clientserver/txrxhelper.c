#include "txrxhelper.h"

#include <stdlib.h>             // rand
#include <unistd.h>             // close
#include <string.h>             // memset

#include <sys/types.h>
#include <sys/socket.h>

void exitGracefully(FILE **fptr, int *socketfd)
{
    if(fptr != NULL){
        fclose(*fptr);
        *fptr = NULL;
    }
    if(*socketfd != -1){
        close(*socketfd);
        *socketfd = -1;
    }
}

void formPacket(uint8_t *packetBuffer, int packetLength, uint16_t packetNumber)
{
    // extract and store left most byte of packet number
    packetBuffer[0] = (uint8_t)(packetNumber >> (uint8_t)8U);

    // extract and store right most byte of packet number
    packetBuffer[1] = (uint8_t)(packetNumber & 0x00FF);

    for(int i = 2; i < packetLength; ++i){
        packetBuffer[i] = (uint8_t)(rand()%256);
    }
}

void formDummyPacket(uint8_t *packetBuffer, int packetLength)
{
    memset(packetBuffer, 0, packetLength*sizeof(uint8_t));
    packetBuffer[0] = (uint8_t)0xFF;
    packetBuffer[1] = (uint8_t)0xFF;
}

void recordData(FILE *out, const uint8_t *packetBuffer, int packetLength)
{
    fwrite((packetBuffer), sizeof(uint8_t),packetLength,out);
}


int udp_send_data(int socketfd, const uint8_t* data, size_t len, struct sockaddr_in *server_addr)
{
    size_t index = 0;

    /** loop until all data is sent or send function fails
    *
    *   Upon successful completion, send() shall return the number of bytes sent.
    *   Otherwise, -1 shall be returned and errno set to indicate the error.
    */
    while( index < len)
    {
        /** ssize_t sendto(int sockfd, const void *message, size_t len, int flags,
         *                  const struct sockaddr *dest_addr, socklen_t dest_len);
        */
       ssize_t bytes_sent = sendto(socketfd, &data[index], len-index, 
           0, (const struct sockaddr*)server_addr, sizeof(*server_addr)); 

        //fprintf(stderr, "bytes_sent: %ld\n", bytes_sent);

        if(bytes_sent < 1)  /// send failed
        {
            perror("ERROR failed to send udp data");
            return -1;
        }

        index += (size_t)bytes_sent;

    }
    return 0;
}