cmake_minimum_required(VERSION 2.8.3)
project(tesseract_ext)

if ($ENV{ROS_VERSION} VERSION_EQUAL "1")

    find_package(catkin REQUIRED)
    catkin_metapackage()

else()
    
    find_package(ament_cmake REQUIRED)

endif()