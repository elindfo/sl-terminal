cmake_minimum_required(VERSION 3.10)
project(SLTerminal)

# Version number
set (SL_VERSION_MAJOR 1)
set (SL_VERSION_MINOR 0)

set(SOURCE_FILES main.c platsuppslag.c platsuppslag.h realtidsinfo.c realtidsinfo.h SLResponse.c SLResponse.h gui.c gui.h curl.c curl.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GTK_CFLAGS} -Wall -Werror -g -std=99")

add_executable(SL ${SOURCE_FILES})

target_link_libraries(SL curl jansson pthread ncurses)
