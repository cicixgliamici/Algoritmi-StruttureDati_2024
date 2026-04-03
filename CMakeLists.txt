cmake_minimum_required(VERSION 3.22.1)
project(BakeryManagementSystem C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(bakery
    main.c
    strutture_dati.c
    ordini.c
)

add_executable(test_runner
    test.c
    strutture_dati.c
)

target_include_directories(bakery PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
