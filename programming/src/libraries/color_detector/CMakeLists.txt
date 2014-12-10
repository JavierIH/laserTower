add_library(color_detector SHARED color_detector.cpp)
target_link_libraries(color_detector color_detector webcam debuga ${OpenCV_LIBS})


