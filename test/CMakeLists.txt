cmake_minimum_required(VERSION 3.13)
project(facetrackTest)# project name
set(CMAKE_CXX_STANDARD 11) 
set(SOURCE_FILES main.cpp ../src/DetectTrackFace.cpp ../src/drawLandmarks.cpp DetectTrackFace_test.cpp) 
set(GTEST_ROOT "/usr/src/googletest" CACHE PATH "Path to googletest")

# locate libraries
find_package(OpenCV REQUIRED) #cmake find the package 
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(facetrackTest ${SOURCE_FILES})#generate the executable file 

# link libraries
target_link_libraries(facetrackTest ${OpenCV_LIBS})
target_link_libraries(facetrackTest ${GTEST_LIBRARIES} pthread)