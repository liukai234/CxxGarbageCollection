cmake_minimum_required(VERSION 3.17)
project(GarbageCollection)

set(CMAKE_CXX_STANDARD 14)

#add_definitions("-D DEBUG")

add_executable(GarbageCollection main.cpp Object.cpp Object.h MemoryManager.cpp MemoryManager.h type_pointer.c)

