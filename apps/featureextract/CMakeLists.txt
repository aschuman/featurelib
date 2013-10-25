add_executable(featureextract featureextract_main.cpp)
target_link_libraries(featureextract
    featurelib
    ${OpenCV_LIBRARIES}
)

set_target_properties(featureextract PROPERTIES OUTPUT_NAME fextract)
install(TARGETS featureextract DESTINATION bin)
