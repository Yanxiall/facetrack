cmake_minimum_required(VERSION 3.13)
project(remoteMonitor)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)