cmake_minimum_required(VERSION 2.8)
project(AIPS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(COMMON_HEADER common/common.hpp common/settings.hpp common/packets.hpp)
set(COMMON_SRC common/common.cpp common/settings.cpp common/packets.cpp)
set(COMMON_FILES ${COMMON_SRC} ${COMMON_HEADER})
include_directories(common)

add_library(aips-common ${COMMON_FILES})
target_link_libraries(aips-common yaml-cpp)

add_subdirectory(camera)

add_subdirectory(hub)

add_subdirectory(gui)

add_subdirectory(rc)
