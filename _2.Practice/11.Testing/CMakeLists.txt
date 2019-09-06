cmake_minimum_required(VERSION 3.2.1)
project(11.Testing)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

if (APPLE)
elseif (UNIX)
    set(SYSTEM_LIBRARIES pthread)
else ()
    set(SYSTEM_LIBRARIES ws2_32)
endif ()

add_executable(${PROJECT_NAME}_monitor_tests monitor_tests.cpp)
target_link_libraries(${PROJECT_NAME}_monitor_tests 10.Synchronized_monitor_impl tests_main ${SYSTEM_LIBRARIES})


