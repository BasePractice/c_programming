cmake_minimum_required(VERSION 3.2.1)
project(05.Memory)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(SIMPLE_MAIN simple_main.c)

add_executable(${PROJECT_NAME}_simple_main ${SIMPLE_MAIN})
if (UNIX)
    set_target_properties(${PROJECT_NAME}_simple_main PROPERTIES COMPILE_OPTIONS -Wattributes)
endif ()

if (WIN32)
    add_executable(${PROJECT_NAME}_map_file map_file.c)
    add_executable(${PROJECT_NAME}_process_memory process_memory.c)
endif ()
