cmake_minimum_required(VERSION 3.2.1)
project(simple_server C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.c main_server.c main_server.h main_algorithm.c)

if (WIN32)
    include_directories(windows)
    set(OPCUA_SOURCES windows/open62541.c windows/open62541.h)
    set(OPCUA_LIBRARIES ws2_32 iphlpapi)
    target_link_libraries(${PROJECT_NAME} ws2_32 iphlpapi)
elseif (UNIX)
    include_directories(unix)
    set(OPCUA_SOURCES unix/open62541.c unix/open62541.h)
endif ()
add_library(opcua STATIC ${OPCUA_SOURCES})
target_link_libraries(opcua PUBLIC ${OPCUA_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
target_link_libraries(${PROJECT_NAME} opcua)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()