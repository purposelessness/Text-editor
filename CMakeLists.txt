cmake_minimum_required(VERSION 3.20)
project(Coursework C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize-recover=address -O1 -fno-omit-frame-pointer -g")

add_executable(Coursework main.c iodata.c utility.c handler.c handler.h ioutility.c ioutility.h memutility.c memutility.h datautility.c datautility.h colorutility.c colorutility.h hashtable.c hashtable.h rmnumsutility.c rmnumsutility.h)
