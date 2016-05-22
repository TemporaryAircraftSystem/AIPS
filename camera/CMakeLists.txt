set(SOURCE_HEADERS camera.hpp camera_connector.hpp)
set(SOURCE_CPP main.cpp camera.cpp camera_connector.cpp)
set(SOURCE_FILES ${SOURCE_CPP} ${SOURCE_HEADERS})


find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS system regex chrono)
set(LIBAV_ROOT_DIR /usr/include/x86_64-linux-gnu/)
find_package(LibAV REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBAV_INCLUDE_DIRS})

add_executable(aips-camera ${SOURCE_FILES})
target_link_libraries(aips-camera aips-common)
target_link_libraries(aips-camera ${OpenCV_LIBS})
target_link_libraries(aips-camera ${LIBAV_LIBRARIES})
target_link_libraries(aips-camera ${Boost_LIBRARIES})


