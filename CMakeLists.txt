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

macro( zeroPad _input _output )
    if( ${${_input}} LESS 10 )
        set( ${_output} "0${${_input}}" )
    else()
        set( ${_output} "${${_input}}" )
    endif()
endmacro()

set( JAG3D_MAJOR_VERSION 0 )
set( JAG3D_MINOR_VERSION 0 )
set( JAG3D_SUB_VERSION 16 )
zeroPad( JAG3D_MINOR_VERSION __minorPad )
zeroPad( JAG3D_SUB_VERSION __patchPad )
set( JAG3D_VERSION ${JAG3D_MAJOR_VERSION}.${__minorPad}.${__patchPad} )


set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )


if( WIN32 AND MSVC )
    # Disable warnings
    # C4251: <membervar> : class <classX> needs to have dll-interface to be used by clients of class <classY>
    # C4275: non dll-interface class <classX> used as base for dll-interface class <classY>
    foreach( warning 4251 4275 )
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

set( _boostComponents )
list( APPEND _boostComponents thread )
list( APPEND _boostComponents program_options )
list( APPEND _boostComponents date_time )
list( APPEND _boostComponents chrono )
list( APPEND _boostComponents system )

#
#   At least one windowing API is required for demos/tests/examples:
option( JAG3D_USE_FREEGLUT "If set and freeglut is found, Jag3D builds windowed executables with freeglut." ON )
if( JAG3D_USE_FREEGLUT )
    include( FindFreeglutHelper )
endif()
option( JAG3D_USE_QT "If set and Qt is found, Jag3D builds windowed executables with Qt." ON )
if( JAG3D_USE_QT )
    find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL )
endif()
option( JAG3D_USE_VRJ "If set and VR Juggler is found, Jag3D builds windowed executables with VR Juggler." ON )
if( JAG3D_USE_VRJ )
    find_package( VRJuggler-OpenGL 3.0 )
endif()

#   Required dependencies:
find_package( OpenGL REQUIRED )
find_package( GL3 REQUIRED )
find_package( Boost 1.44.0 REQUIRED ${_boostComponents} )
include( FindPOCOHelper )
# include( FindGGTHelper )
if( NOT GMTL_FOUND )
    find_package( GMTL REQUIRED )
endif()

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    mark_as_advanced(COREFOUNDATION_LIBRARY)
endif(APPLE)


if( JAG3D_USE_FREEGLUT )
    FreeglutMenuSetup()
endif()
# if( JAG3D_USE_VRJ )
#     CppDOMMenuSetup()
#     VrjMenuSetup()
# endif()
# GGTMenuSetup()
POCOMenuSetup()

if( JAG3D_USE_FREEGLUT )
    FreeglutFinder()
endif()
# if( JAG3D_USE_VRJ )
#     CppDOMFinder()
#     VrjFinder()
# endif()
# GGTFinder()
POCOFinder()

#
# Eventually, this will be included only in the src/plugins/CMakeLists.txt.
# For now, do it here, so that the jagload test can leverage it.
include( FindOSGHelper )

# find the optional Directx SDK (for DirectInput game pad support).
set( DIRECTINPUT_ROOT_DIR "" CACHE PATH "DirectInput installation directory" )
option( DIRECTINPUT_ARCH_x64 "Select for 64-bit target architectures, unselect for 32-bit." ON )
find_package( DirectInput )

if( DIRECTINPUT_FOUND )
    add_definitions( -DDIRECTINPUT_ENABLED )
endif()


if( QT_FOUND )
    include( ${QT_USE_FILE} )
else()
    if( JAG3D_USE_QT AND QT_QMAKE_EXECUTABLE AND NOT QT4_QGLOBAL_H_FILE )
        message( STATUS "Qt was not found because the qglobal.h file was not found." )
    endif()
endif()

# Must use Boost as dynamic libs.
add_definitions( -DBOOST_ALL_DYN_LINK )

if( WIN32 )
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." ON )
else()
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." OFF )
endif()
if( JAG3D_USE_GL3W )
    set( JAG3D_USE_GL3W_DEFINITION "#define JAG3D_USE_GL3W 1" )

    option( JAG3D_USE_GL3W_AUTOGENERATE "If set, gl3w source will be automatically regenerated as needed." OFF )
    if( JAG3D_USE_GL3W_AUTOGENERATE )
        find_package( PythonInterp REQUIRED )
    endif()
else()
    set( JAG3D_USE_GL3W_DEFINITION "#undef JAG3D_USE_GL3W" )
endif()


# See src/jagBase/Config.h.in
option( JAG3D_ENABLE_ERROR_CHECKS "If set, Jag3D enables internal OpenGL error checking." ON )
if( JAG3D_ENABLE_ERROR_CHECKS )
    set( JAG3D_ERROR_ENABLE_DEFINITION "#define JAG3D_ENABLE_ERROR_CHECKS 1" )
