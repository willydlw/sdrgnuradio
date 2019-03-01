#define _POSIX_C_SOURCE 200112L          // pselect
#define  _DEFAULT_SOURCE                 // psignal 

#include <errno.h>              
#include <stdio.h>              // perror
#include <stdlib.h>             // atoi
#include <string.h>             // memset
#include <signal.h>             // sig_atomic_int, SIGINT, SIGTERM

#include <sys/types.h>          // socket
#include <sys/socket.h>         // socket, sendto
#include <sys/select.h>         // select, FD_SET, FD_ZERO,

#include <unistd.h>             // close

#include <netinet/in.h>         // sockaddr_in

#include "txrxhelper.h"


/*============== Global Variable Declarations =============================*/


/* You have to be careful about the fact that access to a single datum is not necessarily atomic. 
    This means that it can take more than one instruction to read or write a single object. 
    In such cases, a signal handler might be invoked in the middle of reading or writing the object.

    To avoid uncertainty about interrupting access to a variable, you can use a particular data 
    type for which access is always atomic: sig_atomic_t. 

    Reading and writing this data type is guaranteed to happen in a single instruction, 
    so there’s no way for a handler to run “in the middle” of an access.

    The type sig_atomic_t is always an integer data type, but which one it is, and how many bits 
    it contains, may vary from machine to machine.

    https://www.gnu.org/software/libc/manual/html_node/Atomic-Types.html#Atomic-Types
*/

static volatile sig_atomic_t exitRequest = 0;


/*========================= Function Definitions ==========================*/



/**
* @brief Sets exit request flag to 1 when SIGINT is received
*        or when SIGTERM is raised
*
* @param[in]    sig                       signal passed from operating system 
*
* @param[out]
*       sig_atomic_t    exitRequest      flag set to 1 when signal received
*                                         Defined as global
*
* @return void       
*
*      
* @note
*       Function is not directly invoked. Operating system calls it when
*       the registered signal is received. Signals are software interrupts.
*
*/
static void signal_handler_term(int sig)
{
    /* psignal used for debugging purposes
       debugging console output lets us know this function was
       triggered. Prints the string message and a string for the
       variable sig
    */
    psignal(sig, "signal_handler_term");
    if(sig == SIGINT || sig == SIGTERM){
        exitRequest = 1;
    }
    
}



