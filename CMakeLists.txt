cmake_minimum_required(VERSION 3.16)
project(GarbageCollection)

set(CMAKE_CXX_STANDARD 14)
link_libraries(pthread)
add_definitions("-D POINTER_DEBUG") #DEBUG

add_executable(GarbageCollection main.cpp Object.cpp Object.h MemoryManager.cpp MemoryManager.h type_pointer.c MemoryBadAlloc.cpp MemoryBadAlloc.h)

