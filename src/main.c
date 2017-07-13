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
#include "mylain_lexer.h"


int main(int argc, char** argv)
{
    LAIN_MSTATE   = 0ul;
    LAIN_SUBSTATE = 0ul;
    LAIN_MOTTO    = malloc(33 * sizeof(char));
    strcpy(LAIN_MOTTO, "Close this world. Open the next.");
    LAIN_LOCAL_RUNNING = LAIN_RETURN_FAILURE;
    {
        const struct passwd* pass = getpwuid(getuid());
        assert(pass != NULL);
        LAIN_LOCAL_USER  = pass->pw_name;
    }
    printf("MyLain v%u.%u.%u\n"
           "Hello, %s! Type \"quit\" to exit.\n",
           LAIN_VERSION_MAJOR,
           LAIN_VERSION_MINOR,
           LAIN_VERSION_REV,
           LAIN_LOCAL_USER);
    puts(LAIN_MOTTO);
    puts("");

    LAIN_START_TIME = clock();
    while(LAIN_LOCAL_RUNNING) {
        char* input = readline("lain > ");
        {
            unsigned i;
            for(i = 0; i < strlen(input); i++) {
                input[i] = tolower(input[i]);
            }
        }
        add_history(input);
        
        unsigned r = lain_eval(input);

        switch(r) {
        case LAIN_RETURN_SUCCESS:
            puts("Process returned success.");
            break;
        case LAIN_RETURN_ONGOING:
            puts("Error: Not enough arguments.");
            lain_reset_all();
            break;
        default:
            printf("Process returned error code 0x%08X\n"
                   "On command: \"%s\"\n", r, input);
            lain_reset_all();
            break;
        };
        
        free(input);
    }

    free(LAIN_MOTTO);

    puts("MyLain client halted. Downgrading to reality.\n");
    return 0;
}
