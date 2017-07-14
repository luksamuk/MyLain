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
#include "mylain_common.h"
#include "mylain_lexer.h"
#include "mylain_net.h"


void lain_print_help(const char* literal)
{
    if(literal == NULL)
        printf("MyLain v%u.%u.%u\n"
               "The very useful, distributed personal assistant for smart homes\n"
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
               "            status is fetched as well.\n\n",
               LAIN_VERSION_MAJOR, LAIN_VERSION_MINOR, LAIN_VERSION_REV);
    else {
        if(LAIN_CHECK_LITERAL(literal, "help"))
            printf("command: help [command]\n"
                   "Shows help prompt for command. If no command was "
                   "specified, then shows general help cheatsheet.\n");
        else if(LAIN_CHECK_LITERAL(literal, "exit")
                || LAIN_CHECK_LITERAL(literal, "quit"))
            printf("command: exit\n"
                   "alias:   quit\n"
                   "Safely halts MyLain.\n");
        else if(LAIN_CHECK_LITERAL(literal, "connect"))
            printf("command: connect [ip:port]\n"
                   "Connects to a MyLain server on the given IP address "
                   "and port. If omitted, will connect to the default server, "
                   "given that it has already been configured.\n");
        else if(LAIN_CHECK_LITERAL(literal, "config"))
            printf("command: config [{get/set} field]\n"
                   "Retrieves or changes a configuration field. "
                   "Omit arguments to see all configuration fields.\n");
        else if(LAIN_CHECK_LITERAL(literal, "status")) {
            printf("command: status\n"
                   "Shows status of this MyLain client, and shows "
                   "status of the server and/or the Lain network, if "
                   "connected.\n");
        }
        else {
            printf("Sorry, no help for this command right now...\n");
        }
    }
}

void lain_print_configfields(void)
{
    printf("GENERAL CONFIGURATION\n"
           "motto      -- \"%s\"\n"
           "localport  -- %u\n"
           "interface  -- %s\n"
           "\n",
           LAIN_MOTTO,
           LAIN_LOCAL_PORT,
           LAIN_NET_INTERFACE);
}