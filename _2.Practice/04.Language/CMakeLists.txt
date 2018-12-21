cmake_minimum_required(VERSION 3.2.1)
project(04.Language)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(SIMPLE_MAIN basic_main.c)
set(ENVIRONMENT_MAIN environment_main.c)
set(FUNCTION_MAIN fun_main.c)

add_executable(${PROJECT_NAME}_basic_main ${SIMPLE_MAIN})
add_executable(${PROJECT_NAME}_environment_main ${ENVIRONMENT_MAIN})
add_executable(${PROJECT_NAME}_function_main ${FUNCTION_MAIN})
add_executable(${PROJECT_NAME}_binary_to_hex binary_to_hex.c)
