cmake_minimum_required(VERSION 2.8)

###############################################################################
# Project setup
###############################################################################
project(FeatureLib)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# OpenCV
find_package(OpenCV REQUIRED)


set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
make_directory(${LIBRARY_OUTPUT_PATH})
make_directory(${EXECUTABLE_OUTPUT_PATH})
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)


include_directories("${CMAKE_SOURCE_DIR}/include")
add_subdirectory(src)
add_subdirectory(apps)

