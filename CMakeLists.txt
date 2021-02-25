cmake_minimum_required(VERSION 3.7)
project(robot)

set(CMAKE_CXX_STANDARD 14)

add_executable(robot main.cpp)
target_link_libraries(robot ${CMAKE_SOURCE_DIR}/libev3dev.a)