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


#include "mylain/lexer.h"
#include "mylain/global.h"
#include "mylain/common.h"
#include "mylain/net.h"

enum LAIN_COMMAND lain_get_command_unchecked(const char* literal)
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
    else if(LAIN_CHECK_LITERAL(literal, "qdisp"))
        return LAIN_COM_QDISP;
    else if(LAIN_CHECK_LITERAL(literal, "printext"))
        return LAIN_COM_PRINTEXT;
    else if(LAIN_CHECK_LITERAL(literal, "dispatch"))
        return LAIN_COM_DISPATCH;
    
    return LAIN_COM_ATOM;
}

enum LAIN_COMMAND lain_get_command(const char* literal)
{
    if(LAIN_MSTATE != 0ul)
        return LAIN_COM_ATOM;
    
    return lain_get_command_unchecked(literal);
}

const char* lain_command_name(enum LAIN_COMMAND com)
{
    switch(com) {
    case LAIN_COM_ATOM:      return "ATOM";
    case LAIN_COM_QUIT:      return "QUIT";
    case LAIN_COM_CONNECT:   return "CONNECT";
    case LAIN_COM_CONFIG:    return "CONFIG";
    case LAIN_COM_HELP:      return "HELP";
    case LAIN_COM_END:       return "COMMAND_END";
    case LAIN_COM_STATUS:    return "STATUS";
    case LAIN_COM_QDISP:     return "QUEUE_DISPATCH";
    case LAIN_COM_PRINTEXT:  return "PRINTEXT";
    case LAIN_COM_DISPATCH:  return "REMOTE_DISPATCH";
    };
    return "UNKNOWN";
}

// TODO: unbloat this function
unsigned lain_dispatch(const char* literal, enum LAIN_COMMAND comm)
{
    if(!LAIN_MSTATE)
        return LAIN_RETURN_SUCCESS;

    // Connect to remote server
    if(LAIN_CHKSTATE(LAIN_COM_CONNECT)) {
        // Disabling this for now.
        /*if(LAIN_NET_READY) {
            puts("Cannot connect: already connected. Disconnect first!");
            lain_reset_all();
            return LAIN_RETURN_FAILURE;
        }*/
        if(comm == LAIN_COM_ATOM) {
            /*printf("I'm gonna try to connect to a third-party "
              "server now. If your connection is good, "
              "this will most likely speed up processing "
              "some features you may request.\n");*/
            printf("Connecting to node at %s...\n", literal);
            lain_reset_all();
            return lain_net_connect(literal);
        } else if(comm == LAIN_COM_END) {
            // TODO: Connect to default node
            lain_reset_all();
            return lain_net_connect(NULL);
        }
    }
       
    
    // Get/set config
    else if(LAIN_CHKSTATE(LAIN_COM_CONFIG)) {
        if(comm == LAIN_COM_ATOM) {
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

                // Get/set Motto
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

                // Get/set local ports
                else if(LAIN_CHECK_LITERAL(literal, "sendport")) {
                    if(LAIN_CHKSUB(LAIN_SUBCOM_GETCFG)) {
                        printf("%u\n", LAIN_LOCAL_SEND_PORT);
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    } else if(LAIN_CHKSUB(LAIN_SUBCOM_SETCFG)) {
                        if(LAIN_NET_READY) {
                            printf("Cannot change port number while connected.\n");
                            return LAIN_RETURN_FAILURE;
                        }
                        char* newports = readline("Input new port number: ");
                        unsigned newport = strtoul(newports, NULL, 10);
                        free(newports);
                        printf("Changing sending port to %u...\n", newport);
                        LAIN_LOCAL_SEND_PORT = newport;
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    }
                }
                else if(LAIN_CHECK_LITERAL(literal, "recvport")) {
                    if(LAIN_CHKSUB(LAIN_SUBCOM_GETCFG)) {
                        printf("%u\n", LAIN_LOCAL_RECV_PORT);
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    } else if(LAIN_CHKSUB(LAIN_SUBCOM_SETCFG)) {
                        if(LAIN_NET_READY) {
                            printf("Cannot change port number while connected.\n");
                            return LAIN_RETURN_FAILURE;
                        }
                        char* newports = readline("Input new port number: ");
                        unsigned newport = strtoul(newports, NULL, 10);
                        free(newports);
                        printf("Changing receive port to %u...\n", newport);
                        LAIN_LOCAL_RECV_PORT = newport;
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    }
                }

                // Get/set interface
                else if(LAIN_CHECK_LITERAL(literal, "interface")) {
                    if(LAIN_CHKSUB(LAIN_SUBCOM_GETCFG)) {
                        printf("%s\n", LAIN_NET_INTERFACE);
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    } else if(LAIN_CHKSUB(LAIN_SUBCOM_SETCFG)) {
                        if(LAIN_NET_READY) {
                            printf("Cannot change interface while connected.\n");
                            return LAIN_RETURN_FAILURE;
                        }
                        LAIN_NET_INTERFACE = readline("Input new interface name: ");
                        printf("Requesting IPv4 address to interface %s...\n", LAIN_NET_INTERFACE);
                        lain_net_fetch_ip();
                        lain_reset_all();
                        return LAIN_RETURN_SUCCESS;
                    }
                }
                
                else {
                    lain_reset_all();
                    return LAIN_RETURN_FAILURE;
                }
            }
        } else if(comm == LAIN_COM_END && LAIN_SUBSTATE == 0ul) {
            lain_print_configfields();
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
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
                printf("Uptime: %0.0lfd %0.0lfh %02.0lfm %02.0lfs\n"
                       "Local address: %s\n"
                       "Connected: %s\n",
                       uptime_days, uptime_hours, uptime_minutes, uptime_total,
                       LAIN_LOCAL_IP,
                       (LAIN_NET_READY) ? "YES" : "NO");
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
            lain_print_help(NULL);
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
        }
        else if(comm == LAIN_COM_ATOM) {
            lain_print_help(literal);
            lain_reset_all();
            return LAIN_RETURN_NOT_IMPLEMENTED;
        }
    }

    // Dispatch queue
    else if(LAIN_CHKSTATE(LAIN_COM_QDISP)) {
        lain_reset_all();
        unsigned sub_ret_val = LAIN_RETURN_ONGOING;
        // Dispatch queued commands
        sem_wait(&LAIN_NET_LISTENER_SEMAPHORE);
        while(LAIN_REMOTE_COM_QUEUE->first != NULL) {
            lain_com_queue_info info = lain_com_dequeue(LAIN_REMOTE_COM_QUEUE);
            if(info.com == LAIN_COM_ATOM)
                printf("!lain_qdisp{ATOM:%s}\n", info.atom);
            else printf("!lain_qdisp{%s}\n", lain_command_name(info.com));
            
            if(!((info.com == LAIN_COM_END) && !sub_ret_val)) {
                LAIN_MSTATE |= info.com;
                sub_ret_val = lain_dispatch(info.atom, info.com);
            }
            if(info.atom) free(info.atom);
        }
        
        if(sub_ret_val == LAIN_RETURN_ONGOING)
                sub_ret_val = lain_dispatch(NULL, LAIN_COM_END);
        
        if(sub_ret_val == LAIN_RETURN_SUCCESS)
            puts("!sub_ans{Ok}");
        else printf("!ans{Err:0x%08X}\n", sub_ret_val);
        
        sem_post(&LAIN_NET_LISTENER_SEMAPHORE);
        
        return LAIN_RETURN_SUCCESS;
    }

    // Print text
    else if(LAIN_CHKSTATE(LAIN_COM_PRINTEXT)) {
        if(comm == LAIN_COM_ATOM) {
            printf("%s ", literal);
        } else if(comm == LAIN_COM_END) {
            puts("");
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
        }
    }

    // Dispatch remotely
    else if(LAIN_CHKSTATE(LAIN_COM_DISPATCH)) {
        // Check for connection
        if(!LAIN_NET_READY) {
            puts("Cannot dispatch remote commands while disconnected");
            lain_reset_all();
            return LAIN_RETURN_FAILURE;
        }
        
        if(comm == LAIN_COM_ATOM) {
            // First command needs to be checked
            if(!LAIN_SUBSTATE) {
                enum LAIN_COMMAND possible_com = lain_get_command_unchecked(literal);
                if(possible_com == LAIN_COM_ATOM) {
                    lain_net_dispatch_atom(literal);
                } else lain_net_dispatch_com(possible_com);
                LAIN_SUBSTATE = 1llu;
            } else {
                // Next commands are always just atoms
                lain_net_dispatch_atom(literal);
            }
        } else if(comm == LAIN_COM_END) {
            // Dispatch command end
            lain_net_dispatch_com(LAIN_COM_END);
            lain_reset_all();
            return LAIN_RETURN_SUCCESS;
        }
    }
    
    return LAIN_RETURN_ONGOING;
}






