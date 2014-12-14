find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})

find_package(RD REQUIRED)
include_directories(${RD_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})

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

OPTION(TEST_TURRET "Testing turret movement" 1)
IF (TEST_TURRET)
add_executable(test_Turret test_Turret.cpp)
target_link_libraries(test_Turret Turret ${YARP_LIBRARIES} ${OpenCV_LIBS} ${RD_LIBRARIES})
ENDIF ()

OPTION(TEST_FACE_DETECTOR "Testing haar face detector" 1)
IF (TEST_FACE_DETECTOR)
add_executable(test_FaceDetector test_FaceDetector.cpp)
target_link_libraries(test_FaceDetector FaceDetector debuga webcam ${OpenCV_LIBS})
ENDIF ()

OPTION(TEST_CONTROL_LOOP "Testing turret control loop" 1)
IF (TEST_CONTROL_LOOP)
add_executable(test_control_loop test_control_loop.cpp)
target_link_libraries(test_control_loop Turret FaceDetector ${YARP_LIBRARIES} ${OpenCV_LIBS} ${RD_LIBRARIES})
ENDIF ()

### ESTRUCTURA PARA ANADIR TESTS
#OPTION(VARIABLE "<descripcion>" 1)
#IF (VARIABLE)
#add_executable(nombre_de_archivo nombre_de_archivo.cpp)
#target_link_libraries(nombre_de_archivoi debuga ${OpenCV_LIBS})
#ENDIF ()

