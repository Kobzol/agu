add_executable(tree tree.cpp)
target_link_libraries(tree agulib ${OpenCV_LIBS})
target_include_directories(tree PRIVATE "../lib")

add_executable(bsptree bsptree.cpp)
target_link_libraries(bsptree agulib ${OpenCV_LIBS})
target_include_directories(bsptree PRIVATE "../lib")
