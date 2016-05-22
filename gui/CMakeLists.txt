
find_package(Qt4 REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)


find_package(Boost REQUIRED COMPONENTS system regex)
set(QT_COMPONENTS_REQ Qt4::QtGui)
find_package(OpenMP REQUIRED)
set(SOURCE_HEADERS gui.h gui_connector.h)
set(SOURCE_CPP main.cpp gui.cpp gui_connector.cpp)
set(SOURCE_FILES ${SOURCE_CPP} ${SOURCE_HEADERS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")


add_executable(aips-gui ${SOURCE_FILES})
target_link_libraries(aips-gui ${OpenMP_LIBRARIES} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY} ${QT_COMPONENTS_REQ})
target_link_libraries(aips-gui aips-common)

