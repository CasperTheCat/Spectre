cmake_minimum_required(VERSION 2.8)
project(Spectre)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -Wall -ggdb")

set(SOURCE_FILES
        main.cpp
        spectre.cpp
        util.cpp
        )

set(HEADER_FILES 
        spectre.h
        util.h
        )

include_directories(
)

link_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(Spectre ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(Spectre)
