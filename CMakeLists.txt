cmake_minimum_required(VERSION 3.10)

project(camera-module)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)

# include_directories(${OpenCV_INCLUDE_DIRS})
# Add library
add_library(Camera src/Camera.cpp)

# Include RealSense and OpenCV Libraries
target_link_libraries(Camera ${realsense2_LIBRARY})

# Add library
add_library(CameraManager src/CameraManager.cpp)

# Include RealSense and OpenCV Libraries
target_link_libraries(CameraManager ${realsense2_LIBRARY})
target_link_libraries(CameraManager ${OpenCV_LIBS})
target_link_libraries(CameraManager Camera)



# Test main File Configuration

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(camera-module CameraManager)

target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

