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


unset( _optionalDependencyIncludes )
unset( _optionalDependencyLibraries )

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

if(APPLE)
    list(APPEND _requiredDependencyLibraries
        ${COREFOUNDATION_LIBRARY}
    )
endif(APPLE)

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
        elseif( ${element} STREQUAL ${_tokenIn} )
            set( tokenFound 1 )
        else()
            list( APPEND ${_list0out} ${element} )
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
        RUNTIME DESTINATION ${_installDir}
    )
endmacro()


macro( _addNonWindowedExecutable _category _exeName )
    add_executable( ${_exeName} ${ARGN} )

    include_directories(
        ${_projectIncludes}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )

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

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/freeglutSupport.cpp
        ${_sources}
    )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${Freeglut_INCLUDE_DIR}
        ${_includes}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
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
        ${PROJECT_SOURCE_DIR}/src/demoSupport/qtGlWidget.h
        OPTIONS "-f"
    )

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/qtSupport.cpp
        ${_mocFiles}
        ${_sources}
    )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${QT_INCLUDE_DIR}
        ${QT_QTOPENGL_INCLUDE_DIR}
        ${QT_QTGUI_INCLUDE_DIR}
        ${QT_QTCORE_INCLUDE_DIR}
        ${_includes}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
        ${QT_QTOPENGL_LIBRARY}
        ${QT_LIBRARIES}
        ${_projectLibraries}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )
endmacro()

macro( _addVrjExecutable _category _exeName )
    set( _localExeName "${_exeName}-vrj" )

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/vrjSupport.cpp
        ${_sources}
    )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${Vrj_INCLUDE_DIRS}
        ${_includes}
        ${CppDOM_INCLUDE_DIRS}
        ${_optionalDependencyIncludes}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
        ${Vrj_LIBRARIES}
        ${CppDOM_LIBRARIES}
        ${_projectLibraries}
        ${_optionalDependencyLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )
endmacro()

macro( _addExecutable _category _exeName )
    if( Freeglut_FOUND AND JAG3D_USE_FREEGLUT )
        _addFreeglutExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
    if( QT4_FOUND AND JAG3D_USE_QT )
        _addQtExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
    if( VRJ_FOUND AND CPPDOM_FOUND AND JAG3D_USE_VRJ )
        _addVrjExecutable( ${_category} ${_exeName} ${ARGN} )
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

# Supports jag3d plugin info (.jagpi) files, which must reside in the
# same directory as the plugin shared libraries / DLLs. This means
# they must be installed into the bin directory, but also they
# must be copied into the CMake output build tree per config type.
#
macro( _addPluginInfo _name )
    # Copy .jagpi file to development bin directories.
    # NOTE this is done during CMake config.
    foreach( _configDir ${CMAKE_CONFIGURATION_TYPES} )
        configure_file( ${_name}
            ${PROJECT_BINARY_DIR}/bin/${_configDir}/${_name} COPYONLY )
    endforeach()

    # Install .jagpi file to bin directory.
    install( FILES ${_name}
        DESTINATION bin
    )
endmacro()
