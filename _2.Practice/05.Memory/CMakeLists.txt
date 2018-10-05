cmake_minimum_required(VERSION 3.2.1)
project(05.Memory)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

if (NOT UNIX)
    set(SIMPLE_MAIN simple_main.c)
else()
    set(SIMPLE_MAIN empty.c)
endif ()

if ((UNIX AND NOT APPLE) OR WIN32)
    add_executable(${PROJECT_NAME}_simple_main ${SIMPLE_MAIN})
    set_target_properties(${PROJECT_NAME}_simple_main PROPERTIES COMPILE_OPTIONS -Wattributes)
endif ()

if (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME}_simple_main PROPERTIES COMPILE_OPTIONS -Wattributes)
endif ()

if (WIN32)
    add_executable(${PROJECT_NAME}_map_file map_file.c)
    add_executable(${PROJECT_NAME}_process_memory process_memory.c)
endif ()
