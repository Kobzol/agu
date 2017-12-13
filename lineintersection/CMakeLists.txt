add_executable(lineintersection lineintersection.cpp)
target_link_libraries(lineintersection agulib ${OpenCV_LIBS})
target_include_directories(lineintersection PRIVATE "../lib")
