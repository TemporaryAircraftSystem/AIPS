set(SOURCE_HEADERS client_handler.hpp server.hpp)
set(SOURCE_CPP main.cpp client_handler.cpp server.cpp)
set(SOURCE_FILES ${SOURCE_CPP} ${SOURCE_HEADERS})

find_package(Boost REQUIRED COMPONENTS system regex)

add_executable(aips-hub ${SOURCE_FILES} )

target_link_libraries(aips-hub ${Boost_LIBRARIES})