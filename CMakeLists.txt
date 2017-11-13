cmake_minimum_required(VERSION 3.4)

project(agu)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_subdirectory("lib")
add_subdirectory("cviko1")
add_subdirectory("cviko2")
add_subdirectory("convexhull")
add_subdirectory("tree")
add_subdirectory("closeness")
add_subdirectory("triangulation")

add_executable(agu agu.cpp)
target_link_libraries(agu agulib cviko1 cviko2 convexhull tree closeness triangulation ${OpenCV_LIBS})
