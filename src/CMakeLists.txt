
cmake_minimum_required(VERSION 3.8)
project(src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp main.h)
add_executable(src ${SOURCE_FILES})

target_link_libraries(src m simlib)