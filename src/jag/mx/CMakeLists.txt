set( _libName jagMx )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGMX_LIBRARY )
endif()


set( _dxHeaders )
set( _dxSources )
set( _dxLibrary )
if( DIRECTINPUT_FOUND )
    add_definitions(-DDIRECTINPUT_ENABLED)
    include_directories( ${DIRECTINPUT_INCLUDE_DIR} )
    set( _dxHeaders
        MxGamePadDX.h
    )
    set( _dxSources
        MxGamePadDX.cpp
    )
    set( _dxLibrary ${DIRECTINPUT_LIBRARIES} )
endif()

set( _jagMxHeaderFiles
    Export.h
    ${_dxHeaders}
    MxCore.h
    MxGamePad.h
    MxUtils.h
)
#    MxEventHandler.h
#)

set( _jagMxSourceFiles
    ${_dxSources}
    MxCore.cpp
    MxGamePad.cpp
    MxUtils.cpp
)
#    MxEventHandler.cpp
#)

_addLibrary( ${_libName}
    ${_jagMxHeaderFiles}
    ${_jagMxSourceFiles}
    JAG_LIBRARIES
        jagBase
        ${_dxLibrary}
)

install(
    DIRECTORY .
    DESTINATION include/jag/mx
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
