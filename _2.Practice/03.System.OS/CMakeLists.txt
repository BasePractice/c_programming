cmake_minimum_required(VERSION 3.2.1)
project(03.System.OS)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(.)

set(HEADER_FILES if.h)
set(EXE_SOURCE_FILES import_if.c)
set(DLL_SOURCE_FILES export_if.c ../00.Base/base_macro.h)
add_definitions(-DDLL_EXPORT)

if (UNIX AND NOT APPLE)
    set(OS_LIBRARIES dl)
endif ()

add_library(${PROJECT_NAME}_export_if SHARED ${DLL_SOURCE_FILES} ${HEADER_FILES})
add_library(${PROJECT_NAME}_static_if STATIC ${DLL_SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME}_import_if ${EXE_SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME}_stlink_if ${EXE_SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME}_import_dlsym_if import_dlsym_if.c ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}_import_if ${PROJECT_NAME}_export_if)
target_link_libraries(${PROJECT_NAME}_stlink_if ${PROJECT_NAME}_static_if)
target_link_libraries(${PROJECT_NAME}_import_dlsym_if ${OS_LIBRARIES})
