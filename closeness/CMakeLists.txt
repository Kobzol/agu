add_library(closeness STATIC closeness.cpp)
target_link_libraries(closeness agulib ${OpenCV_LIBS})
target_include_directories(closeness PRIVATE "../lib")
