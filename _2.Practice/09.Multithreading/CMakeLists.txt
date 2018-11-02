cmake_minimum_required(VERSION 3.2.1)
project(09.Multithreading)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)


if (UNIX)
    set(SYSTEM_LIBRARIES pthread)
    set(SIMPLE_MAIN simple_thread.c)
    set(MT_SERVER mt_unix_server.c)
else ()
    set(SYSTEM_LIBRARIES ws2_32)
    message(ERROR "For windows not implement yet")
endif ()

add_executable(${PROJECT_NAME}_simple ${SIMPLE_MAIN})
add_executable(${PROJECT_NAME}_mt_server ${MT_SERVER})
target_link_libraries(${PROJECT_NAME}_simple ${SYSTEM_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_mt_server ${SYSTEM_LIBRARIES})


if (UNIX)
    add_executable(${PROJECT_NAME}_fork_bomb fork_bomb.c)
endif ()
