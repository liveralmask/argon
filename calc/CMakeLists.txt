cmake_minimum_required(VERSION 2.8)

include(${CMAKE_CURRENT_LIST_DIR}/../argon.cmake)

add_definitions(-g -Wall)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
add_executable(calc calc.cpp ${ARGON_SRCS})
