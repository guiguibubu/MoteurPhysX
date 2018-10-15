cmake_minimum_required( VERSION 3.0 )
project( Simulation C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )

SET (SIMULATION_INCLUDE_DIR
	${CMAKE_SOURCE_DIR}/Include
	${CMAKE_SOURCE_DIR}/Include/Graphics/include/win32/GL
	${CMAKE_SOURCE_DIR}/Include/PxShared/include
	${CMAKE_SOURCE_DIR}/Include/PxInclude
)

include_directories(${SIMULATION_INCLUDE_DIR})

SET (SIMULATION_LIB_DIR
	${CMAKE_SOURCE_DIR}/Include/PxShared/lib/vc15win64
	${CMAKE_SOURCE_DIR}/Include/SnippetUtils/Lib
	${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib
	${CMAKE_SOURCE_DIR}/Include/Graphics/lib/win64/glut
	${CMAKE_SOURCE_DIR}/API_PhysX/Lib
)
link_directories(${SIMULATION_LIB_DIR})

add_compile_options(/std:c++latest)
add_compile_options(/MTd)
add_compile_definitions(RENDER_SNIPPET)

project(Simulation)

#Projet Simulation
SET( SIMULATION_BUILDFILES 
	Include/ConsigneDevoir2.pdf
)

SET( SIMULATION_SRC 

	#Framework
	Simulation/SnippetHelloWorld.cpp
	Simulation/SnippetHelloWorldRender.cpp

	#Main
	Simulation/ClassicMain.cpp
)

SET(jobfiles "${SIMULATION_SRC};${SIMULATION_BUILDFILES}")
file(WRITE jobfilesSimulation.txt "${jobfiles}")

add_executable(

	Simulation
	${SIMULATION_SRC}
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
	RenderDEBUG.lib
	SnippetUtilsDEBUG.lib
)

set_target_properties(Simulation  PROPERTIES ${SIMULATION_TARGET_PROPERTIES})

target_link_libraries (Simulation ${SIMULATION_TARGET_LINK_LIBRARIES})

#Projet Render
SET( RENDER_HEADERS 

	Render/SnippetCamera.h
	Render/SnippetRender.h
)

SET( RENDER_SRC 

	Render/SnippetCamera.cpp
	Render/SnippetRender.cpp
)

SET(jobfiles "${RENDER_HEADERS};${RENDER_SRC};${RENDER_BUILDFILES}")
file(WRITE jobfilesRender.txt "${jobfiles}")

add_library(

	Render STATIC
	${RENDER_BUILDFILES}
	${RENDER_HEADERS}
	${RENDER_SRC}
)

SET( RENDER_TARGET_PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/"
)

set_target_properties(Render PROPERTIES ARCHIVE_OUTPUT_NAME_DEBUG "RenderDEBUG")
set_target_properties(Render PROPERTIES ${RENDER_TARGET_PROPERTIES})

target_link_libraries (Simulation Render)

CREATE_VISUAL_STUDIO_FILTERS(SIMULATION)

