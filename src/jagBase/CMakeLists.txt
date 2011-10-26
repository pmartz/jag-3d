set( _libName jagBase )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGBASE_LIBRARY )
endif()


set( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${_libName} )
set( _jagBaseHeaderFiles
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/Buffer.h
    ${HEADER_PATH}/PlatformGLEW.h
    ${HEADER_PATH}/PlatformOpenGL.h
    ${HEADER_PATH}/ptr.h
    ${HEADER_PATH}/Version.h
)

set( _jagBaseSourceFiles
    Version.cpp
)

_addLibrary( ${_libName}
    ${_jagBaseHeaderFiles}
    ${_jagBaseSourceFiles}
)
