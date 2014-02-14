cmake_minimum_required(VERSION 2.8)
 
PROJECT(vtktp)

IF(APPLE)

SET( CMAKE_PREFIX_PATH /Users/emmanuelmelin/developpements/VTK/BIN/lib/vtk-5.10)

ENDIF (APPLE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET(OperatingSystem "Linux")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(tp  proj.cpp)

target_link_libraries(tp ${VTK_LIBRARIES})
