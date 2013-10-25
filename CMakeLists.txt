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


##############################
# Build process for MATLAB  
##############################
option(MATLAB_BUILD "Select if library should be compiled for MATLAB usage" OFF)
if(MATLAB_BUILD)
	# add definition
	file(WRITE ${CMAKE_SOURCE_DIR}/include/build_matlab.h "#define HAVE_MATLAB")
	
	# generate script for MATLAB compilation to mex-files	
	set(MATLAB_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/mex)
	make_directory(${MATLAB_OUTPUT_PATH})
	if(WIN32)
		string(REPLACE ";" " -I" MATLAB_OpenCV_INCLUDE_DIRS "${OpenCV_INCLUDE_DIRS}")
		string(REPLACE ";" "/*.lib " MATLAB_OpenCV_LIB_DIR "${OpenCV_LIB_DIR}")
		
		file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat  "@echo off\nrem Automatically generated build file for MATLAB. Execute to build mex files. Do not modify.\n\n")
		file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat "set flibpath=${CMAKE_SOURCE_DIR}\n")
		file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat "set outputpath=${MATLAB_OUTPUT_PATH}\n\n")
		file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat "for /R %flibpath%/src/ %%f in (*.cpp) do (\n")
		file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat "    echo Compile %%~nxf\n")
		file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/matlab_compile.bat "	mex %%f -I%flibpath%/include/ -I%flibpath%/include/features/ -I${MATLAB_OpenCV_INCLUDE_DIRS} ${MATLAB_OpenCV_LIB_DIR}/*.lib -outdir %outputpath% -output %%~nfMex\n)")		
	endif()
	
	# TODO: Linux - build shell script to call mex compiler similar to windows batch file
	
	# create MATLAB wrapper files for classes
	file(READ ${CMAKE_SOURCE_DIR}/src/BaseFeature.m MATLAB_CLASS_TEMPLATE)
	# read cpp file names
	aux_source_directory(${CMAKE_SOURCE_DIR}/src CLASSES_NAMES)
	foreach(CLASS_PATH ${CLASSES_NAMES})
		get_filename_component(FEXT ${CLASS_PATH} EXT)
		if(FEXT STREQUAL ".cpp")
			# just replace the class names from the template and copy the new version to destination
			get_filename_component(FN ${CLASS_PATH} NAME_WE)
			string(REPLACE "BaseFeatureMex" "${FN}Mex" MATLAB_CLASS_FILE "${MATLAB_CLASS_TEMPLATE}")
			string(REPLACE "BaseFeature" "${FN}" MATLAB_CLASS_FILE "${MATLAB_CLASS_FILE}")
			file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/mex/${FN}.m "${MATLAB_CLASS_FILE}")
		endif()
	endforeach()	

	message(STATUS "Note: MATLAB build file generated. Execute matlab_compile.bat manually to compile mex-files for MATLAB. CMake generating is not necessary.")	
else()
	# remove definition if existing
	file(WRITE ${CMAKE_SOURCE_DIR}/include/build_matlab.h "")	
endif()