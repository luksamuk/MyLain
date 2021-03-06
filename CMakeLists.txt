##################################################################
#  Copyright (c) 2017 Lucas Vieira <lucas.samuel2002@gmail.com>  #
#  This file is part of MyLain.                                  #
#                                                                #
#  MyLain is free software: you can redistribute it and/or       #
#  modify it under the terms of the GNU Lesser General Public    #
#  License as published by the Free Software Foundation, either  #
#  version 3 of the License, or (at your option) any later       #
#  version.                                                      #
#                                                                #
#  You should have received a copy of the GNU Lesser General     #
#  Public License along with MyLain. If not, see                 #
#  <http://www.gnu.org/licenses/>.                               #
##################################################################

cmake_minimum_required(VERSION 2.8)
project(MyLain)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(MYLAIN_SOURCES
  src/main.c
  src/global.c
  src/lexer.c
  src/common.c
  src/net.c
  src/repl.c)

set(MYLAIN_HEADERS
  src/mylain/global.h
  src/mylain/lexer.h
  src/mylain/common.h
  src/mylain/net.h
  src/mylain/repl.h)

add_executable(mylain ${MYLAIN_SOURCES} ${MYLAIN_HEADERS})
target_link_libraries(mylain m pthread)

IF(LAIN_NO_READLINE)
  add_definitions(-DLAIN_NO_READLINE)
ELSE()
  target_link_libraries(mylain readline)
ENDIF()
