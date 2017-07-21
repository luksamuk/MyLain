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


#include "mylain_global.h"

#ifdef LAIN_NO_READLINE
static char INPUT_BUFFER[2048];

char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(INPUT_BUFFER, 2048, stdin);
    char* cpy = malloc(strlen(INPUT_BUFFER) + 1);
    strcpy(cpy, INPUT_BUFFER);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

void add_history(char* unused) {}
#endif

unsigned           LAIN_LOCAL_RUNNING;
char*              LAIN_LOCAL_USER;
unsigned long long LAIN_MSTATE;
unsigned long long LAIN_SUBSTATE;
char*              LAIN_MOTTO;
clock_t            LAIN_START_TIME;


void lain_reset_all() {
    LAIN_SUBSTATE = 0ul;
    LAIN_MSTATE   = 0ul;
}

void lain_reset_sub() {
    LAIN_SUBSTATE = 0ul;
}