else()
    set( JAG3D_ERROR_ENABLE_DEFINITION "#undef JAG3D_ENABLE_ERROR_CHECKS" )
endif()

# See src/jagBase/Config.h.in
option( JAG3D_ENABLE_LOGGING "If set, Jag3D enables console and/or log file writes." ON )
if( JAG3D_ENABLE_LOGGING )
    set( JAG3D_LOG_ENABLE_DEFINITION "#define JAG3D_ENABLE_LOGGING 1" )
else()
    set( JAG3D_LOG_ENABLE_DEFINITION "#undef JAG3D_ENABLE_LOGGING" )
endif()

# See src/jagBase/Config.h.in
option( JAG3D_ENABLE_PROFILING "If set, Jag3D enables profiling." OFF )
if( JAG3D_ENABLE_PROFILING )
    set( JAG3D_PROFILE_ENABLE_DEFINITION "#define JAG3D_ENABLE_PROFILING 1" )
else()
    set( JAG3D_PROFILE_ENABLE_DEFINITION "#undef JAG3D_ENABLE_PROFILING" )
endif()

option( BUILD_SHARED_LIBS "Set to ON to build JAG3D for dynamic linking. Use OFF for static." ON )
if( NOT BUILD_SHARED_LIBS )
    add_definitions( -DJAG3D_STATIC )
endif()


include( CMakeMacros )


option( JAG3D_BUILD_PLUGINS "Enable to build plugins" ON )
option( JAG3D_BUILD_TESTS "Enable to build test code" ON )
option( JAG3D_BUILD_UNITTESTS "Enable to build unit test code" OFF )
option( JAG3D_BUILD_APPS "Enable to build applications" ON )
option( JAG3D_BUILD_EXAMPLES "Enable to build example code" ON )

if( JAG3D_BUILD_UNITTESTS )
    # CMake kludge: enable_testing() must be in source directory root.
    enable_testing()
endif()


add_subdirectory( src )


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

#install pdb files for release with debug info builds
if( MSVC )
    install(
        DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/\${CMAKE_INSTALL_CONFIG_NAME}/
        DESTINATION lib
        USE_SOURCE_PERMISSIONS
        COMPONENT libjag3d
        FILES_MATCHING PATTERN "*.pdb"
    )
endif()


# Install the CMake find script
install( FILES "extra/Findjag3d.cmake"
    DESTINATION "share/${CMAKE_PROJECT_NAME}/extra"
    COMPONENT libjag3d
)


#
# Create and install all of the auto find tools
# =============================================

# Add all targets to the build-tree export set
export( TARGETS jagMx jagUtil jagSG jagDraw jagDisk jagBase
  FILE "${PROJECT_BINARY_DIR}/lib/jag3dLibraryDepends.cmake" )

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export( PACKAGE jag3d )

# Create a jag3dBuildTreeSettings.cmake file for the use from the build tree
configure_file( CMakeModules/jag3dBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/lib/jag3dBuildTreeSettings.cmake" @ONLY )

configure_file( CMakeModules/jag3dConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/lib/jag3dConfigVersion.cmake" @ONLY )
configure_file( CMakeModules/jag3dUsejag3d.cmake.in
  "${PROJECT_BINARY_DIR}/lib/Usejag3d.cmake" @ONLY )

# Install the export set for use with the install-tree
install( EXPORT jag3d-targets DESTINATION lib )

include( CMakePackageConfigHelpers )
# Create the jag3dConfig.cmake and jag3dConfigVersion files
file( RELATIVE_PATH CONF_REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/lib"
   "${CMAKE_INSTALL_PREFIX}/include" )

configure_package_config_file( 
    CMakeModules/jag3dConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/lib/jag3dConfig.cmake
    INSTALL_DESTINATION lib
    PATH_VARS CONF_REL_INCLUDE_DIR )

install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib/jag3dConfig.cmake 
    ${CMAKE_CURRENT_BINARY_DIR}/lib/jag3dConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lib/Usejag3d.cmake
    DESTINATION lib )


#
# Setup the fpc auto detection files
if( WIN32 )
    set( JAG3D_FPC_LIBRARIES "/libpath:\"\${libdir}\" jagSG.lib jagDraw.lib jagDisk.lib jagBase.lib" )
    set( JAG3D_FPC_INCLUDES "/I\"\${includedir}\""  )
else()
    set( JAG3D_FPC_INCLUDES "-I\${includedir}" )
    set( JAG3D_FPC_LIBRARIES "-L\${libdir} -ljagSG -ljagDraw -ljagDisk -ljagBase" )
endif()

configure_file( extra/jag3d.fpc.in
  "${PROJECT_BINARY_DIR}/lib/flagpoll/jag3d.fpc" @ONLY )

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/flagpoll/jag3d.fpc
        DESTINATION lib/flagpoll )
