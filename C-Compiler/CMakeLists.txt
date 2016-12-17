cmake_minimum_required(VERSION 3.6)
project(C_Compiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    cifa.cpp
    cifa.h
    global.h
    main.cpp
        )

add_executable(C_Compiler ${SOURCE_FILES})