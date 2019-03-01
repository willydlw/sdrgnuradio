#ifndef TXRXHELPER_H_INCLUDED
#define TXRXHELPER_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

#include <netinet/in.h>         // struct sockaddr


#ifdef __cplusplus
extern "C" void exitGracefully(FILE **fptr, int *socketfd);
extern "C" void formPacket(uint8_t *packetBuffer, int packetLength, uint16_t packetNumber);
extern "C" void recordData(FILE *out, const uint8_t *packetBuffer, int packetLength);
extern "C" int udp_send_data(int socketfd, const uint8_t* data, size_t len, struct sockaddr_in *server_addr);
extern "C" void formDummyPacket(uint8_t *packetBuffer, int packetLength);
#else
void exitGracefully(FILE **fptr, int *socketfd);
void formPacket(uint8_t *packetBuffer, int packetLength, uint16_t packetNumber);
void recordData(FILE *out, const uint8_t *packetBuffer, int packetLength);
int udp_send_data(int socketfd, const uint8_t* data, size_t len, struct sockaddr_in *server_addr);
void formDummyPacket(uint8_t *packetBuffer, int packetLength);
#endif


#endif



