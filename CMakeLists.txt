cmake_minimum_required(VERSION 3.2)
project(AIPS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_INCLUDE_PATH})
include_directories(common)
include_directories(${PROJECT_BINARY_DIR}/common)
add_subdirectory(common)
include_directories(${ProtobufIncludePath})
add_subdirectory(camera)

add_subdirectory(hub)

add_subdirectory(gui)

add_subdirectory(rc)
