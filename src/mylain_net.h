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


#ifndef MYLAIN_NET
#define MYLAIN_NET

#include <pthread.h>
#include <semaphore.h>
#include "mylain_global.h"

#define LAIN_DEFAULT_PORT      6214
#define LAIN_DEFAULT_INTERFACE "eth0"

extern unsigned  LAIN_LOCAL_SEND_PORT;
extern int       LAIN_LOCAL_SEND_SOCKET;
extern char*     LAIN_LOCAL_IP;
extern unsigned  LAIN_NET_READY;
extern char*     LAIN_NET_INTERFACE;
extern pthread_t LAIN_NET_LISTENER_THREAD;
extern sem_t     LAIN_NET_LISTENER_SEMAPHORE;


typedef struct LAIN_REMOTE_COM
{
    unsigned long long MSTATE;
    unsigned long long SUBSTATE;
} lain_remote_t;


unsigned lain_net_setup(void);
unsigned lain_net_connect(const char* address);
unsigned lain_net_dispose(void);
void*    lain_net_listener_loop(void* unused);

#endif
