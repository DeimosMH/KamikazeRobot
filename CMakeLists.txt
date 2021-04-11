cmake_minimum_required(VERSION 3.7)
project(robot)

set(CMAKE_CXX_STANDARD 14)

add_executable(robot src/main.cpp src/control.cpp src/control.h)
target_link_libraries(robot ${CMAKE_SOURCE_DIR}/lib/libev3dev.a)
