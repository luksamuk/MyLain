/******************************************************************
 *  Copyright (c) 2017 Lucas Vieira <lucas.samuel2002@gmail.com>  *
 *  This file is part of MyLain.                                  *
 *                                                                *
 *  MyLain is free software: you can redistribute it and/or       *
 *  modify it under the terms of the GNU Lesser General Public    *
 *  License as published by the Free Software Foundation, either  *
 *  version 3 of the License, or (at your option) any later       *
 *  version.                                                      *
 *                                                                *
 *  You should have received a copy of the GNU Lesser General     *
 *  Public License along with MyLain. If not, see                 *
 *  <http://www.gnu.org/licenses/>.                               *
 ******************************************************************/


#include "mylain/global.h"
#include "mylain/net.h"
#include "mylain/repl.h"


int main(int argc, char** argv)
{ 
    printf("MyLain v%u.%u.%u\n",
           LAIN_VERSION_MAJOR,
           LAIN_VERSION_MINOR,
           LAIN_VERSION_REV);

    // Initialize modules
    lain_init();
    
    // Run REPL
    lain_repl();

    // Dispose all
    lain_stop();

    puts("MyLain client halted. Downgrading to reality.");
    return 0;
}
