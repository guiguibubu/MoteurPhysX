cmake_minimum_required( VERSION 3.0 )
project( Simulation C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(GLOBAL PROPERTY CMAKE_SKIP_INSTALL_ALL_DEPENDENCY  ON)
set_property(GLOBAL PROPERTY CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY  ON)
set_property(GLOBAL PROPERTY CMAKE_SUPPRESS_REGENERATION  ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Filters
include( Filters.cmake )

SET (SIMULATION_INCLUDE_DIR
	${CMAKE_SOURCE_DIR}/Include
	${CMAKE_SOURCE_DIR}/Include/Graphics/include/win32/GL
	${CMAKE_SOURCE_DIR}/Include/PxShared/include
	${CMAKE_SOURCE_DIR}/Include/PxInclude
	${CMAKE_SOURCE_DIR}/Include/PxShared/src/foundation/include
	${CMAKE_SOURCE_DIR}/Include/PxToolkit/include
	#${CMAKE_SOURCE_DIR}/Include/SnippetRender
)

include_directories(${SIMULATION_INCLUDE_DIR})

SET (SIMULATION_LIB_DIR
	${CMAKE_SOURCE_DIR}/Include/PxShared/lib/vc15win64
	${CMAKE_SOURCE_DIR}/Include/SnippetUtils/Lib
	${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib
	${CMAKE_SOURCE_DIR}/Include/Graphics/lib/win64/glut
	${CMAKE_SOURCE_DIR}/Include/API_PhysX/Lib
	${CMAKE_SOURCE_DIR}/Include/PxToolkit/lib
)
link_directories(${SIMULATION_LIB_DIR})

add_compile_options(/std:c++latest)
add_compile_options(/MTd)

#Projet Simulation
SET( SIMULATION_BUILDFILES 
)

SET( SIMULATION_HEADERS 
	Simulation/MyCallback.h
	Simulation/SampleAllocator.h
	Simulation/Simulation.h
	Simulation/SimulationRender.h
	Simulation/commonImport.h
	Simulation/SnippetCamera.h
	Simulation/SnippetRender.h
	Simulation/Incopiable.h
	Simulation/Minuteur.h
)

SET( SIMULATION_SRC 

	#Framework
	Simulation/Simulation.cpp
	Simulation/SimulationRender.cpp
	Simulation/MyCallback.cpp
	Simulation/SampleAllocator.cpp
	Simulation/SnippetCamera.cpp
	Simulation/SnippetRender.cpp
	Simulation/Minuteur.cpp

	#Main
	Simulation/ClassicMain.cpp
)

SET(jobfiles "${SIMULATION_SRC};${SIMULATION_BUILDFILES}")
file(WRITE jobfilesSimulation.txt "${jobfiles}")

add_executable(
	Simulation
	${SIMULATION_SRC}
	${SIMULATION_HEADERS}
	${SIMULATION_BUILDFILES}
)

SET( SIMULATION_TARGET_PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Bin/"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Bin/"
)

SET( SIMULATION_TARGET_LINK_LIBRARIES 
	PhysX3CharacterKinematicDEBUG_x64.lib
	PhysX3CommonDEBUG_x64.lib
	PhysX3CookingDEBUG_x64.lib
	PhysX3DEBUG_x64.lib
	PhysX3ExtensionsDEBUG.lib
	PhysX3VehicleDEBUG.lib
	SceneQueryDEBUG.lib
	SimulationControllerDEBUG.lib
	glut32.lib
	PsFastXmlDEBUG_x64.lib
	PxFoundationDEBUG_x64.lib
	PxPvdSDKDEBUG_x64.lib
	PxTaskDEBUG_x64.lib
	SnippetUtilsDEBUG.lib
	SampleBaseDEBUG.lib
	PxToolkitDEBUG.lib
)

set_target_properties(Simulation  PROPERTIES ${SIMULATION_TARGET_PROPERTIES})

target_link_libraries (Simulation ${SIMULATION_TARGET_LINK_LIBRARIES})


CREATE_VISUAL_STUDIO_FILTERS(SIMULATION)

