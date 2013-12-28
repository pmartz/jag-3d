
if( NOT ( Freeglut_FOUND AND JAG3D_USE_FREEGLUT ) )
    return()
endif()

macro( _abufferExecutable _category _exeName )
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
        ${_sources}
    )
    set_property( TARGET ${_localExeName} PROPERTY DEBUG_OUTPUT_NAME "${_localExeName}${CMAKE_DEBUG_POSTFIX}" )

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

    set_property( TARGET ${_localExeName} PROPERTY FOLDER "${_category}/FreeGlut")
endmacro()


_abufferExecutable( Example abuffer
    ABufferGL4.cpp
    ABufferGL4_Basic.cpp
    ABufferGL4_Basic.h
    ABufferGL4_LinkedList.cpp
    ABufferGL4_LinkedList.h
    ABufferGL4Parameters.h
    InfoWidget.h
    Matrix.h
    ShadersManagment.cpp
    ShadersManagment.h
    Vector.h
)
