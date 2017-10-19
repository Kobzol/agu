add_library(cviko1 STATIC cviko1.cpp)
target_link_libraries(cviko1 agulib ${OpenCV_LIBS})
target_include_directories(cviko1 PRIVATE "../lib")
