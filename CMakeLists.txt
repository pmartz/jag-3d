#
# Jag3D top-level CMakeLists.txt
# http://jag-3d.googlecode.com
#

cmake_minimum_required( VERSION 2.8.5 )
project( Jag3D )

# Define project-specific macros.
set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH} )
include( CMakeMacros )


# Define JAG version. Used to configure_file jag/base/Version.h.
set( JAG3D_MAJOR_VERSION 0 )
set( JAG3D_MINOR_VERSION 1 )
set( JAG3D_SUB_VERSION 16 )
_zeroPad( JAG3D_MINOR_VERSION __minorPad )
_zeroPad( JAG3D_SUB_VERSION __patchPad )
set( JAG3D_VERSION ${JAG3D_MAJOR_VERSION}.${__minorPad}.${__patchPad} )


# Installation directories
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )


if( WIN32 AND MSVC )
    # Use folders in Visual Studio. This will cause warnings in VS Express,
    # but there is no way to differentiate between VS SKUs in CMake.
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )

    # Global warning disable.
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

    # Common debug postfix for Windows.
    set( CMAKE_DEBUG_POSTFIX d )
endif()

if( UNIX )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x" )

    # Turn on C++ 2011 support.
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
endif()

option( JAG3D_USE_GLES3 "Build with GLES 3 Support" OFF )

if( APPLE )
    option( JAG3D_TARGET_IOS "Build for iOS device" OFF )
    if( JAG3D_TARGET_IOS )
        set( IOS_SDK 7.0 )
        set( IOS_MIN_SDK 7.0 )
        set( IOS_DEVROOT "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer" )
        set( IOS_SDKROOT "${IOS_DEVROOT}/SDKs/iPhoneOS${IOS_SDK}.sdk" )

        set( CMAKE_OSX_ARCHITECTURES "armv7;armv7s;arm64" CACHE STRING "Build architectures for iOS" FORCE )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -miphoneos-version-min=${IOS_MIN_SDK} -pipe -no-cpp-precomp" CACHE STRING "Flags used by the compiler during all build types." FORCE )

        # Here we set the specific iphone sdk version. We can only set either device or simulator sdk. So if you want both you currently have to have two seperate projects
        set( CMAKE_OSX_SYSROOT "${IOS_SDKROOT}" CACHE STRING "System root for iOS" FORCE )
        # Hack, force link to opengles
        #set( CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework OpenGLES”)
        include( FindPkgMacros )
    endif()
endif()

#
# Dependencies
#

#
# Required dependencies:
#   OpenGL
find_package( OpenGL REQUIRED )
#   GL3 header file
find_package( GL3 REQUIRED )
#   Boost
set( _boostComponents thread program_options date_time chrono system )
find_package( Boost 1.44.0 REQUIRED ${_boostComponents} )
#   Poco
include( FindPOCOHelper )
POCOMenuSetup()
POCOFinder()
if( APPLE )
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    mark_as_advanced(COREFOUNDATION_LIBRARY)
endif()
#   GMTL / GGT
if( NOT GMTL_FOUND )
    find_package( GMTL REQUIRED )
endif()

#
#   Window system - At least one is required for demos/tests/examples:
option( JAG3D_USE_FREEGLUT "If set and freeglut is found, Jag3D builds windowed executables with freeglut." ON )
if( JAG3D_USE_FREEGLUT )
    include( FindFreeglutHelper )
    FreeglutMenuSetup()
    FreeglutFinder()
endif()
option( JAG3D_USE_QT "If set and Qt is found, Jag3D builds windowed executables with Qt." ON )
if( JAG3D_USE_QT )
    find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL )
endif()
option( JAG3D_USE_VRJ "If set and VR Juggler is found, Jag3D builds windowed executables with VR Juggler." ON )
if( JAG3D_USE_VRJ )
    find_package( VRJuggler-OpenGL 3.0 )
endif()
#   Append window system name suffix to executables?
option( JAG3D_ENABLE_WINDOW_SYSTEM_SUFFIXES "If enabled, executable names are suffices with the window system (-fg, -qt, or -vrj)" on )

