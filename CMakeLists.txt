cmake_minimum_required(VERSION 3.28)
project(Snake)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(Snake main.cpp)
target_link_libraries(Snake PRIVATE raylib winmm)