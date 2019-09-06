cmake_minimum_required(VERSION 3.2.1)
project(10.Synchronized)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

if (IMPLEMENTED)

    if (UNIX)
        set(SYSTEM_LIBRARIES pthread)
    else ()
        set(SYSTEM_LIBRARIES ws2_32)
    endif ()
    set(MONITOR_SOURCES monitor_impl.c)
    set(MONITOR_EXECUTE_SOURCES monitor_execute.c)

    add_library(${PROJECT_NAME}_monitor_impl STATIC ${MONITOR_SOURCES})
    target_link_libraries(${PROJECT_NAME}_monitor_impl ${SYSTEM_LIBRARIES})
    add_executable(${PROJECT_NAME}_monitor_execute ${MONITOR_EXECUTE_SOURCES})
    target_link_libraries(${PROJECT_NAME}_monitor_execute ${PROJECT_NAME}_monitor_impl ${SYSTEM_LIBRARIES})
else ()
    add_library(${PROJECT_NAME}_monitor_impl STATIC .not_implement_yet.c)
endif ()
