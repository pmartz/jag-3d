
set( _demoIncludes
    ${PROJECT_SOURCE_DIR}/include/demoSupport
)


macro( _addExecutable _category _exeName )
    message( STATUS "In _addExecutable ${_exeName}" )
endmacro()

macro( _addFreeglutExecutable _category _exeName )
    if( NOT Freeglut_FOUND )
        return()
    endif()
    message( STATUS "In _addFreeglutExecutable ${_exeName}" )

    add_executable( ${_exeName} ${ARGN} )

    include_directories(
        ${_demoIncludes}
        ${Freeglut_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
    )

    if( WIN32 )
        set( RELATIVE_LIB_PATH ../../../lib/ )
    endif()

    target_link_libraries( ${_exeName}
        ${Freeglut_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
    )
    
    if( _category STREQUAL "App" )
        set( _installDir bin )
    else()
        set( _installDir share/${CMAKE_PROJECT_NAME}/bin )
    endif()
    install(
        TARGETS ${_exeName}
        RUNTIME DESTINATION _installDir COMPONENT libjag3d
    )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )
endmacro()

