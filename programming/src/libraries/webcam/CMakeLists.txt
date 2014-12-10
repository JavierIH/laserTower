add_library(webcam SHARED webcam.cpp)
target_link_libraries(webcam webcam debuga ${OpenCV_LIBS})


