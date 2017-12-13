add_executable(convexhull convexhull.cpp)
target_link_libraries(convexhull agulib ${OpenCV_LIBS})
target_include_directories(convexhull PRIVATE "../lib")
