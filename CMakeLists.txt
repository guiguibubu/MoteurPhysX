cmake_minimum_required( VERSION 3.0 )
project( Simulation C CXX )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(GLOBAL PROPERTY CMAKE_SKIP_INSTALL_ALL_DEPENDENCY  ON)
set_property(GLOBAL PROPERTY CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY  ON)
set_property(GLOBAL PROPERTY CMAKE_SUPPRESS_REGENERATION  ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Compile Option
SET (MY_COMPILE_OPTIONS /std:c++17 -D_UNICODE -DUNICODE /MP /FR )

SET (MY_RELEASE_OPTIONS ${MY_COMPILE_OPTIONS})
SET (MY_DEBUG_OPTIONS ${MY_COMPILE_OPTIONS} -D_DEBUG /MDd)

SET (SIMULATION_RELEASE_OPTIONS ${MY_RELEASE_OPTIONS})
SET (SIMULATION_DEBUG_OPTIONS ${MY_DEBUG_OPTIONS})

SET (DOSSIER_BIN_SORTIE ${CMAKE_SOURCE_DIR}/Bin)
SET (DOSSIER_INCLUDE_SORTIE ${CMAKE_SOURCE_DIR}/Include)
SET (DOSSIER_API_PHYSX ${DOSSIER_INCLUDE_SORTIE}/API_PhysX)
SET (DOSSIER_GLUT ${DOSSIER_INCLUDE_SORTIE}/Graphics)

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
    ${DOSSIER_API_PHYSX}/Lib
    ${CMAKE_SOURCE_DIR}/Include/PxToolkit/lib
)
link_directories(${SIMULATION_LIB_DIR})

#Projet Simulation
SET( SIMULATION_HEADERS 
    Simulation/FilterShader.h
    Simulation/SampleAllocator.h
    Simulation/Simulation.h
    Simulation/SimulationRender.h
    Simulation/commonImport.h
    #Simulation/SnippetCamera.h
    #Simulation/SnippetRender.h
    Simulation/Incopiable.h
    Simulation/Minuteur.h
    Simulation/Vehicule.h
    Simulation/Goal.h
    Simulation/Balle.h
    Simulation/Bot.h
)

SET( SIMULATION_SRC 

    #Framework
    Simulation/Simulation.cpp
    Simulation/SimulationRender.cpp
    Simulation/FilterShader.cpp
    Simulation/SampleAllocator.cpp
    #Simulation/SnippetCamera.cpp
    #Simulation/SnippetRender.cpp
    Simulation/Minuteur.cpp
    Simulation/Vehicule.cpp
    Simulation/Goal.cpp
    Simulation/Balle.cpp
    Simulation/Bot.cpp

    #Main
    Simulation/ClassicMain.cpp
)

SET( SIMULATION_BUILDFILES 

    CheckInstall.bat
    CleanAll.bat
    CMakeLists.txt
    Filters.cmake
    Include/ConsigneDevoir2.pdf
)

SET(jobfiles "${SIMULATION_SRC};${SIMULATION_HEADERS};${SIMULATION_BUILDFILES}")
file(WRITE jobfilesSimulation.txt "${jobfiles}")

add_executable(
    Simulation
    ${SIMULATION_SRC}
    ${SIMULATION_HEADERS}
    ${SIMULATION_BUILDFILES}
)

SET( SIMULATION_TARGET_PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${DOSSIER_BIN_SORTIE}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DOSSIER_BIN_SORTIE}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DOSSIER_BIN_SORTIE}"
)

SET( SIMULATION_TARGET_LINK_LIBRARIES
    freeglut_staticd.lib
    Render.lib
)

SET( SIMULATION_TARGET_LINK_LIBRARIES_DEBUG 
    ${SIMULATION_TARGET_LINK_LIBRARIES}
    PhysX3CharacterKinematicDEBUG_x64.lib
    PhysX3CommonDEBUG_x64.lib
    PhysX3CookingDEBUG_x64.lib
    PhysX3DEBUG_x64.lib
    PhysX3ExtensionsDEBUG.lib
    PhysX3VehicleDEBUG.lib
    SceneQueryDEBUG.lib
    SimulationControllerDEBUG.lib
    PsFastXmlDEBUG_x64.lib
    PxFoundationDEBUG_x64.lib
    PxPvdSDKDEBUG_x64.lib
    PxTaskDEBUG_x64.lib
    SnippetUtilsDEBUG.lib
    SampleBaseDEBUG.lib
    PxToolkitDEBUG.lib
)

SET( SIMULATION_TARGET_LINK_LIBRARIES_RELEASE 
    ${SIMULATION_TARGET_LINK_LIBRARIES}
    PhysX3CharacterKinematic_x64.lib
    PhysX3Common_x64.lib
    PhysX3Cooking_x64.lib
    PhysX3_x64.lib
    PhysX3Extensions.lib
    PhysX3Vehicle.lib
    SceneQuery.lib
    SimulationController.lib
    PsFastXml_x64.lib
    PxFoundation_x64.lib
    PxPvdSDK_x64.lib
    PxTask_x64.lib
    SnippetUtils.lib
    SampleBase.lib
    PxToolkit.lib
)

set_target_properties(Simulation  PROPERTIES ${SIMULATION_TARGET_PROPERTIES})

target_link_libraries (Simulation ${SIMULATION_TARGET_LINK_LIBRARIES})

target_link_libraries(Simulation "$<$<CONFIG:RELEASE>:${SIMULATION_TARGET_LINK_LIBRARIES_RELEASE}>")
target_link_libraries(Simulation "$<$<CONFIG:DEBUG>:${SIMULATION_TARGET_LINK_LIBRARIES_DEBUG}>")

target_compile_options(Simulation PUBLIC "$<$<CONFIG:RELEASE>:${SIMULATION_RELEASE_OPTIONS}>")
target_compile_options(Simulation PUBLIC "$<$<CONFIG:DEBUG>:${SIMULATION_DEBUG_OPTIONS}>")

file(TO_NATIVE_PATH ${DOSSIER_API_PHYSX}/DLL DOSSIER_MOTEUR_PHYSX_NATIVE_PATH)
file(TO_NATIVE_PATH ${DOSSIER_GLUT}/DLL/win64 DOSSIER_GLUT_NATIVE_PATH)
file(TO_NATIVE_PATH ${DOSSIER_BIN_SORTIE} DOSSIER_BIN_SORTIE_NATIVE_PATH)

add_custom_command(TARGET Simulation
                   POST_BUILD
                   COMMAND xcopy ARGS "/y /f /e \"${DOSSIER_MOTEUR_PHYSX_NATIVE_PATH}\"  \"${DOSSIER_BIN_SORTIE_NATIVE_PATH}\""
                   COMMENT "Running copy PhysX DLLs")

add_custom_command(TARGET Simulation
                   POST_BUILD
                   COMMAND xcopy ARGS "/y /f /e \"${DOSSIER_GLUT_NATIVE_PATH}\"  \"${DOSSIER_BIN_SORTIE_NATIVE_PATH}\""
                   COMMENT "Running copy Glut DLLs")

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

    Render/commonImport.h
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

add_dependencies(Simulation Render)


# Filters
include( Filters.cmake )
CREATE_VISUAL_STUDIO_FILTERS(SIMULATION)

