cmake_minimum_required( VERSION 2.8 )
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



find_package( OpenGL REQUIRED )
find_package( Boost )

include( FindFreeglutHelper )
include( FindGGTHelper )
include( FindGLEWHelper )

FreeglutMenuSetup()
GGTMenuSetup()
GLEWMenuSetup()

FreeglutFinder()
GGTFinder()
GLEWFinder()



option( JAG3D_USE_BOOST_POINTERS "If set, and Boost is found, JAG uses Boost shared pointers instead of TR1" OFF )
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
#include( ${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL )
option( JAG3D_DOCUMENTATION "Enable to create build target for Doxygen documentation." OFF )
mark_as_advanced( CLEAR JAG3D_DOCUMENTATION )
if( JAG3D_DOCUMENTATION )
    find_package( Doxygen REQUIRED )
    set( HAVE_DOT "NO" )
    if( DOXYGEN_DOT_PATH )
        set( HAVE_DOT "YES" )
    endif( DOXYGEN_DOT_PATH )

#    configure_file( ${PROJECT_SOURCE_DIR}/doc/doxyfile.cmake
#        ${PROJECT_BINARY_DIR}/doc/doxyfile
#    )
    add_custom_target( Documentation ${DOXYGEN_EXECUTABLE}
        ${PROJECT_BINARY_DIR}/doc/doxyfile
    )
    
#    add_subdirectory( doc/images )
endif()
