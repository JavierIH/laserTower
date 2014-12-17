find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})

find_package(RD REQUIRED)
include_directories(${RD_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})


OPTION(APP_YES_WE_CAN "App for controlling the turret and shooting down cans" 1)
IF (APP_YES_WE_CAN)
add_executable(yes_we_can yes_we_can.cpp)
target_link_libraries(yes_we_can Turret FaceDetector color_detector ${YARP_LIBRARIES} ${OpenCV_LIBS} ${RD_LIBRARIES})
ENDIF ()

OPTION(APP_YES_WE_CAN_HSV "App for controlling the turret and shooting down cans in HSV colorspace" 1)
IF (APP_YES_WE_CAN_HSV)
add_executable(yes_we_can_hsv yes_we_can_hsv.cpp)
target_link_libraries(yes_we_can_hsv Turret FaceDetector color_detector ${YARP_LIBRARIES} ${OpenCV_LIBS} ${RD_LIBRARIES})
ENDIF ()

### ESTRUCTURA PARA ANADIR APPS
#OPTION(VARIABLE "<descripcion>" 1)
#IF (VARIABLE)
#add_executable(nombre_de_archivo nombre_de_archivo.cpp)
#target_link_libraries(nombre_de_archivoi debuga ${OpenCV_LIBS})
#ENDIF ()

