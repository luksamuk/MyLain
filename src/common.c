
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
#include "mylain/common.h"
#include "mylain/lexer.h"
#include "mylain/net.h"


void lain_print_help(const char* literal)
{
    if(literal == NULL)
        printf("MyLain v%u.%u.%u\n"
               "The very useful, distributed personal assistant for smart homes\n"
               "This cheatsheet is temporary and may change.\n\n"

               "GENERAL COMMANDS\n"
               "help     -- [] Shows this prompt.\n"
               "            [command] Shows help for said command.\n"
               "quit     -- [] Exits MyLain.\n"
               "exit     -- [] Exits MyLain.\n"
               "connect  -- [ipaddr:port] Connects to server running\n"
               "            at the specified IP address and port.\n"
               "config   -- [get field] Prints the current\n"
               "            configuration for field.\n"
               "            [set field] Redefines the current\n"
               "            configuration for field.\n"
               "status   -- [] Shows general client status. If\n"
               "            client is connected to server, server\n"
               "            status is fetched as well.\n"
               "dispatch -- Dispatches queued subprocesses.\n"
               "printext -- Prints next sequence of atoms as a \n"
               "            continuous string.\n"
               "\n",
               LAIN_VERSION_MAJOR, LAIN_VERSION_MINOR, LAIN_VERSION_REV);
    else {
        // TODO: We should just obtain the command already instead of
        // checking, eh? We already have a function for that.
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
        else if(LAIN_CHECK_LITERAL(literal, "dispatch")) {
            printf("command: dispatch\n"
                   "Dispatches subprocesses which were queued by "
                   "remote requests.\n");
        }
        else if(LAIN_CHECK_LITERAL(literal, "printext")) {
            printf("command: printext [atoms]\n"
                   "Prints all atoms following the command.\n");
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
           "sendport   -- %u\n"
           "recvport   -- %u\n"
           "interface  -- %s\n",
           LAIN_MOTTO,
           LAIN_LOCAL_SEND_PORT,
           LAIN_LOCAL_RECV_PORT,
           LAIN_NET_INTERFACE);
}
