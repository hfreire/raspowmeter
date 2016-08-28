cmake_minimum_required(VERSION 3.4)
project(raspowmeter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/raspowmeter.c src/gpio.c include/gpio.h include/rasppowmeter.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include(FindPkgConfig)

pkg_check_modules(GLIB2 glib-2.0 REQUIRED)
include_directories(${GLIB2_INCLUDE_DIRS})
link_directories (${GLIB2_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES})