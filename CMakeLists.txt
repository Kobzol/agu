cmake_minimum_required(VERSION 3.4)

project(agu)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_subdirectory("lib")
add_subdirectory("polygontest")
add_subdirectory("lineintersection")
add_subdirectory("convexhull")
add_subdirectory("tree")
add_subdirectory("closeness")
add_subdirectory("triangulation")
