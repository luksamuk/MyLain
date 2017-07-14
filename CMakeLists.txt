
#################################################################
#  Copyright (c) 2017 Lucas Vieira <lucas.samuel2002@gmail.com> #
#  This file is part of MyLain.                                 #
#                                                               #
#  OficinaFramework is free software: you can redistribute      #
#  it and/or modify it under the terms of the GNU Lesser        #
#  General Public License as published by the Free Software     #
#  Foundation, either version 3 of the License, or (at your     #
#  option) any later version.                                   #
#                                                               #
#  You should have received a copy of the GNU Lesser General    #
#  Public License along with OficinaFramework.  If not, see     #
#  <http://www.gnu.org/licenses/>.                              #
#################################################################

cmake_minimum_required(VERSION 2.8)
project(MyLain)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(MYLAIN_SOURCES
  src/main.c
  src/global.c
  src/lexer.c
  src/common.c
  src/net.c)

set(MYLAIN_HEADERS
  src/mylain_global.h
  src/mylain_lexer.h
  src/mylain_common.h
  src/mylain_net.h)

add_executable(mylain ${MYLAIN_SOURCES} ${MYLAIN_HEADERS})
target_link_libraries(mylain readline m pthread)
