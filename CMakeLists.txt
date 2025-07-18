cmake_minimum_required(VERSION 3.30)
project(Cache_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Cache_Simulator
    Cache.cpp
    Cache.h
    CacheLine.h
    main.cpp
        Memory_Access_Simulator.cpp
        Memory_Access_Simulator.h)
