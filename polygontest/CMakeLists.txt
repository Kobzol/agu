add_executable(polygontest polygontest.cpp)
target_link_libraries(polygontest agulib ${OpenCV_LIBS})
target_include_directories(polygontest PRIVATE "../lib")
