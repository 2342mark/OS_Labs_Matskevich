cmake_minimum_required(VERSION 3.10)
project(marker_sync)

set(CMAKE_CXX_STANDARD 98)

add_executable(marker_sync main.cpp)

# Windows specific threading library (optional)
if(WIN32)
    target_link_libraries(marker_sync PRIVATE synchronization)
endif()