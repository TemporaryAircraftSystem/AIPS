

include(FindProtobuf)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
        CACHE INTERNAL "Path to generated protobuf files.")
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS packets.proto)


set(COMMON_HEADER common.hpp settings.hpp packedmessage.h packets.h)
set(COMMON_SRC common.cpp settings.cpp packets.cpp)
set(COMMON_FILES ${COMMON_SRC} ${COMMON_HEADER})
add_library(aips-common ${COMMON_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(aips-common yaml-cpp ${PROTOBUF_LIBRARY})
target_include_directories(aips-common PUBLIC ${CMAKE_CURRENT_BINARY_DIR})