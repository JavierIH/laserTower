OPTION(RGB_CAN "Can detection in RGB" 1)
IF (RGB_CAN)
add_executable(test_rgb_can test_rgb_can.cpp)
target_link_libraries(test_rgb_can debuga ${OpenCV_LIBS})
ENDIF ()


### ESTRUCTURA PARA ANADIR TESTS
#OPTION(VARIABLE "<descripcion>" 1)
#IF (VARIABLE)
#add_executable(nombre_de_archivo nombre_de_archivo.cpp)
#target_link_libraries(nombre_de_archivoi debuga ${OpenCV_LIBS})
#ENDIF ()

