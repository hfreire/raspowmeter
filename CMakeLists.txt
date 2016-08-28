cmake_minimum_required(VERSION 3.4)
project(raspowmeter)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.c src/gpio.c include/gpio.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(GLIB2 REQUIRED)
if (GLIB2_FOUND)
    include_directories(${PROJECT_NAME} ${GLIB2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES})
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} pthread rt)
endif ()