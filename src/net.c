/******************************************************************
 *  Copyright (c) 2017 Lucas Vieira <lucas.samuel2002@gmail.com>  *
 *  This file is part of MyLain.                                  *
 *                                                                *
 *  OficinaFramework is free software: you can redistribute       *
 *  it and/or modify it under the terms of the GNU Lesser         *
 *  General Public License as published by the Free Software      *
 *  Foundation, either version 3 of the License, or (at your      *
 *  option) any later version.                                    *
 *                                                                *
 *  You should have received a copy of the GNU Lesser General     *
 *  Public License along with MyLain. If not, see                 *
 *  <http://www.gnu.org/licenses/>.                               *
 ******************************************************************/

#include "mylain/net.h"
#include "mylain/global.h"
#include "mylain/lexer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <fcntl.h>

unsigned  LAIN_LOCAL_SEND_PORT;
int       LAIN_LOCAL_SEND_SOCKET;
char*     LAIN_LOCAL_IP;
unsigned  LAIN_NET_READY;
char*     LAIN_NET_INTERFACE;
pthread_t LAIN_NET_LISTENER_THREAD;
sem_t     LAIN_NET_LISTENER_SEMAPHORE;

lain_com_queue_t* LAIN_REMOTE_COM_QUEUE;

static struct sockaddr_in sa_local;
static struct sockaddr_in sa_remote;


unsigned lain_net_setup(void)
{
    // Initialize variables
    LAIN_LOCAL_SEND_PORT = LAIN_DEFAULT_PORT;
    LAIN_NET_READY = 0;
    LAIN_LOCAL_IP = malloc(9 * sizeof(char));
    strcpy(LAIN_LOCAL_IP, "<unknown>");
    LAIN_NET_INTERFACE = malloc(strlen(LAIN_DEFAULT_INTERFACE) * sizeof(char));
    strcpy(LAIN_NET_INTERFACE, LAIN_DEFAULT_INTERFACE);
    LAIN_LOCAL_SEND_SOCKET = -1;
    LAIN_REMOTE_COM_QUEUE = malloc(sizeof(lain_com_queue_t));
    LAIN_REMOTE_COM_QUEUE->first = LAIN_REMOTE_COM_QUEUE->last = NULL;
    LAIN_REMOTE_COM_QUEUE->amount = 0ul;
    sem_init(&LAIN_NET_LISTENER_SEMAPHORE, 0, 1);

    // Initialize listener thread
    assert(pthread_create(&LAIN_NET_LISTENER_THREAD, NULL, lain_net_listener_loop, NULL) == 0);
    
    return LAIN_RETURN_SUCCESS;
}

void* lain_net_listener_loop(void* unused)
{
    // Prepare default suspension time
    struct timespec suspension_time;
    suspension_time.tv_sec = 0;
    suspension_time.tv_nsec = 400000;
    
    lain_remote_t remote_com;

    // "Infinite" loop for job execution
    while(LAIN_LOCAL_RUNNING) {
        // Sleep for 400ms, then do
        nanosleep(&suspension_time, NULL);

        // Receive message
        struct sockaddr from;
        socklen_t       fromsz;
        if(recvfrom(LAIN_LOCAL_SEND_SOCKET, &remote_com, sizeof(lain_remote_t), 0, &from, &fromsz) > 0) {
            // Process machine state
            sem_wait(&LAIN_NET_LISTENER_SEMAPHORE);
            lain_com_enqueue(LAIN_REMOTE_COM_QUEUE, remote_com.MSTATE);
            sem_post(&LAIN_NET_LISTENER_SEMAPHORE);
        }
        
    }

    // Properly kill thread
    pthread_exit(NULL);
    return NULL; // Avoid warnings
}

unsigned lain_net_connect(const char* address)
{
    // Open socket
    printf("Opening socket...\n");
    LAIN_LOCAL_SEND_SOCKET = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    assert(LAIN_LOCAL_SEND_SOCKET != -1);

    struct ifreq ifr;
    printf("Requesting IPv4 address to interface %s...\n", LAIN_NET_INTERFACE);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, LAIN_NET_INTERFACE, IFNAMSIZ-1);
    ioctl(LAIN_LOCAL_SEND_SOCKET, SIOCGIFADDR, &ifr);

    // Fetch local IP
    free(LAIN_LOCAL_IP);
    LAIN_LOCAL_IP = malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr, LAIN_LOCAL_IP, INET_ADDRSTRLEN);

    // Bind local IP and port
    memset(&sa_local, 0, sizeof(struct sockaddr_in));
    sa_local.sin_family = AF_INET;
    sa_local.sin_port   = htons(LAIN_LOCAL_SEND_PORT);
    sa_local.sin_addr.s_addr =  INADDR_ANY;
    assert(bind(LAIN_LOCAL_SEND_SOCKET, (struct sockaddr*)&sa_local, sizeof(struct sockaddr)) != -1);

    // Set non-blocking mode
    assert(fcntl(LAIN_LOCAL_SEND_SOCKET, F_SETFL, O_NONBLOCK, 1) != -1);

    LAIN_NET_READY = 1;

    printf("Currently listening on port %u.\n", LAIN_LOCAL_SEND_PORT);

    printf("Sorry, this is an incomplete feature\n");

    // Setup remote server
    memset(&sa_remote, 0, sizeof(struct sockaddr_in));
    sa_remote.sin_family = AF_INET;
    sa_remote.sin_port = htons(LAIN_LOCAL_SEND_PORT);
    sa_remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    puts("Connecting to localhost...");
    /*if(connect(LAIN_LOCAL_SEND_SOCKET, (struct sockaddr*)&sa_remote, sizeof(struct sockaddr)) == -1) {
        printf("Couldn't connect.\n");
        return LAIN_RETURN_FAILURE;
        }*/
    
    puts("Sending message...");
    lain_remote_t message;
    message.MSTATE = LAIN_COM_STATUS;
    message.SUBSTATE = 0ul;
    int i;
    for(i = 0; i < 5; i++) {
        if(sendto(LAIN_LOCAL_SEND_SOCKET, (void*)&message, sizeof(lain_remote_t), 0,
                  (struct sockaddr*)&sa_remote, sizeof(struct sockaddr)) == -1) {
            printf("Couldn't send message #%d.\n", i);
            return LAIN_RETURN_FAILURE;
        }
    }
    
    return LAIN_RETURN_SUCCESS;
}

unsigned lain_net_dispose(void)
{
    // Join thread
    pthread_join(LAIN_NET_LISTENER_THREAD, NULL);
    
    if(LAIN_LOCAL_SEND_SOCKET != -1) {
        assert(close(LAIN_LOCAL_SEND_SOCKET) != -1);
    }
    lain_com_queue_clear(LAIN_REMOTE_COM_QUEUE);
    free(LAIN_REMOTE_COM_QUEUE);
    
    return LAIN_RETURN_SUCCESS;
}
