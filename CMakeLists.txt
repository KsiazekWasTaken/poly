cmake_minimum_required(VERSION 3.5)
project(kalkulator)

set(CMAKE_C_STANDARD 11)
add_definitions(-g)
set(SOURCE_FILES main.c poly.c poly.h array.c array.h stack.c stack.h parse.c parse.h)
add_executable(kalkulator ${SOURCE_FILES})