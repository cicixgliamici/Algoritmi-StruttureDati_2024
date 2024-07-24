cmake_minimum_required(VERSION 3.28)
project(API C)

set(CMAKE_C_STANDARD 11)

add_executable(API main.c
        header.h
        test.c
        strutture_dati.c)
