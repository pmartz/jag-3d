# Copyright

set( _demoIncludes
    ${PROJECT_SOURCE_DIR}/include/demoSupport
)

set( _requiredDependencyIncludes
    ${GGT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)

set( _requiredDependencyLibraries
    ${GLEW_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
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
        ${_demoIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_exeName}
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
        ${Freeglut_INCLUDE_DIR}
        ${_demoIncludes}
        ${_requiredDependencyIncludes}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()
    target_link_libraries( ${_exeName}
        ${Freeglut_LIBRARIES}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_exeName} )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )
endmacro()

