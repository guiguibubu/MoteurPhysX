cmake_minimum_required( VERSION 3.0 )
project( Simulation C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/")

project(Simulation)

#Projet Simulation
SET( SIMULATION_BUILDFILES 

	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
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

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Include/SnippetRender/Lib/")

#Projet Render
SET( RENDER_BUILDFILES 

	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
)

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


CREATE_VISUAL_STUDIO_FILTERS(SIMULATION)

