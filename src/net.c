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
unsigned  LAIN_LOCAL_RECV_PORT;
int       LAIN_LOCAL_SEND_SOCKET;
int       LAIN_LOCAL_RECV_SOCKET;
char*     LAIN_LOCAL_IP;
unsigned  LAIN_NET_READY;
char*     LAIN_NET_INTERFACE;
pthread_t LAIN_NET_LISTENER_THREAD;
sem_t     LAIN_NET_LISTENER_SEMAPHORE;

lain_com_queue_t* LAIN_REMOTE_COM_QUEUE;

static struct sockaddr_in sa_local_send;
static struct sockaddr_in sa_local_recv;
static struct sockaddr_in sa_remote;


unsigned lain_net_setup(void)
{
    // Initialize variables
    // Initialize port numbers
    LAIN_LOCAL_SEND_PORT = LAIN_DEFAULT_SEND_PORT;
    LAIN_LOCAL_RECV_PORT = LAIN_DEFAULT_RECV_PORT;
    // Deactivate net trigger
    LAIN_NET_READY = 0;
    // Zero-fill and initialize general stuff to null/error
    LAIN_LOCAL_IP = malloc(strlen("0.0.0.0") * sizeof(char));
    strcpy(LAIN_LOCAL_IP, "0.0.0.0");
    LAIN_NET_INTERFACE = malloc(strlen(LAIN_DEFAULT_INTERFACE) * sizeof(char));
    strcpy(LAIN_NET_INTERFACE, LAIN_DEFAULT_INTERFACE);
    LAIN_LOCAL_SEND_SOCKET = LAIN_LOCAL_RECV_SOCKET = -1;
    LAIN_REMOTE_COM_QUEUE = malloc(sizeof(lain_com_queue_t));
    LAIN_REMOTE_COM_QUEUE->first = LAIN_REMOTE_COM_QUEUE->last = NULL;
    LAIN_REMOTE_COM_QUEUE->amount = 0ul;
    sem_init(&LAIN_NET_LISTENER_SEMAPHORE, 0, 1);

    // Create listener socket
    LAIN_LOCAL_RECV_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    assert(LAIN_LOCAL_RECV_SOCKET != -1);

    // Bind listener socket to port
    memset(&sa_local_recv, 0, sizeof(struct sockaddr_in));
    sa_local_recv.sin_family      = AF_INET;
    sa_local_recv.sin_port        = htons(LAIN_LOCAL_RECV_PORT);
    sa_local_recv.sin_addr.s_addr = INADDR_ANY;
    assert(bind(LAIN_LOCAL_RECV_SOCKET, (struct sockaddr*)&sa_local_recv, sizeof(struct sockaddr)) >= 0);

    // Let socket listen to incoming connections
    assert(listen(LAIN_LOCAL_RECV_SOCKET, 10) != -1);
    
    // Set it to nonblocking mode
    assert(fcntl(LAIN_LOCAL_RECV_SOCKET, F_SETFL, O_NONBLOCK) != -1);

    // Fetch IP address
    lain_net_fetch_ip();

    // Initialize listener thread
    assert(pthread_create(&LAIN_NET_LISTENER_THREAD, NULL, lain_net_listener_loop, NULL) == 0);
    
    return LAIN_RETURN_SUCCESS;
}

unsigned lain_net_connect(const char* address)
{
    printf("Warning: this is an incomplete feature\n");
    if(LAIN_NET_READY == 0) {
        // Open socket
        printf("Opening socket...\n");
        //LAIN_LOCAL_SEND_SOCKET = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        LAIN_LOCAL_SEND_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
        assert(LAIN_LOCAL_SEND_SOCKET != -1);

        // Bind local IP and port
        memset(&sa_local_send, 0, sizeof(struct sockaddr_in));
        sa_local_send.sin_family = AF_INET;
        sa_local_send.sin_port   = htons(LAIN_LOCAL_SEND_PORT);
        sa_local_send.sin_addr.s_addr =  INADDR_ANY;
        assert(bind(LAIN_LOCAL_SEND_SOCKET, (struct sockaddr*)&sa_local_send, sizeof(struct sockaddr)) != -1);

        printf("Currently listening to port %u.\n", LAIN_LOCAL_RECV_PORT);
        
    
        // Setup remote server
        memset(&sa_remote, 0, sizeof(struct sockaddr_in));
        sa_remote.sin_family = AF_INET;
        sa_remote.sin_port = htons(LAIN_LOCAL_RECV_PORT);
        sa_remote.sin_addr.s_addr = inet_addr((address == NULL) ? "127.0.0.1" : address);

        puts("Connecting to localhost...");
        if(connect(LAIN_LOCAL_SEND_SOCKET, (struct sockaddr*)&sa_remote, sizeof(struct sockaddr)) == -1) {
            printf("Couldn't connect.\n");
            return LAIN_RETURN_FAILURE;
        }
        puts("Connected!");

        LAIN_NET_READY = 1;
    }
    
    puts("Sending message #1: Request printing status...");
    lain_net_dispatch_com(LAIN_COM_STATUS);
    lain_net_dispatch_com(LAIN_COM_END);

    puts("Sending message #2: Request printing motto...");
    lain_net_dispatch_com(LAIN_COM_CONFIG);
    lain_net_dispatch_atom("get");
    lain_net_dispatch_atom("motto");
    lain_net_dispatch_com(LAIN_COM_END);
    
    return LAIN_RETURN_SUCCESS;
}

