
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


#ifndef MYLAIN_GLOBAL
#define MYLAIN_GLOBAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#ifndef LAIN_NO_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#else
char*       readline(char* prompt);
void        add_history(char* unused);
#endif

#ifndef LAIN_VERSION_EXTERN
#define LAIN_VERSION_MAJOR 0
#define LAIN_VERSION_MINOR 0
#define LAIN_VERSION_REV   1
#endif

#define LAIN_DEFAULT_MOTTO "Close this world. Open the next."

#define LAIN_RETURN_SUCCESS 0u
#define LAIN_RETURN_FAILURE 1u
#define LAIN_RETURN_ONGOING 2u
#define LAIN_RETURN_NOT_IMPLEMENTED 3u

extern unsigned           LAIN_LOCAL_RUNNING;
extern char*              LAIN_LOCAL_USER;
extern unsigned long long LAIN_MSTATE;
extern unsigned long long LAIN_SUBSTATE;
extern char*              LAIN_MOTTO;
extern clock_t            LAIN_START_TIME;


void lain_reset_all();
void lain_reset_sub();

void lain_init();
void lain_stop();

#endif
