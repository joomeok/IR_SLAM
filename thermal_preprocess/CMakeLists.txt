cmake_minimum_required(VERSION 3.1)
project(thermal_filter)
set(CMAKE_CXX_STANDARD 17)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    message_filters
    cv_bridge
    )
find_package(OpenCV REQUIRED)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES opencv
    CATKIN_DEPENDS roscpp cv_bridge message_filters
    )

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    /home/jaeho/catkin_ws/src/thermal_preprocess/include
    )

add_library(img_histo ./src/image_histogram.cpp)
add_executable(filtering_node ./src/main.cpp)
target_link_libraries(filtering_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} img_histo)
