# PangoBasic
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE on )

get_filename_component( CINDER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE )
include( ${CINDER_DIR}/linux/cmake/Cinder.cmake )

project( PangoBasic )

get_filename_component( PANGO_BLOCK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../src" ABSOLUTE )
get_filename_component( SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src" ABSOLUTE )
get_filename_component( INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" ABSOLUTE )

if( NOT TARGET cinder${CINDER_LIB_SUFFIX} )
    find_package( cinder REQUIRED
        PATHS ${PROJECT_SOURCE_DIR}/../../../../../linux/${CMAKE_BUILD_TYPE}/${CINDER_OUT_DIR_PREFIX}
        $ENV{Cinder_DIR}/linux/${CMAKE_BUILD_TYPE}/${CINDER_OUT_DIR_PREFIX}
    )
endif()

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# Find and include Pango and dependencies.
find_package( HarfBuzz REQUIRED )
find_package( Cairo REQUIRED )
find_package( Pango REQUIRED )

# Use PROJECT_NAME since CMAKE_PROJET_NAME returns the top-level project name.
set( EXE_NAME ${PROJECT_NAME} )

set( SRC_FILES
	${SRC_DIR}/PangoBasicApp.cpp
    ${PANGO_BLOCK_SRC_DIR}/CinderPango.cpp
)

add_executable( "${EXE_NAME}" ${SRC_FILES} )

target_include_directories(
	"${EXE_NAME}"
    PUBLIC ${INC_DIR} 
           ${PANGO_BLOCK_SRC_DIR}
           ${HARFBUZZ_INCLUDE_DIRS}
           ${CAIRO_INCLUDE_DIRS}
           ${PANGO_INCLUDE_DIRS}
           
)

target_link_libraries( "${EXE_NAME}" cinder${CINDER_LIB_SUFFIX} ${HARFBUZZ_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGO_LIBRARIES} )
