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


#include "mylain/global.h"
#include "mylain/net.h"

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

void lain_init()
{
    // Reset all states
    lain_reset_all();
    LAIN_LOCAL_RUNNING = LAIN_RETURN_FAILURE;

    // Initial configuration loading/definition
    // Motto
    LAIN_MOTTO    = malloc(strlen(LAIN_DEFAULT_MOTTO) * sizeof(char));
    strcpy(LAIN_MOTTO, LAIN_DEFAULT_MOTTO);

    // Get and show username
    {
        const struct passwd* pass = getpwuid(getuid());
        if(pass != NULL) {
            LAIN_LOCAL_USER = malloc(strlen(pass->pw_name) * sizeof(char));
            strcpy(LAIN_LOCAL_USER, pass->pw_name);
        } else {
            LAIN_LOCAL_USER = malloc(strlen("user") * sizeof(char));
            strcpy(LAIN_LOCAL_USER, "user");
        }
    }
    printf("Hello, %s!\n", LAIN_LOCAL_USER);

    
    puts("Initializing core modules...");
    
    // Initialize networking
    printf("networking ............. "); fflush(stdout);
    assert(lain_net_setup() == LAIN_RETURN_SUCCESS);
    puts("OK");

    printf("timing     ............. "); fflush(stdout);
    LAIN_START_TIME = clock();
    puts("OK");
}

void lain_stop()
{
    // Dispose common stuff
    free(LAIN_MOTTO);
    free(LAIN_LOCAL_USER);

    // Dispose modules
    puts("Disposing core modules...");

    printf("networking ............. "); fflush(stdout);
    assert(lain_net_dispose() == LAIN_RETURN_SUCCESS);
    puts("OK");
}
