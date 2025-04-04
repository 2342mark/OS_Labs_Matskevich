cmake_minimum_required(VERSION 3.10)
project(marker_sync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(marker_logic marker_logic.cpp)

add_executable(app main.cpp)
target_link_libraries(app marker_logic)

add_executable(tests tests.cpp)
target_link_libraries(tests gtest gtest_main marker_logic)

include(GoogleTest)
gtest_discover_tests(tests)