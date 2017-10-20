add_library(agulib STATIC
        algo.cpp
        polygon.cpp
        planesweep/planesweep.cpp
        planesweep/segment.cpp
        planesweep/event.cpp
        planesweep/sweepstatus.cpp
        planesweep/segmentcomparer.cpp
        convexhull/convexhull.cpp
        draw.cpp
        tree/kdtree.cpp
        tree/intervaltree.cpp tree/intervaltree.h closeness/closeness.cpp closeness/closeness.h)
target_include_directories(agulib PUBLIC ${OpenCV_INCLUDE_DIRS})
