cmake_minimum_required(VERSION 3.7)
project(Arquivos_do_lab1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        source/shell.c
        source/shell.h
        source/sim.c
        source/sim.h)

add_executable(Arquivos_do_lab1 ${SOURCE_FILES})