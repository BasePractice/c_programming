cmake_minimum_required(VERSION 3.2.1)
project(08.Socket)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(SIMPLE_MAIN simple_client.c)

if (WIN32)
    set(SYSTEM_LIBRARIES ws2_32)
endif ()

add_executable(${PROJECT_NAME}_simple_client ${SIMPLE_MAIN})
target_link_libraries(${PROJECT_NAME}_simple_client ${SYSTEM_LIBRARIES})
