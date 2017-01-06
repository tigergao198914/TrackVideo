project(trackVideo)
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH
  /home/tiger/Qt5.7.0/5.7/gcc_64/lib/cmake/Qt5Core;
  /home/tiger/Qt5.7.0/5.7/gcc_64/lib/cmake/Qt5Gui;
  /home/tiger/Qt5.7.0/5.7/gcc_64/lib/cmake/Qt5Widgets;
  /usr/local/ffmpeg/lib;
  )

IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

#enable threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#add resource
file (GLOB Resources *.qrc)

find_package(Qt5Widgets REQUIRED)

include_directories(/usr/local/MATLAB/R2015b/extern/include)
include_directories(./ffmpeg)
#find source file
aux_source_directory(. SRC_LIST)
aux_source_directory(ui UI)
aux_source_directory(ffmpeg FFMPEG)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${UI} ${FFMPEG} ${Resources})
FIND_LIBRARY( TRACKLIB Track ./matlab REQUIRED )
FIND_LIBRARY( CLM mwmclmcrrt /usr/local/MATLAB/R2015b/runtime/glnxa64 REQUIRED)
message(${TRACKLIB})
message(${CLM})
#link libraries
target_link_libraries(trackVideo Threads::Threads Qt5::Widgets
 -lpostproc
  -lavresample
  -lavdevice
  -lavutil
  -lavfilter
  -lavcodec
  -lswscale
  -lswresample
  -lavformat
  ${TRACKLIB}
  ${CLM})
