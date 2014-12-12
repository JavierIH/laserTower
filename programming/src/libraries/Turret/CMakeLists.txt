find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})

find_package(RD REQUIRED)
include_directories(${RD_INCLUDE_DIRS})

add_library(Turret turret.cpp)
target_link_libraries(Turret debuga ${OpenCV_LIBS} ${YARP_LIBRARIES} ${RD_LIBRARIES})