void lain_com_enqueue(lain_com_queue_t* queue, unsigned long long com)
{
    if(!queue) return;

    if(!queue->first) {
        queue->first = malloc(sizeof(lain_com_queue_node));
        queue->first->next = NULL;
        queue->first->com = com;
        queue->first->atom = NULL;
        queue->last = queue->first;
        queue->amount = 1ul;
        return;
    }

    lain_com_queue_node* aux = malloc(sizeof(lain_com_queue_node));
    aux->com = com;
    aux->atom = NULL;
    aux->next = NULL;
    queue->last->next = aux;
    queue->last = aux;
    queue->amount++;
}

void lain_com_enqueue_atom(lain_com_queue_t* queue, char* atom)
{
    if(!queue) return;

    if(!queue->first) {
        queue->first = malloc(sizeof(lain_com_queue_node));
        queue->first->next = NULL;
        queue->first->com = LAIN_COM_ATOM;
        queue->first->atom = atom;
        queue->last = queue->first;
        queue->amount = 1ul;
        return;
    }

    lain_com_queue_node* aux = malloc(sizeof(lain_com_queue_node));
    aux->com = LAIN_COM_ATOM;
    aux->atom = atom;
    aux->next = NULL;
    queue->last->next = aux;
    queue->last = aux;
    queue->amount++;
}

lain_com_queue_info lain_com_dequeue(lain_com_queue_t* queue)
{
    lain_com_queue_info info;
    info.com = LAIN_COM_END;
    info.atom = NULL;
    
    if(!queue || !queue->first)
        return info;

    info.com = queue->first->com;
    info.atom = queue->first->atom;
    lain_com_queue_node* next = queue->first->next;
    free(queue->first);
    queue->first = next;
    queue->amount--;
    return info;
}

void lain_com_queue_clear(lain_com_queue_t* queue)
{
    while(queue->first != NULL)
        lain_com_dequeue(queue);
    queue->amount = 0ul;
}
