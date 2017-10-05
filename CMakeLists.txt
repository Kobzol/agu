cmake_minimum_required(VERSION 3.4)

project(agu)

set(CMAKE_CXX_STANDARD 14)

set(OpenCV_DIR "D:/pg/Programovani/graphics/agu/opencv")
find_package(OpenCV REQUIRED)

add_subdirectory("lib")
add_subdirectory("cviko1")
add_subdirectory("cviko2")

add_executable(agu agu.cpp)
target_link_libraries(agu agulib cviko1 cviko2 ${OpenCV_LIBS})
