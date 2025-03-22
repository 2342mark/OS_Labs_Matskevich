cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(${CMAKE_SOURCE_DIR})


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(Creator Creator/creator.cpp)
add_executable(Main Main/main.cpp)
add_executable(Reporter Reporter/reporter.cpp)


enable_testing()
add_executable(tests tests/test_main.cpp tests/test_creator.cpp tests/test_reporter.cpp)

target_link_libraries(tests gtest gtest_main)

add_dependencies(tests Creator Reporter)

include(GoogleTest)
gtest_discover_tests(tests)
