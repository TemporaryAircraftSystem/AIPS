cmake_minimum_required(VERSION 3.3)
project(AIPS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(COMMON_HEADER common/common.hpp common/settings.hpp)
set(COMMON_SRC common/common.cpp common/settings.cpp)
set(COMMON_FILES ${COMMON_SRC} ${COMMON_HEADER})
include_directories(common)

add_library(aips-common ${COMMON_FILES})
target_link_libraries(aips-common yaml-cpp)
add_subdirectory(camera)

add_subdirectory(hub)

add_subdirectory(gui)

add_subdirectory(rc)