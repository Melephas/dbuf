cmake_minimum_required(VERSION 3.13)
project(dbuf C)

set(CMAKE_C_STANDARD 11)

add_library(
        dbuf SHARED
        src/libdbuf.c
)

target_include_directories(
        dbuf
        PUBLIC include
)

install(
    TARGETS dbuf
    LIBRARY  DESTINATION lib
    INCLUDES DESTINATION include
)

install(
    FILES "include/libdbuf.h"
    DESTINATION include
)
