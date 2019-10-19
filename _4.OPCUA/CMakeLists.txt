cmake_minimum_required(VERSION 3.2.1)
project(simple_server C)

add_executable(${PROJECT_NAME} main.c open62541.c open62541.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 iphlpapi)
endif()