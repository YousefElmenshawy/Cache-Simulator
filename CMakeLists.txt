cmake_minimum_required(VERSION 3.16)
project(Cache-Simulator)  # You can change this name

set(CMAKE_CXX_STANDARD 17)

add_executable(Cache-Simulator main.cpp
        Cache.cpp
        Cache.h
        CacheLine.h)