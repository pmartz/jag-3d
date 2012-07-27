# Find CppDOM
# Find the native CppDOM includes and library
# This module defines
#  CppDOM_INCLUDE_DIRS
#  CppDOM_LIBRARIES, libraries to link against to use CppDOM.
#  CppDOM_VERSION, package version in CMake decimal version# format (e.g., 3.1.6)
#  CppDOM_FOUND, If false, do not try to use CppDOM.
#
# Suppert environment variable search hint:
#  CppDOM_ROOT, CppDOM install directory root.


# Find the main CppDOM header.
unset( CppDOM_INCLUDE_DIRS )
find_path( CppDOM_INCLUDE_DIRS cppdom/cppdom.h )


# Set CppDOM_VERSION.
# Also set _libVersionString, which will be appended to the lib name.
set( CppDOM_VERSION )
if( CppDOM_INCLUDE_DIRS )
    set( _versionFile "${CppDOM_INCLUDE_DIRS}/cppdom/version.h" )
    if( NOT EXISTS ${_versionFile} )
        message( SEND_ERROR "Can't find ${_versionFile}" )
    else()
        file( READ "${_versionFile}" _versionContents )
        string( REGEX REPLACE ".*#define CPPDOM_VERSION_MAJOR[ \t]+([0-9]+).*CPPDOM_VERSION_MINOR[ \t]+([0-9]+).*CPPDOM_VERSION_PATCH[ \t]+([0-9]+).*"
            "-\\1_\\2_\\3" _libVersionString ${_versionContents} )

        # Create CppDOM_VERSION from _libVersionString
        # _libVersionString is in form "-<M>_<m>_<p>"
        #    but we need CppDOM_VERSION in form "<M>.<m>.<p>".
        string( REGEX REPLACE "-([0-9]+)_([0-9]+)_([0-9]+)"
            "\\1.\\2.\\3" CppDOM_VERSION ${_libVersionString} )

#        message( STATUS "versions: ${_libVersionString} ${CppDOM_VERSION}" )
    endif()
endif()


# Find the library.
unset( CppDOM_LIBRARIES )
unset( "CppDOM_LIBRARY" CACHE )
set( _libName cppdom${_libVersionString} )
find_library( CppDOM_LIBRARY
    NAMES ${_libName}
    PATH_SUFFIXES lib64
    ENV Vrj_ROOT
)
if( NOT CppDOM_LIBRARY )
    message( WARNING "Could not find library ${_libName}" )
else()
    list( APPEND CppDOM_LIBRARIES "optimized" ${CppDOM_LIBRARY} )
    mark_as_advanced( CppDOM_LIBRARY )
endif()

unset( "CppDOM_LIBRARY_DEBUG" CACHE )
set( _libName cppdom_d${_libVersionString} )
find_library( CppDOM_LIBRARY_DEBUG
    NAMES ${_libName}
    PATH_SUFFIXES lib64
    ENV Vrj_ROOT
)
if( NOT CppDOM_LIBRARY_DEBUG )
    message( WARNING "Could not find library ${_libName}" )
else()
    list( APPEND CppDOM_LIBRARIES "debug" ${CppDOM_LIBRARY_DEBUG} )
    mark_as_advanced( CppDOM_LIBRARY_DEBUG )
endif()


# handle the QUIETLY and REQUIRED arguments and set CppDOM_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CppDOM
    REQUIRED_VARS CppDOM_INCLUDE_DIRS CppDOM_LIBRARIES
    VERSION_VAR CppDOM_VERSION
)


mark_as_advanced(
    CppDOM_INCLUDE_DIRS
    CppDOM_LIBRARIES
    CppDOM_VERSION
)
