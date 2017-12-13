add_executable(triangulation triangulation.cpp)
target_link_libraries(triangulation agulib ${OpenCV_LIBS})
target_include_directories(triangulation PRIVATE "../lib")
