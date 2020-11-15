cmake_minimum_required(VERSION 3.13)
project(facetrack)# project name
set(CMAKE_CXX_STANDARD 11) 
set(SOURCE_FILES facetrack.cpp drawLandmarks.cpp DetectTrackFace.cpp socket_rasp.cpp PWM_Control.cpp sha1.cpp base64.cpp DatabaseManager.cpp) 

     
# locate libraries
find_package(OpenCV REQUIRED) #cmake find the package 
find_package(Threads REQUIRED)
find_package(WiringPi REQUIRED)
find_package(MariaDBClient REQUIRED)

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS}) #include the headers which are not in the defalult path 
include_directories(${MariaDBClient_INCLUDE_DIR})

add_executable(facetrack ${SOURCE_FILES})#generate the executable file 

# link libraries
target_link_libraries(facetrack ${OpenCV_LIBS})#build the connection between the libraries and source file
target_link_libraries(facetrack ${WIRINGPI_LIBRARIES})
target_link_libraries(facetrack ${MariaDBClient_LIBRARY})