#   Required dependencies:
if( JAG3D_USE_GLES3 )
    find_package( OpenGLES3 REQUIRED )
    #find_package( AGL, REQUIRED )
else()
    find_package( OpenGL REQUIRED )
    find_package( GL3 REQUIRED )
endif()
find_package( Boost 1.44.0 REQUIRED ${_boostComponents} )
include( FindPOCOHelper )
if( NOT GMTL_FOUND )
    find_package( GMTL REQUIRED )
endif()
#
# Optional OSG - Used for model and image loading plugins
if( NOT JAG3D_USE_OSG )
    # By default, use dynamic-linked OSG. Set to 'none' to disable all use of OSG.
    set( JAG3D_USE_OSG "dynamic" CACHE STRING "Set to 'dynamic' or 'static' for OSG-based plugins, or set to 'none' to not build OSG-based plugins." )
endif()
#   Pulldown menu for cmake-gui.
set_property( CACHE JAG3D_USE_OSG PROPERTY STRINGS "dynamic" "static" "none" )
#   Must be at top level for static links.

include( FindOSGHelper )

set( JAG3D_USE_OPENSCENEGRAPH )
if( NOT JAG3D_USE_OSG STREQUAL "none" )
    # For jag/base/Config.h
    set( JAG3D_USE_OPENSCENEGRAPH TRUE )
endif()
if( OSG_FOUND AND ( JAG3D_USE_OSG STREQUAL "static" ) )
    # Controls how OSG header files define exports.
    add_definitions( -DOSG_LIBRARY_STATIC )
endif()
#   Set defaults for OSG support in osgModel and osgImage loader.
set( JAG3D_OSGIMAGE_PLUGIN_SUPPORT "bmp;gif;jpg;jpeg;png;rgb;rgba;tga;tif;tiff" CACHE STRING "List of osgImage plugin supported extensions" )
set( JAG3D_OSGMODEL_PLUGIN_SUPPORT "3ds;flt;obj;osg;ive;osgt;osgb;ply" CACHE STRING "List of osgModel plugin supported extensions" )

#
# Optional ASSIMP - used for model loading plugin.
option( JAG3D_USE_ASSIMP "If set, build the ASSIMP-based model loading plugin." ON )
if( JAG3D_USE_ASSIMP )
    find_package( ASSIMP REQUIRED )
endif()
set( JAG3D_ASSIMPMODEL_PLUGIN_SUPPORT
    "3d;3ds;ac;ase;blend;dae;dxf;irrmesh;lwo;lxo;m3;md3;md5;mdl;ms3d;obj;off;ply;q3d;scn;smd;stl;terx;xgl;xml"
    CACHE STRING "List of osgImage plugin supported extensions" )

#
# Optional Directx SDK - for DirectInput game pad support.
set( DIRECTINPUT_ROOT_DIR "" CACHE PATH "DirectInput installation directory" )
option( DIRECTINPUT_ARCH_x64 "Select for 64-bit target architectures, unselect for 32-bit." ON )
find_package( DirectInput )
set( JAG3D_USE_DIRECTINPUT )
if( DIRECTINPUT_FOUND )
    set( JAG3D_USE_DIRECTINPUT TRUE )
    add_definitions( -DDIRECTINPUT_ENABLED )
endif()

#
# Optional Doxygen, for documentation.
# add a custom target to build documentation.
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


# Use Qt...
if( QT_FOUND )
    include( ${QT_USE_FILE} )
endif()

# Link with Boost dynamic libs...
if( BOOST_ALL_DYN_LINK )
    add_definitions( -DBOOST_ALL_DYN_LINK )
    set( USE_BOOST_DEFINITION "add_definitions( -DBOOST_ALL_DYN_LINK )" )
endif()

# Use GL3W for OpenGL function pointers...
if( WIN32 )
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." ON )
else()
    option( JAG3D_USE_GL3W "If set, use gl3w (GL3/4 replacement for GLEW). Otherwise, assume native GL3/4 support." OFF )