int main(int argc, char **argv){

    // file descriptor variables
    int udpfd;
    int maxfd;                          // largest file descriptor value    

    // select variables
    fd_set readset;
    int selectReturn;

    // address variables
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    socklen_t addresslen;

    // message variables
    uint8_t buffer[256];
    ssize_t bytesReceived;

    const char* errormsg = "server did not receive udp client message";

    // packet variables
    int packetLength = 16;
    int packetsReceived = 0;
    uint16_t packetNumber;
    float percentReceived;

    // output file variables
    char *outputFileName = "../recorded_data/packetsReceived.bin";
    FILE *outptr = NULL;

    // signal handling
    sigset_t sigmask;
    sigset_t empty_mask;
    

    /* struct sigaction{
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t *, void*);
        sigset_t    sa_mask;
        int     sa_flags;
        void (*sa_restorer)(void);
    }
    */
    struct sigaction saterm;            // SIGTERM raised by this program or another
    struct sigaction saint;             // SIGINT caused by ctrl + c

    struct timespec timeout;            // timeout for pselect


     // ensure minimum number arguments passed to main
    if(argc < 2){
        fprintf(stderr, "usage %s packetLength\n", argv[0]);
        return -1;
    }

    packetLength = atoi(argv[1]);


     // open output file
    outptr = fopen(outputFileName, "wb");
    if(outptr == NULL){
        fprintf(stderr, "%s failed to open\n", outputFileName);
        return -1;
    }

    // create UDP socket
    if( (udpfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("Error creating upd socket");
        exitGracefully(&outptr, &udpfd);
        return -1;
    }


    // set server address structure
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(8080);

    // bind server address structure to udp socket file descriptor
    if( bind(udpfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0){
        perror("Error binding udp socket");
        exitGracefully(&outptr, &udpfd);
        return -1;
    }


    // pselect requires an argument that is 1 more than
    // the largest file descriptor value
    maxfd = udpfd + 1;   


    // pselect does not change the timeout argument, so we only need to
    // initialize it here. If the code is changed to use select instead
    // of pselect, then this initialization should be moved inside the
    // while loop, as select updates the timeout argument, deducting
    // elapsed time from it. 
    // reference: http://man7.org/linux/man-pages/man2/select.2.html 
    timeout.tv_sec = 2;                     // seconds
    timeout.tv_nsec = 0;                    // nanoseconds   


    // register the SIGTERM signal handler function
    memset(&saterm, 0, sizeof(saterm));
    saterm.sa_handler = signal_handler_term;

    /*  The sigaction() system call is used to change the action 
        taken by a process on receipt of a specific signal.
    */
    if(sigaction(SIGTERM, &saterm, NULL) < 0){
        fprintf(stderr, "FATAL: sigaction saterm, errno: %s\n", strerror(errno));
        exitGracefully(&outptr, &udpfd);
        return 1;
    }
    
    // register the SIGINT signal handler function
    memset(&saint, 0, sizeof(saint));
    saint.sa_handler = signal_handler_term;
    if(sigaction(SIGINT, &saint, NULL) < 0){
        fprintf(stderr, "FATAL: sigaction saint, errno: %s\n", strerror(errno));
        exitGracefully(&outptr, &udpfd);
        return 1;
    }

    // signal mask initialization
    sigemptyset(&sigmask);
    sigemptyset(&empty_mask);

    sigaddset(&sigmask, SIGTERM);
    sigaddset(&sigmask, SIGINT);

    // set as blocking so that pselect can receive event
    if(sigprocmask(SIG_BLOCK, &sigmask, NULL) < 0){
        fprintf(stderr, "FATAL: sigprocmask, errno: %s\n", strerror(errno));
        return 1;
    }


    // clear the file desriptor set
    FD_ZERO(&readset);
    
    while(exitRequest == 0){
        /*  can get SIGTERM, SIGINT at this point, but they will be delivered while
            in pselect, because SIGTERM, SIGINT are blocked.
        */
       
       // build the file descriptor set
       FD_ZERO(&readset);
       FD_SET(udpfd, &readset);
       
       /* pselect will return when one of the requested operations is ready,
       *  a timeout occurs, or a signal interrupt occurs
       */
       selectReturn = pselect(maxfd, &readset, NULL, NULL, &timeout, &empty_mask);

       // was a signal interrupt received while executing pselect?
       if(exitRequest == 1){
            fprintf(stderr, "info: received exit request\n");
            break;
        }  

        if(selectReturn < 0 && errno!= EINTR){ 
            // EINTR means signal was caught
            fprintf(stderr, "Error, select failure, errno: %s\n", strerror(errno));
            break;
        }
		
       // is the udp socket readable?
       if( (FD_ISSET(udpfd, &readset))){
           addresslen = sizeof(clientaddr);
           memset(buffer, 0, sizeof(buffer));
           bytesReceived = recvfrom(udpfd, buffer, sizeof(buffer),0,
                    (struct sockaddr*)&clientaddr, &addresslen);

           if(bytesReceived > 0){
               // sometimes multiple packets are read
               // ex: packetLength of 16, bytes received 32
               packetsReceived += (bytesReceived/packetLength);
               packetNumber = (buffer[0] << 8) | buffer[1];
               
               // packetNumber + 1 because first packet number is 0
               percentReceived = (float)packetsReceived / ((float)packetNumber + 1) * 100.0f;

               fprintf(stderr, "bytesReceived: %3ld, packetsReceived: %3d, packet number: %5d, "
                            "percent received: %5.2f\n", bytesReceived, packetsReceived, 
                            packetNumber, percentReceived);

               recordData(outptr, buffer, bytesReceived);
           }
           else{
               fprintf(stderr, "recvfrom failed to return message, bytesReceived: %ld, numReady: %d\n", 
                                bytesReceived, selectReturn);
           }
       }
   }

   exitGracefully(&outptr, &udpfd);
   return 0;

}