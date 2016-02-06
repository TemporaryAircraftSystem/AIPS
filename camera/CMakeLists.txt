set(SOURCE_HEADERS camera.hpp camera_connector.hpp)
set(SOURCE_CPP main.cpp camera.cpp camera_connector.cpp)
set(SOURCE_FILES ${SOURCE_CPP} ${SOURCE_HEADERS})


find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS system regex chrono)
find_package(x264 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(aips-camera ${SOURCE_FILES})
target_link_libraries(aips-camera aips-common)
target_link_libraries(aips-camera ${OpenCV_LIBS})
target_link_libraries(aips-camera ${Boost_LIBRARIES})
target_link_libraries(aips-camera ${X264_LIBS})
