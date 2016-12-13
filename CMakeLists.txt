project(trackVideo)
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH
  /home/tiger/qt-5.7.0/5.7/gcc_64/lib/cmake/Qt5Core;
  /home/tiger/qt-5.7.0/5.7/gcc_64/lib/cmake/Qt5Gui;
  /home/tiger/qt-5.7.0/5.7/gcc_64/lib/cmake/Qt5Widgets;
  )
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
find_package(Threads REQUIRED)

file (GLOB Resources *.qrc)
find_package(Qt5Widgets REQUIRED)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${Resources})

target_link_libraries(trackVideo Threads::Threads Qt5::Widgets
  -lavcodec
  -lswscale
  -lavformat
  -lavutil )
