#ifndef CHECKDATA_H_INCLUDED
#define CHECKDATA_H_INCLUDED

#ifdef __cplusplus
extern "C" void packetfrequencycount(const char* inputFileName, int packetLength, int totalPackets);
#else
void packetfrequencycount(const char* inputFileName, int packetLength, int totalPackets);
#endif


#endif
