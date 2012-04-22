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


set( _optionalDependencyIncludes )
set( _optionalDependencyLibraries )

# Using gl3w, not GLEW. See top-level CMakeLists.txt for info.
# if( GLEW_FOUND )
#     list( APPEND _optionalDependencyIncludes
#         ${GLEW_INCLUDE_DIR}
#     )
#     list( APPEND _optionalDependencyLibraries
#         ${GLEW_LIBRARIES}
#     )
# endif()

set( _requiredDependencyIncludes
    ${POCO_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${GGT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)
set( _projectIncludes
    ${PROJECT_SOURCE_DIR}/include
)

set( _requiredDependencyLibraries
    ${POCO_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
)
set( _projectLibraries
    jagDraw
    jagBase
)


# Given an input token _tokenIn, return all the elements
# in ARGN preceding _tokenIn in _list0out, and all the elements
# following _tokenIn in _list1out. If _tokenIn is not present in _listIn,
# _list0out will be identical to ARGN, and _list1out will be empty.
#
macro( _splitList _tokenIn _list0out _list1out )
    set( ${_list0out} )
    set( ${_list1out} )
    set( tokenFound 0 )

    foreach( element ${ARGN} )
        if( tokenFound )
            list( APPEND ${_list1out} ${element} )
        else()
            if( ${element} STREQUAL ${_tokenIn} )
                set( tokenFound 1 )
            else()
                list( APPEND ${_list0out} ${element} )
            endif()
        endif()
    endforeach()
endmacro()


# Support installing non-app executables into the share/jag3d/bin directory.
#
macro( _exeInstall _category _exeName )
    if( _category STREQUAL "App" )
        set( _installDir bin )
    else()
        set( _installDir share/${CMAKE_PROJECT_NAME}/bin )
    endif()
    install(
        TARGETS ${_exeName}
        RUNTIME DESTINATION _installDir COMPONENT libjag3d
    )
endmacro()


macro( _addNonWindowedExecutable _category _exeName )
    add_executable( ${_exeName} ${ARGN} )

    include_directories(
        ${_projectIncludes}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_exeName}
        ${_projectLibraries}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )

    _exeInstall( ${_category} ${_exeName} )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )
endmacro()

macro( _addFreeglutExecutable _category _exeName )
    set( _localExeName "${_exeName}-freeglut" )

    _splitList( ADDITIONAL_LIBRARIES sources libs ${ARGN} )

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/freeglutSupport.cpp
        ${sources}
    )

    include_directories(
        ${_projectIncludes}
        ${Freeglut_INCLUDE_DIR}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_localExeName}
        ${libs}
        ${Freeglut_LIBRARIES}
        ${_projectLibraries}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )
endmacro()

macro( _addQtExecutable _category _exeName )
    set( _localExeName "${_exeName}-qt" )

    QT4_WRAP_CPP( _mocFiles
        ${PROJECT_SOURCE_DIR}/include/demoSupport/qtGlWidget.h
        OPTIONS "-f"
    )

    _splitList( ADDITIONAL_LIBRARIES sources libs ${ARGN} )

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/qtSupport.cpp
        ${_mocFiles}
        ${sources}
    )

    include_directories(
        ${_projectIncludes}
        ${QT_QTOPENGL_INCLUDE_DIR}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_localExeName}
        ${libs}
        ${QT_QTOPENGL_LIBRARY}
        ${QT_LIBRARIES}
        ${_projectLibraries}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )
endmacro()

macro( _addExecutable _category _exeName )
    if( Freeglut_FOUND )
        _addFreeglutExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
    if( QT4_FOUND )
        _addQtExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
endmacro()



macro( _addLibrary _libName )
    include_directories(
        ${_projectIncludes}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )

    _splitList( JAG_LIBRARIES sources libs ${ARGN} )

    if( BUILD_SHARED_LIBS )
        add_library( ${_libName} SHARED ${sources} )
    else()
        add_library( ${_libName} ${sources} )
    endif()

    target_link_libraries( ${_libName}
        ${libs}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )

    set_target_properties( ${_libName} PROPERTIES VERSION ${JAG3D_VERSION} )
    set_target_properties( ${_libName} PROPERTIES SOVERSION ${JAG3D_VERSION} )
    set_target_properties( ${_libName} PROPERTIES PROJECT_LABEL "Lib ${_libName}" )

    include( ModuleInstall REQUIRED )
endmacro()

