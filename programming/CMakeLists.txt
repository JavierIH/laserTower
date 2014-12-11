                                

project(LASERTOWER)

cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0022 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH ${LASERTOWER_SOURCE_DIR})
find_package(OpenCV REQUIRED)
find_package(ZBar0 REQUIRED)

set(TRUNK_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LASERTOWER_BIN_DIR "${TRUNK_DIR}/build/bin")
set(LASERTOWER_TESTS_DIR "${TRUNK_DIR}/build/tests")
set(LASERTOWER_LIBS_DIR "${TRUNK_DIR}/build/lib")

set(LIBRARY_OUTPUT_PATH ${LASERTOWER_LIBS_DIR})
set(EXECUTABLE_OUTPUT_PATH ${LASERTOWER_BIN_DIR})
set(TEST_OUTPUT_PATH ${LASERTOWER_TESTS_DIR})

find_package(RD REQUIRED)
include_directories(${RD_INCLUDE_DIRS})

add_subdirectory(src)


