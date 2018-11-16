cmake_minimum_required(VERSION 3.2.1)
project(10.Synchronized)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)


if (UNIX)
    set(SYSTEM_LIBRARIES pthread)
else ()
    set(SYSTEM_LIBRARIES ws2_32)
endif ()
set(MONITOR_SOURCES monitor_impl.c)

add_executable(${PROJECT_NAME}_monitor ${MONITOR_SOURCES})
target_link_libraries(${PROJECT_NAME}_monitor ${SYSTEM_LIBRARIES})
