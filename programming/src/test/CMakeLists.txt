OPTION(TEST_RGB_CAN "Can detection in RGB" 1)
IF (TEST_RGB_CAN)
add_executable(test_rgb_can test_rgb_can.cpp)
target_link_libraries(test_rgb_can color_detector webcam debuga ${OpenCV_LIBS})
ENDIF ()

OPTION(TEST_WEBCAM "Testing the camera" 1)
IF (TEST_WEBCAM)
add_executable(test_webcam test_webcam.cpp)
target_link_libraries(test_webcam webcam debuga ${OpenCV_LIBS})
ENDIF ()

OPTION(TEST_BOXES "Bounding boxes of cans" 1)
IF (TEST_BOXES)
add_executable(test_boxes test_boxes.cpp)
target_link_libraries(test_boxes color_detector webcam debuga ${OpenCV_LIBS})
ENDIF ()

OPTION(TEST_TURRET "Can detection in RGB" 1)
IF (TEST_TURRET)
add_executable(test_Turret test_Turret.cpp)
target_link_libraries(test_Turret Turret ${OpenCV_LIBS})
ENDIF ()

### ESTRUCTURA PARA ANADIR TESTS
#OPTION(VARIABLE "<descripcion>" 1)
#IF (VARIABLE)
#add_executable(nombre_de_archivo nombre_de_archivo.cpp)
#target_link_libraries(nombre_de_archivoi debuga ${OpenCV_LIBS})
#ENDIF ()

