# Find VR Juggler
# Find the native VR Juggler includes and libraries
# This module defines
#  Vrj_INCLUDE_DIRS
#  Vrj_LIBRARIES, libraries to link against to use VR Juggler.
#  Vrj_VERSION, package version in CMake decimal version# format (e.g., 3.1)
#  Vrj_FOUND, If false, do not try to use VR Juggler.
#
# Suppert environment variable search hint:
#  Vrj_ROOT, VR Juggler install directory root.


# Find the main Vrj header.
unset( Vrj_INCLUDE_DIRS )
find_path( Vrj_INCLUDE_DIRS vrj/vrjDefines.h )


# Error if we didn't find VRJ's GL3 branch.
if (Vrj_INCLUDE_DIRS )
    set( _gl3File "${Vrj_INCLUDE_DIRS}/vrj/Draw/OpenGL/GL3/gl3.h" )
    if( NOT EXISTS ${_gl3File} )
        message( STATUS "Can't find ${_gl3File}" )
        message( SEND_ERROR "JAG3D requires the GL3 branch of VR Juggler." )
    endif()
endif()


# Set Vrj_VERSION.
set( Vrj_VERSION )
if( Vrj_INCLUDE_DIRS )
    set( _versionFile "${Vrj_INCLUDE_DIRS}/vrj/vrjDefines.h" )
    if( NOT EXISTS ${_versionFile} )
        message( SEND_ERROR "Can't find ${_versionFile}" )
    else()
        file( READ "${_versionFile}" _versionContents )
        string( REGEX REPLACE ".*#define PACKAGE_VERSION[ \t]+\"([0-9.]+).*"
            "\\1" _version ${_versionContents} )
    endif()
endif()


# Get a list of requested Vrj components. 'vrj' is assumed as always requested.
#   vrj gadget jccl sonix vpr
set( _requestedComponents )
foreach( _component ${Vrj_FIND_COMPONENTS})
    list( APPEND _requestedComponents ${_component} )
endforeach()
list( APPEND _requestedComponents "vrj-3_1_6" )
list( REMOVE_DUPLICATES _requestedComponents )

# Find each library.
set( Vrj_LIBRARIES )
foreach( lib ${_requestedComponents} )
    unset( "Vrj_${lib}_LIBRARY" CACHE )
    find_library( Vrj_${lib}_LIBRARY
        NAMES ${lib}
        ENV Vrj_ROOT
    )
    if( NOT Vrj_${lib}_LIBRARY )
        message( WARNING "Could not find Vrj component library ${lib}" )
    else()
        list( APPEND Vrj_LIBRARIES "optimized" ${Vrj_${lib}_LIBRARY} )
        mark_as_advanced( Vrj_${lib}_LIBRARY )
    endif()

    unset( "Vrj_${lib}_LIBRARY_DEBUG" CACHE )
    find_library( Vrj_${lib}_LIBRARY_DEBUG
        NAMES ${lib}
        PATH_SUFFIXES debug
        ENV Vrj_ROOT
    )
    if( NOT Vrj_${lib}_LIBRARY_DEBUG )
        message( WARNING "Could not find Vrj component Debug library ${lib}" )
    else()
        list( APPEND Vrj_LIBRARIES "debug" ${Vrj_${lib}_LIBRARY_DEBUG} )
        mark_as_advanced( Vrj_${lib}_LIBRARY_DEBUG )
    endif()
endforeach()


# handle the QUIETLY and REQUIRED arguments and set Vrj_FOUND to TRUE if 
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Vrj
    REQUIRED_VARS Vrj_INCLUDE_DIRS Vrj_LIBRARIES
    VERSION_VAR Vrj_VERSION
)


mark_as_advanced(
    Vrj_INCLUDE_DIRS
    Vrj_LIBRARIES
    Vrj_VERSION
)
