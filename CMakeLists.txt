cmake_minimum_required(VERSION 3.12)

project(StrokeSegBootStrapper VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

###############################################################################
# Sources Files and paths setup
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )# Include directories for headers

set(SOURCES # List of all source files
    consoleAllocator.cpp
    executors.cpp
    helpers.cpp
    commandLineTools.cpp
    main.cpp
	
	StrokeSegBootStrapper.rc
)

set(HEADERS # List of all header files
    consoleAllocator.h
    executors.h
    helpers.h
    commandLineTools.h
    deps.h
	
	resource.h
)

###############################################################################
# Add the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin) # Set output directory for all configurations
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE UNICODE _UNICODE)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_GLOBAL_CharacterSet "Unicode")
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE # Configure runtime libraries based on build configuration
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE # Configure subsystem
    $<$<CONFIG:Debug>:/SUBSYSTEM:WINDOWS>
    $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
)

install(TARGETS 
  ${CMAKE_PROJECT_NAME}
  RUNTIME DESTINATION .
  )

###############################################################################
# Doxygen : doxydoc generation
find_package(Doxygen)
if(Doxygen_FOUND)    
    option(BUILD_DOXYGEN "Build Doxygen documentation" ON)
    
    if(BUILD_DOXYGEN)
        set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Doxydoc")
        configure_file(Doxyfile.in Doxyfile)

        doxygen_add_docs(
            doxygen_docs
            ${CMAKE_CURRENT_SOURCE_DIR}
            ALL
            COMMENT "Generating Doxygen documentation"
            CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            )

        # Ajout de la documentation à la phase d'installation
        install(
            DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Doxydoc/html/"
            DESTINATION "../doc/${CMAKE_PROJECT_NAME}/html"
        )
    endif()
else()
    message(WARNING "Doxygen n'a pas été trouvé. La cible 'doxygen_docs' et l'installation de la documentation ne seront pas créées.")
endif()