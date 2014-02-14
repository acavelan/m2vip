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
 
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(tp  proj)

add_custom_target(prun1
    COMMAND mpirun   -mca btl tcp,self -np 1 tp
    DEPENDS tp
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(prun2
    COMMAND mpirun   -mca btl tcp,self -np 2 tp
    DEPENDS tp
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(prun4
    COMMAND mpirun   -mca btl tcp,self -np 4 tp
    DEPENDS tp
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
 
target_link_libraries(tp ${VTK_LIBRARIES} ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(tp PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(tp PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
