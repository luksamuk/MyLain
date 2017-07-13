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

#ifndef MYLAIN_LEXER
#define MYLAIN_LEXER

#define LAIN_CHECK_LITERAL(x,y) (!strcmp(x,y))
#define LAIN_CHKSTATE(x) (LAIN_MSTATE & x)
#define LAIN_CHKSUB(x) (LAIN_SUBSTATE & x)

enum LAIN_COMMAND
{
    LAIN_COM_ATOM    = 0u,
    LAIN_COM_QUIT    = 1u,
    LAIN_COM_CONNECT = 2u,
    LAIN_COM_CONFIG  = 4u,
    LAIN_COM_HELP    = 8u,
    LAIN_COM_END     = 16u,
    LAIN_COM_STATUS  = 32u,
};

enum LAIN_SUBCOMMAND_CFG
{
    LAIN_SUBCOM_GETCFG = 1u,
    LAIN_SUBCOM_SETCFG = 2u
};

enum LAIN_COMMAND lain_get_command(const char* literal);
unsigned          lain_dispatch(const char* literal, enum LAIN_COMMAND command);
unsigned          lain_eval(char* command);

#endif
