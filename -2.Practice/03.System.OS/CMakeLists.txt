cmake_minimum_required(VERSION 3.2.1)
project(03.System.OS)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(HEADER_FILES if.h)
set(EXE_SOURCE_FILES import_if.c)
set(DLL_SOURCE_FILES export_if.c import_symload_if.c)



add_library(${PROJECT_NAME}_export_if SHARED ${DLL_SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME}_import_if ${EXE_SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME}_import_symload_if import_symload_if.c ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}_import_if ${PROJECT_NAME}_export_if)
