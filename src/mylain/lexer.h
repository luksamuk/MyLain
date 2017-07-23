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

#ifndef MYLAIN_LEXER
#define MYLAIN_LEXER

#define LAIN_CHECK_LITERAL(x,y) (!strcmp(x,y))
#define LAIN_CHKSTATE(x) (LAIN_MSTATE & x)
#define LAIN_CHKSUB(x) (LAIN_SUBSTATE & x)

enum LAIN_COMMAND
{
    LAIN_COM_ATOM      = 0llu,
    LAIN_COM_QUIT      = 1llu,
    LAIN_COM_CONNECT   = 2llu,
    LAIN_COM_CONFIG    = 4llu,
    LAIN_COM_HELP      = 8llu,
    LAIN_COM_END       = 16llu,
    LAIN_COM_STATUS    = 32llu,
    LAIN_COM_QDISP     = 64llu,
    LAIN_COM_PRINTEXT  = 128llu,
    LAIN_COM_DISPATCH  = 256llu
};

enum LAIN_SUBCOMMAND_CFG
{
    LAIN_SUBCOM_GETCFG = 1llu,
    LAIN_SUBCOM_SETCFG = 2llu
};

typedef struct LAIN_COM_QUEUE_INFO
{
    unsigned long long com;
    char*              atom;
} lain_com_queue_info;

typedef struct LAIN_COM_QUEUE_NODE
{
    unsigned long long          com;
    char*                       atom;
    struct LAIN_COM_QUEUE_NODE* next;
} lain_com_queue_node;

typedef struct LAIN_COM_QUEUE
{
    lain_com_queue_node* first;
    lain_com_queue_node* last;
    unsigned long long   amount;
} lain_com_queue_t;

extern lain_com_queue_t* LAIN_REMOTE_COM_QUEUE;


enum LAIN_COMMAND   lain_get_command_unchecked(const char* literal);
enum LAIN_COMMAND   lain_get_command(const char* literal);
const char*         lain_command_name(enum LAIN_COMMAND com);
unsigned            lain_dispatch(const char* literal, enum LAIN_COMMAND command);


void                lain_com_enqueue(lain_com_queue_t* queue, unsigned long long com);
void                lain_com_enqueue_atom(lain_com_queue_t* queue, char* atom);
lain_com_queue_info lain_com_dequeue(lain_com_queue_t* queue);
void                lain_com_queue_clear(lain_com_queue_t* queue);

#endif