endif()
if( JAG3D_USE_GL3W )
    option( JAG3D_USE_GL3W_AUTOGENERATE "If set, gl3w source will be automatically regenerated as needed." OFF )
    if( JAG3D_USE_GL3W_AUTOGENERATE )
        find_package( PythonInterp REQUIRED )
    endif()
endif()

option( JAG3D_ENABLE_ERROR_CHECKS "If set, Jag3D enables internal OpenGL error checking." ON )

option( JAG3D_ENABLE_LOGGING "If set, Jag3D enables console and/or log file writes." ON )

option( JAG3D_ENABLE_PROFILING "If set, Jag3D enables profiling." OFF )

set( JAG3D_STATIC )
option( BUILD_SHARED_LIBS "Set to ON to build JAG3D for dynamic linking. Use OFF for static." ON )
if( NOT BUILD_SHARED_LIBS )
    set( JAG3D_STATIC TRUE )
endif()


#
# Define include directories and libraries for compiling
# and linking. These are referenced by the macros in
# CMakeMacros.cmake for creating executables and libraries.
#

set( _requiredDependencyIncludes
    ${POCO_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${GMTL_INCLUDE_DIR}
    ${GL3_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)

set( _projectIncludes
    ${PROJECT_SOURCE_DIR}/src
)

set( _requiredDependencyLibraries
    ${POCO_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
)
if( APPLE )
    list( APPEND _requiredDependencyLibraries
        ${COREFOUNDATION_LIBRARY}
    )
endif()

set( _projectLibraries
    jagMx
    jagUtil
    jagSG
    jagDraw
    jagDisk
    jagBase
)

unset( _exeStaticLibs )
if( NOT BUILD_SHARED_LIBS )
    # If building static, all executables must link again
    # all jag plugins.
    set( _exeStaticLibs jagp-shader jagp-text )
    if( ASSIMP_FOUND )
        list( APPEND _exeStaticLibs jagp-assimpModel )
    endif()
    if( OSG_FOUND )
        list( APPEND _exeStaticLibs jagp-osgImage )
        list( APPEND _exeStaticLibs jagp-osgModel )
        if( JAG3D_USE_OSG STREQUAL "static" )
            # OSG was also built static. Link against all OSG plugins.
            _findOSGPlugins()
            list( APPEND _exeStaticLibs ${OSG_PLUGIN_LIBRARIES} )
            list( APPEND _exeStaticLibs ${OSG_LIBRARIES} )
        endif()
    endif()
    if( DIRECTINPUT_FOUND )
        list( APPEND _exeStaticLibs ${DIRECTINPUT_LIBRARIES} )
    endif()
endif()


#
# Options to control what to build.
option( JAG3D_BUILD_PLUGINS "Enable to build plugins" ON )
option( JAG3D_BUILD_TESTS "Enable to build test code" ON )
option( JAG3D_BUILD_UNITTESTS "Enable to build unit test code" OFF )
option( JAG3D_BUILD_APPS "Enable to build applications" ON )
option( JAG3D_BUILD_EXAMPLES "Enable to build example code" ON )

if( JAG3D_BUILD_UNITTESTS )
    # CMake kludge: enable_testing() must be in source directory root.
    enable_testing()
endif()



#
# Recursively process the src directory to add all library,
# plugin, and executable targets.
add_subdirectory( src )

# If installing data, also add the data directory.
option( JAG3D_INSTALL_DATA "Enable to add the data directory to the install target" OFF )
if( JAG3D_INSTALL_DATA )
    add_subdirectory( data )
endif()


#
# Install pdb files for Debug and RelWithDebInfo builds
if( MSVC )
    install(
        DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/\${CMAKE_INSTALL_CONFIG_NAME}/
        DESTINATION lib
        USE_SOURCE_PERMISSIONS
        COMPONENT libjag3d
        FILES_MATCHING PATTERN "*.pdb"
    )
endif()

#
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
