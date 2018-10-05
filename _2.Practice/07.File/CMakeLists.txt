cmake_minimum_required(VERSION 3.2.1)
project(07.File)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(SIMPLE_MAIN base_main.c)


add_executable(${PROJECT_NAME}_base_main ${SIMPLE_MAIN})
