# cmake_minimum_required(VERSION <specify CMake version here>)
project(libgump)

set(CMAKE_CXX_STANDARD 11)

add_executable(libgump
        Coordinate.cpp
        Coordinate.h
        Environment.cpp
        Environment.h
        Main.cpp)
