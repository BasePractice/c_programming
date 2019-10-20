cmake_minimum_required(VERSION 3.2.1)
project(tests CXX)

add_executable(${PROJECT_NAME} main.cpp catch.hpp c_default_server.cpp client_events.c ../open62541.c)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 iphlpapi)
endif ()
