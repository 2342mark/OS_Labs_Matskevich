cmake_minimum_required(VERSION 3.10)
project(ProcessProject)


set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0501)  
endif()

set(CREATOR_SOURCES Creator/Creator.cpp)
set(MAIN_SOURCES Main/Main.cpp)
set(REPORTER_SOURCES Reporter/Reporter.cpp)

add_executable(Creator ${CREATOR_SOURCES})

if(WIN32)
    set_target_properties(Creator PROPERTIES 
        SUFFIX ".exe"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  
    )
endif()


add_executable(Main ${MAIN_SOURCES})

if(WIN32)
    target_link_libraries(Main PRIVATE Kernel32.lib)  
    set_target_properties(Main PROPERTIES 
        SUFFIX ".exe"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()


add_executable(Reporter ${REPORTER_SOURCES})

if(WIN32)
    set_target_properties(Reporter PROPERTIES 
        SUFFIX ".exe"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  
    )
endif()
