# Copyright

set( _demoIncludes
    ${PROJECT_SOURCE_DIR}/include/demoSupport
)

set( _requiredDependencyIncludes
    ${GGT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)
set( _projectIncludes
    ${PROJECT_SOURCE_DIR}/include
)

set( _requiredDependencyLibraries
    ${GLEW_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
)
set( _projectLibraries
    jagBase
)


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


macro( _addExecutable _category _exeName )
    add_executable( ${_exeName} ${ARGN} )

    include_directories(
        ${_projectIncludes}
        ${_demoIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_exeName}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )
    _exeInstall( ${_category} ${_exeName} )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )
endmacro()

macro( _addFreeglutExecutable _category _exeName )
    if( NOT Freeglut_FOUND )
        return()
    endif()

    add_executable( ${_exeName} ${ARGN} )

    include_directories(
        ${_projectIncludes}
        ${Freeglut_INCLUDE_DIR}
        ${_demoIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_exeName}
        ${Freeglut_LIBRARIES}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_exeName} )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )
endmacro()



macro( _addLibrary _libName )
    include_directories(
        ${_projectIncludes}
        ${_requiredDependencyIncludes}
    )

    if( BUILD_SHARED_LIBS )
        add_library( ${_libName} SHARED ${ARGN} )
    else()
        add_library( ${_libName} ${ARGN} )
    endif()

    target_link_libraries( ${_libName}
        ${_requiredDependencyLibraries}
    )

    set_target_properties( ${_libName} PROPERTIES VERSION ${JAG3D_VERSION} )
    set_target_properties( ${_libName} PROPERTIES SOVERSION ${JAG3D_VERSION} )
    set_target_properties( ${_libName} PROPERTIES PROJECT_LABEL "Lib ${_libName}" )

    include( ModuleInstall REQUIRED )
endmacro()

