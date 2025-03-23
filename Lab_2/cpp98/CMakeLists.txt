cmake_minimum_required(VERSION 3.10)
project(ThreadApp)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(ThreadApp main.cpp)