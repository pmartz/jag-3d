# - Find ASSIMP
# Find the native ASSIMP includes and libraries
# This module defines
#  ASSIMP_INCLUDE_DIRS, where to find assimp/scene.h, etc.
#  ASSIMP_LIBRARIES, libraries to link against to use ASSIMP.
#  ASSIMP_VERSION, version in CMake decimal version# format (e.g., 1.4.1.1)
#  ASSIMP_FOUND, If false, do not try to use ASSIMP.
#
# Note that if specifying <version> in find_package(), use the CMake format
# rather than the ASSIMP format (i.e., use "1.4.2.1" instead of "1.4.2p1").
#
# Search hint (in CMake or as an environment variable):
#  ASSIMP_ROOT, the ASSIMP install directory root.



# Find the main ASSIMP header.
set( ASSIMP_INCLUDE_DIRS )
find_path( ASSIMP_INCLUDE_DIRS assimp/scene.h
    PATHS ${ASSIMP_ROOT} ENV ASSIMP_ROOT
    PATH_SUFFIXES include
)


# Find each library.
set( _requestedComponents assimp )
set( ASSIMP_LIBRARIES )
foreach( lib ${_requestedComponents} )
    find_library( ASSIMP_${lib}_LIBRARY
        NAMES ${lib}
        PATHS ${ASSIMP_ROOT} ENV ASSIMP_ROOT
        PATH_SUFFIXES lib lib/assimp_release-dll_x64
    )
    find_library( ASSIMP_${lib}_LIBRARY_DEBUG
        NAMES ${lib}
        PATHS ${ASSIMP_ROOT} ENV ASSIMP_ROOT
        PATH_SUFFIXES lib lib/assimp_debug-dll_x64
    )

    if( NOT ASSIMP_${lib}_LIBRARY )
        message( WARNING "Could not find ASSIMP component library ${lib}" )
    else()
        if( ASSIMP_${lib}_LIBRARY_DEBUG AND
                ( NOT ASSIMP_${lib}_LIBRARY_DEBUG STREQUAL ASSIMP_${lib}_LIBRARY ) )
            list( APPEND ASSIMP_LIBRARIES "optimized" ${ASSIMP_${lib}_LIBRARY} )
            list( APPEND ASSIMP_LIBRARIES "debug" ${ASSIMP_${lib}_LIBRARY_DEBUG} )
        else()
            list( APPEND ASSIMP_LIBRARIES ${ASSIMP_${lib}_LIBRARY} )
        endif()
        mark_as_advanced( ASSIMP_${lib}_LIBRARY )
        mark_as_advanced( ASSIMP_${lib}_LIBRARY_DEBUG )
    endif()
endforeach()


# handle the QUIETLY and REQUIRED arguments and set ASSIMP_FOUND to TRUE if 
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( ASSIMP
    REQUIRED_VARS ASSIMP_INCLUDE_DIRS ASSIMP_LIBRARIES
    VERSION_VAR ASSIMP_VERSION
)


mark_as_advanced(
    ASSIMP_INCLUDE_DIR
    ASSIMP_LIBRARIES
    ASSIMP_VERSION
)
