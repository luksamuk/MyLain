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

#define LAIN_DEFAULT_PORT      6214
#define LAIN_DEFAULT_INTERFACE "eth0"

extern unsigned LAIN_LOCAL_PORT;
extern int      LAIN_LOCAL_SOCKET;
extern char*    LAIN_LOCAL_IP;
extern unsigned LAIN_NET_READY;
extern char*    LAIN_NET_INTERFACE;


unsigned lain_net_setup(void);
unsigned lain_net_connect(const char* address);
unsigned lain_net_dispose(void);


#endif
