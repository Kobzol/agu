add_library(agulib STATIC
        algo.cpp
        polygon.cpp
        planesweep/planesweep.cpp
        planesweep/segment.cpp
        planesweep/event.cpp
        planesweep/sweepstatus.cpp convexhull/convexhull.h convexhull/convexhull.cpp)
target_include_directories(agulib PUBLIC ${OpenCV_INCLUDE_DIRS})
