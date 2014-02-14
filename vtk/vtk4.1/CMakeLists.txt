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

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(tp  proj)
 
target_link_libraries(tp ${VTK_LIBRARIES})
