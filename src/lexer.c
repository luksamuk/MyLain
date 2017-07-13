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


#include "mylain_lexer.h"
#include "mylain_global.h"

enum LAIN_COMMAND lain_get_command(const char* literal)
{
    if(LAIN_CHECK_LITERAL(literal, "exit")
       || LAIN_CHECK_LITERAL(literal, "quit"))
        return LAIN_COM_QUIT;
    else if(LAIN_CHECK_LITERAL(literal, "connect"))
        return LAIN_COM_CONNECT;
    else if(LAIN_CHECK_LITERAL(literal, "config"))
        return LAIN_COM_CONFIG;
    else if(LAIN_CHECK_LITERAL(literal, "help"))
        return LAIN_COM_HELP;
    else if(LAIN_CHECK_LITERAL(literal, "status"))
        return LAIN_COM_STATUS;
    return LAIN_COM_ATOM;
}

// TODO: unbloat this function
unsigned lain_dispatch(const char* literal, enum LAIN_COMMAND comm)
{
    if(!LAIN_MSTATE)
        return LAIN_RETURN_SUCCESS;

    // Connect to remote server
    if(LAIN_CHKSTATE(LAIN_COM_CONNECT)
       && comm == LAIN_COM_ATOM) {
        printf("I'm gonna try to connect to a third-party"
               "server now. If your connection is good, "
               "this will most likely speed up processing"
               "some features you may request.\n");
        printf("Connecting to server at %s...\n", literal);
        printf("Fake connection estabilished.\n");
        lain_reset_all();
        return LAIN_RETURN_SUCCESS;
    }
    
    // Get/set config
    else if(LAIN_CHKSTATE(LAIN_COM_CONFIG)
            && comm == LAIN_COM_ATOM) {
        if(LAIN_CHECK_LITERAL(literal, "set")) {
            assert(LAIN_SUBSTATE == 0ul);
            LAIN_SUBSTATE |= LAIN_SUBCOM_SETCFG;
        } else if(LAIN_CHECK_LITERAL(literal, "get")) {
            assert(LAIN_SUBSTATE == 0ul);
            LAIN_SUBSTATE |= LAIN_SUBCOM_GETCFG;
        }
        else {
            if(LAIN_SUBSTATE == 0ul) {
                printf("Config: get/set not specified\n");
                lain_reset_all();
                return LAIN_RETURN_FAILURE;
            }

            else if(LAIN_CHECK_LITERAL(literal, "motto")) {
                if(LAIN_CHKSUB(LAIN_SUBCOM_GETCFG)) {
                    printf("\"%s\"\n", LAIN_MOTTO);
                    lain_reset_all();
                    return LAIN_RETURN_SUCCESS;
                } else if(LAIN_CHKSUB(LAIN_SUBCOM_SETCFG)) {
                    free(LAIN_MOTTO);
                    LAIN_MOTTO = readline("Input new motto: ");
                    lain_reset_all();
                    return LAIN_RETURN_SUCCESS;
                }
            }
            else {
                lain_reset_all();
                return LAIN_RETURN_FAILURE;
            }
        }
    }

    // Status prompt
    else if(LAIN_CHKSTATE(LAIN_COM_STATUS))  {
        if(comm == LAIN_COM_END) {
            // Display general status
            {
                clock_t end = clock();
                double  uptime_total = ((double)(end - LAIN_START_TIME) / CLOCKS_PER_SEC);
                double  uptime_days, uptime_hours, uptime_minutes;
                modf((uptime_total / 86400.0), &uptime_days);
                uptime_total -= uptime_days * 86400.0;
                modf((uptime_total / 3600.0), &uptime_hours);
                uptime_total -= uptime_hours * 3600.0;
                modf((uptime_total / 60.0), &uptime_minutes);
                uptime_total -= uptime_minutes * 60.0;
                printf("Uptime: %0.0lfd %0.0lfh %02.0lfm %02.0lfs\n",
                       uptime_days, uptime_hours, uptime_minutes, uptime_total);
            }
            puts("TODO: Add more monitoring features.");
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
        }
        else if(comm == LAIN_COM_ATOM) {
            printf("Sorry, I cannot give specific statuses for now.\n");
            lain_reset_all();
            return LAIN_RETURN_NOT_IMPLEMENTED;
        }
    }
    
    // Help prompt
    else if(LAIN_CHKSTATE(LAIN_COM_HELP)) {
        if(comm == LAIN_COM_END) {
            printf("MyLain v0.0.1\n"
                   "The very useful, distributed personal assistant "
                   "for smart homes\n"
                   "This cheatsheet is temporary and may change.\n\n"
                   "GENERAL COMMANDS\n"
                   " help    -- [] Shows this prompt.\n"
                   "            [command] Shows help for said command.\n"
                   " quit    -- [] Exits MyLain.\n"
                   " exit    -- [] Exits MyLain.\n"
                   " connect -- [ipaddr:port] Connects to server running\n"
                   "            at the specified IP address and port.\n"
                   " config  -- [get field] Prints the current\n"
                   "            configuration for field.\n"
                   "            [set field] Redefines the current\n"
                   "            configuration for field.\n"
                   " status  -- [] Shows general client status. If\n"
                   "            client is connected to server, server\n"
                   "            status is fetched as well.\n"
                   "\n"
                   "CONFIGURATION FIELDS\n"
                   " motto   -- MyLain's motto, shown at every startup\n"
                   "            prompt.\n"
                   "\n");
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
        }
        else if(comm == LAIN_COM_ATOM) {
            printf("Sorry, cannot obtain command help for now...\n");
            lain_reset_all();
            return LAIN_RETURN_NOT_IMPLEMENTED;
        }
    }
    
    return LAIN_RETURN_ONGOING;
}

unsigned lain_eval(char* command)
{
    char* cr_literal = strtok(command, " ");
    unsigned cr_returnval = LAIN_RETURN_FAILURE;
    while(cr_literal != NULL) {
        enum LAIN_COMMAND comm = lain_get_command(cr_literal);
        
        switch(comm) {
        case LAIN_COM_QUIT:
            LAIN_LOCAL_RUNNING = 0u;
            cr_returnval = LAIN_RETURN_SUCCESS;
            break;
            
        case LAIN_COM_ATOM:
            if(!LAIN_MSTATE) {
                printf("atom{%s}\n", cr_literal);
            }
            break;

        default:
            LAIN_MSTATE |= comm;
            break;
        };

        cr_returnval = lain_dispatch(cr_literal, comm);
        cr_literal = strtok(NULL, " ");
    }
    cr_returnval = lain_dispatch(NULL, LAIN_COM_END);
    return cr_returnval;
}
