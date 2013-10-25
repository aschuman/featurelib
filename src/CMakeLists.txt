file(GLOB LIB_HDRS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB LIB_SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# need this so full header tree shows up in qtcreator
file(GLOB LIB_HDRS2 "${CMAKE_SOURCE_DIR}/include/features/*.h")
list(APPEND LIB_HDRS ${LIB_HDRS2})

add_library(featurelib STATIC
    ${LIB_HDRS}
    ${LIB_SRCS}
)

target_link_libraries(featurelib
    ${OpenCV_LIBRARIES}
)
