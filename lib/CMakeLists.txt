add_library(agulib STATIC
        algo.cpp
        polygon.cpp)
target_include_directories(agulib PUBLIC ${OpenCV_INCLUDE_DIRS})
