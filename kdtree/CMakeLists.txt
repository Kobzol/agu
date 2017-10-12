add_library(kdtree STATIC kdtree.cpp)
target_link_libraries(kdtree agulib ${OpenCV_LIBS})
target_include_directories(kdtree PRIVATE "../lib")
