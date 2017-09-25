cmake_minimum_required(VERSION 3.4)

project(agu)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_subdirectory("lib")
add_subdirectory("cviko1")