unsigned lain_net_dispose(void)
{
    // Join thread
    pthread_join(LAIN_NET_LISTENER_THREAD, NULL);
    
    if(LAIN_LOCAL_SEND_SOCKET != -1)
        assert(close(LAIN_LOCAL_SEND_SOCKET) != -1);
    if(LAIN_LOCAL_RECV_SOCKET != -1)
        assert(close(LAIN_LOCAL_RECV_SOCKET) != -1);

    LAIN_NET_READY = 0;
    
    lain_com_queue_clear(LAIN_REMOTE_COM_QUEUE);
    free(LAIN_REMOTE_COM_QUEUE);
    
    return LAIN_RETURN_SUCCESS;
}

void* lain_net_listener_loop(void* unused)
{
    // Prepare default suspension time
    struct timespec suspension_time;
    suspension_time.tv_sec = 0;
    suspension_time.tv_nsec = 400000;
    
    lain_remote_t remote_com;
    int client_sock = -1;

    // "Infinite" loop for job execution
    while(LAIN_LOCAL_RUNNING) {
        // Sleep for 400ms, then do
        nanosleep(&suspension_time, NULL);

        // Receive message
        struct sockaddr from;
        socklen_t       fromsz;

        // Accept incoming connection
        if(client_sock < 0) {
            client_sock = accept(LAIN_LOCAL_RECV_SOCKET, &from, &fromsz);
            if(client_sock >= 0) {
                // Set client socket to nonblocking mode
                assert(fcntl(client_sock, F_SETFL, O_NONBLOCK) != -1);
            }
        }
        else {
            if(!LAIN_LOCAL_RUNNING)
                break;
            // Accepted connection
            // TODO: Print connection
            int bytes_read = recv(client_sock, &remote_com, sizeof(lain_remote_t), 0);
            if(bytes_read > 0) {
                if(((enum LAIN_COMMAND)remote_com.MSTATE) == LAIN_COM_ATOM) {
                    char* buffer = malloc(remote_com.SUBSTATE * sizeof(char));
                    recv(client_sock, buffer, remote_com.SUBSTATE * sizeof(char), 0);
                    // ... Then I need to do something to that buffer.
                    // for now I'll just free it.
                    //free(buffer);
                    sem_wait(&LAIN_NET_LISTENER_SEMAPHORE);
                    lain_com_enqueue_atom(LAIN_REMOTE_COM_QUEUE, buffer);
                    sem_post(&LAIN_NET_LISTENER_SEMAPHORE);
                } else {
                    // Process machine state
                    sem_wait(&LAIN_NET_LISTENER_SEMAPHORE);
                    lain_com_enqueue(LAIN_REMOTE_COM_QUEUE, remote_com.MSTATE);
                    sem_post(&LAIN_NET_LISTENER_SEMAPHORE);
                }
                // Answer
                //unsigned ans = LAIN_RETURN_SUCCESS;
                //send(client_sock, &ans, sizeof(ans), 0);
                // Or write a string
                //write(client_sock, msg_string, strlen(msg_string));
            } else if(bytes_read == 0) {
                client_sock = -1;
                // Client disconnected
                // TODO: print disconnection
            } else { /* recv failed */ }
        }
        /*
        if(recvfrom(LAIN_LOCAL_SEND_SOCKET, &remote_com, sizeof(lain_remote_t), 0, &from, &fromsz) > 0) {
            // Process machine state
            sem_wait(&LAIN_NET_LISTENER_SEMAPHORE);
            lain_com_enqueue(LAIN_REMOTE_COM_QUEUE, remote_com.MSTATE);
            sem_post(&LAIN_NET_LISTENER_SEMAPHORE);
            }*/
        
    }

    // Properly kill thread
    pthread_exit(NULL);
    return NULL; // Avoid warnings
}

void lain_net_fetch_ip(void)
{
    assert(LAIN_LOCAL_RECV_SOCKET != -1);
    struct ifreq ifr;
    
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, LAIN_NET_INTERFACE, IFNAMSIZ-1);
    ioctl(LAIN_LOCAL_RECV_SOCKET, SIOCGIFADDR, &ifr);
        
    // Fetch local IP
    free(LAIN_LOCAL_IP);
    LAIN_LOCAL_IP = malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr, LAIN_LOCAL_IP, INET_ADDRSTRLEN);
}

unsigned lain_net_dispatch_com(unsigned long long command)
{
    if(LAIN_NET_READY) {
        lain_remote_t message;
        message.MSTATE = command;
        message.SUBSTATE = 0ul;
        if(send(LAIN_LOCAL_SEND_SOCKET, (void*)&message, sizeof(lain_remote_t), 0) < 0)
            return LAIN_RETURN_FAILURE;
        return LAIN_RETURN_SUCCESS;
    }
    return LAIN_RETURN_FAILURE;
}

unsigned lain_net_dispatch_atom(const char* atom)
{
    if(LAIN_NET_READY) {
        lain_remote_t message;
        message.MSTATE   = LAIN_COM_ATOM;
        message.SUBSTATE = strlen(atom) + 1;
        if(send(LAIN_LOCAL_SEND_SOCKET, (void*)&message, sizeof(lain_remote_t), 0) < 0)
            return LAIN_RETURN_FAILURE;
        //if(write(LAIN_LOCAL_SEND_SOCKET, atom, strlen(atom)) < 0)
        if(send(LAIN_LOCAL_SEND_SOCKET, (void*)atom, (strlen(atom) + 1) * sizeof(char), 0) < 0)
            return LAIN_RETURN_FAILURE;
        return LAIN_RETURN_SUCCESS;
    }
    return LAIN_RETURN_FAILURE;
}
