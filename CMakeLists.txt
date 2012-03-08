#*************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
#************** <auto-copyright.pl END do not edit this line> ***************

cmake_minimum_required( VERSION 2.8.5 )
project( Jag3D )


# Please keep in sync with include/jagBase/Version.h
set( JAG3D_VERSION 0.00.00 )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
if( WIN32 )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
else()
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
endif()
foreach( CONF ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER "${CONF}" CONF )
    set( "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
    set( "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}" "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" )
    if( WIN32 )
        set( "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
    else()
        set( "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" )
    endif()
endforeach()


if( WIN32 AND MSVC )
    # Disable warnings
    # Use foreach in case we want to disable more than 1 warning in the future.
    foreach( warning 4251 )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}" )
    endforeach()

    # Enable MP, unless specified otherwise
    option( JAG3D_WIN32_USE_MP "Build with multiple processes (/MP option; VS 2005 and above)." ON )
    mark_as_advanced( JAG3D_WIN32_USE_MP )
    if( JAG3D_WIN32_USE_MP )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
    endif()
endif()



#
# Dependencies

#   Required dependencies:
find_package( OpenGL REQUIRED )
find_package( Boost 1.44.0 REQUIRED thread program_options )
include( FindGGTHelper )
include( FindPOCOHelper )

#   Required for Windows only:
#include( FindGLEWHelper )
# NO. Currently using gl3w (https://github.com/skaslev/gl3w/wiki/)
# to work around GLEW's lack of GL3/4 support. gl3w source is
# embedded in include/jagDraw and src/jagDraw; no external
# dependency requirement.
#
# gl3w usage is an option that default to ON for Windows and OFF
# for other platforms.

#   At least one windowing API is required for demos/tests/examples:
find_package( Qt 4.8.0 COMPONENTS QtCore QtGui QtOpenGL )
include( FindFreeglutHelper )

FreeglutMenuSetup()
GGTMenuSetup()
# GLEWMenuSetup()
POCOMenuSetup()

FreeglutFinder()
GGTFinder()
# GLEWFinder()
POCOFinder()



if( QT_FOUND )
    include( ${QT_USE_FILE} )
endif()

# if( GLEW_FOUND )
#     add_definitions( -DJAG3D_USE_GLEW )
# endif()

option( BOOST_ALL_DYN_LINK "If set, use Boost dynamic libraries (boost_*); otherwise static (libboost_*)." OFF )
if( BOOST_ALL_DYN_LINK )
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

if( WIN32 )
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." ON )
else()
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." OFF )
endif()
if( JAG3D_USE_GL3W )
    add_definitions( -DJAG3D_USE_GL3W )
endif()

option( JAG3D_DISABLE_ALL_ERROR_CHECKS "If set, JAG disables all internal OpenGL error checking" OFF )
if( JAG3D_DISABLE_ALL_ERROR_CHECKS )
    add_definitions( -DJAG3D_DISABLE_ALL_ERROR_CHECKS )
endif()

option( JAG3D_USE_BOOST_POINTERS "If set, JAG uses Boost shared pointers instead of TR1" OFF )
if( JAG3D_USE_BOOST_POINTERS AND Boost_FOUND )
    add_definitions( -DJAG3D_USE_BOOST_POINTERS )
endif()

option( BUILD_SHARED_LIBS "Set to ON to build JAG3D for dynamic linking. Use OFF for static." ON )
if( NOT BUILD_SHARED_LIBS )
    add_definitions( -DJAG3D_STATIC )
endif()

include( CMakeMacros )


add_subdirectory( src/jagBase )
add_subdirectory( src/jagDraw )


option( JAG3D_BUILD_TESTS "Enable to build test code" ON )
if( JAG3D_BUILD_TESTS )
    enable_testing()
    add_definitions( -DJAG3D_BUILD_TESTS )
    add_subdirectory( src/tests )
endif()

option( JAG3D_BUILD_APPS "Enable to build applications" OFF )
if( JAG3D_BUILD_APPS )
    add_subdirectory( src/apps )
endif()

option( JAG3D_BUILD_EXAMPLES "Enable to build example code" OFF )
if( JAG3D_BUILD_EXAMPLES )
    add_subdirectory( src/examples )
endif()

option( JAG3D_INSTALL_DATA "Enable to add the data directory to the install target" OFF )
if( JAG3D_INSTALL_DATA )
    add_subdirectory( data )
endif()



#
# Doxygen
option( JAG3D_DOCUMENTATION "Enable to create build target for Doxygen documentation." OFF )
mark_as_advanced( CLEAR JAG3D_DOCUMENTATION )
if( JAG3D_DOCUMENTATION )
    find_package( Doxygen REQUIRED )
    set( HAVE_DOT "NO" )
    if( DOXYGEN_DOT_PATH )
        set( HAVE_DOT "YES" )
    endif( DOXYGEN_DOT_PATH )

    configure_file( ${PROJECT_SOURCE_DIR}/doc/doxyfile.cmake
        ${PROJECT_BINARY_DIR}/doc/doxyfile
    )
    add_custom_target( Documentation ${DOXYGEN_EXECUTABLE}
        ${PROJECT_BINARY_DIR}/doc/doxyfile
    )
    
#    add_subdirectory( doc/images )
endif()
