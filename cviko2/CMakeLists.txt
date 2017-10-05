add_library(cviko2 STATIC cviko2.cpp)
target_link_libraries(cviko2 agulib ${OpenCV_LIBS})
target_include_directories(cviko2 PRIVATE "../lib")
