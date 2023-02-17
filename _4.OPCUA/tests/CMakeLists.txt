cmake_minimum_required(VERSION 3.2.1)
project(tests CXX)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(${PROJECT_NAME} main.cpp c_default_server.cpp client_events.c ../main_server.c ../main_algorithm.c default_server.cpp default_server.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 iphlpapi)
elseif (UNIX)
    if (NOT APPLE)
        target_link_libraries(${PROJECT_NAME} pthread)
    else()
        include_directories(/usr/local/include)
        target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib/)
    endif()
endif ()
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 open62541)