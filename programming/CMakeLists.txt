                                

project(LASERTOWER)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${LASERTOWER_SOURCE_DIR})
find_package (OpenCV)
find_package (ZBar0)
set(TRUNK_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LASERTOWER_BIN_DIR "${TRUNK_DIR}/build/bin")
set(LASERTOWER_TESTS_DIR "${TRUNK_DIR}/build/tests")
set(LASERTOWER_LIBS_DIR "${TRUNK_DIR}/build/lib")

set(LIBRARY_OUTPUT_PATH ${LASERTOWER_LIBS_DIR})
set(EXECUTABLE_OUTPUT_PATH ${LASERTOWER_BIN_DIR})
set(TEST_OUTPUT_PATH ${LASERTOWER_TESTS_DIR})

add_subdirectory(src)


