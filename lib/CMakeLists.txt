add_library(agulib STATIC
        algo.cpp
        draw.cpp
        polygon.cpp
        closeness/closeness.cpp
        convexhull/convexhull.cpp
        planarmap/vertex.cpp
        planarmap/planarmap.cpp
        planarmap/triangle.cpp
        planesweep/planesweep.cpp
        planesweep/segment.cpp
        planesweep/event.cpp
        planesweep/sweepstatus.cpp
        planesweep/segmentcomparer.cpp
        tree/kdtree.cpp
        tree/intervaltree.cpp
        tree/triangletree.cpp
        tree/bsptree.cpp
        triangulation/triangulation.cpp
)
target_include_directories(agulib PUBLIC ${OpenCV_INCLUDE_DIRS})
