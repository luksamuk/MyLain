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
 *  Public License along with OficinaFramework.  If not, see      *
 *  <http://www.gnu.org/licenses/>.                               *
 ******************************************************************/

#include "mylain_net.h"
#include "mylain_global.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

unsigned LAIN_LOCAL_PORT;
int      LAIN_LOCAL_SOCKET;
char*    LAIN_LOCAL_IP;
unsigned LAIN_NET_READY;
char*    LAIN_NET_INTERFACE;


unsigned lain_net_setup(void)
{
    LAIN_LOCAL_PORT = LAIN_DEFAULT_PORT;
    LAIN_NET_READY = LAIN_RETURN_FAILURE;
    LAIN_LOCAL_IP = malloc(9 * sizeof(char));
    strcpy(LAIN_LOCAL_IP, "<unknown>");
    LAIN_NET_INTERFACE = malloc(strlen(LAIN_DEFAULT_INTERFACE) * sizeof(char));
    strcpy(LAIN_NET_INTERFACE, LAIN_DEFAULT_INTERFACE);
    LAIN_LOCAL_SOCKET = 0;
    
    return LAIN_RETURN_SUCCESS;
}

unsigned lain_net_connect(const char* address)
{
    // Open socket
    printf("Opening socket...\n");
    LAIN_LOCAL_SOCKET = socket(AF_INET, SOCK_DGRAM, 0);

    struct ifreq ifr;
    printf("Requesting IPv4 address to interface %s...\n", LAIN_NET_INTERFACE);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, LAIN_NET_INTERFACE, IFNAMSIZ-1);
    ioctl(LAIN_LOCAL_SOCKET, SIOCGIFADDR, &ifr);

    // Fetch local IP
    free(LAIN_LOCAL_IP);
    LAIN_LOCAL_IP = malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr, LAIN_LOCAL_IP, INET_ADDRSTRLEN);

    printf("Sorry, this is an incomplete feature.\n");
    
    return LAIN_RETURN_FAILURE;
}

unsigned lain_net_dispose(void)
{
    if(LAIN_LOCAL_SOCKET) {
        printf("Closing socket...\n");
        close(LAIN_LOCAL_SOCKET);
    }
    return LAIN_RETURN_SUCCESS;
}
