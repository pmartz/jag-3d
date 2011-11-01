set( _libName jagDraw )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGDRAW_LIBRARY )
endif()


set( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${_libName} )
set( _jagDrawHeaderFiles
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/BufferObject.h
#    ${HEADER_PATH}/Drawable.h
#    ${HEADER_PATH}/DrawableAttribute.h
#    ${HEADER_PATH}/DrawContext.h
    ${HEADER_PATH}/PlatformGLEW.h
    ${HEADER_PATH}/PlatformOpenGL.h
    ${HEADER_PATH}/Shader.h
    ${HEADER_PATH}/ShaderProgram.h
#    ${HEADER_PATH}/Texture.h
#    ${HEADER_PATH}/Transform.h
    ${HEADER_PATH}/UniformValue.h
#    ${HEADER_PATH}/VertexAttribute.h
#    ${HEADER_PATH}/VertexIterator.h
)

set( _jagDrawSourceFiles
    BufferObject.cpp
    Shader.cpp
    ShaderProgram.cpp
#    stats.cpp
    UniformValue.cpp
)

unset( _jagDrawGLEWSourceFiles )
if( GLEW_FOUND )
    set( _jagDrawGLEWSourceFiles
        GLEW.cpp
    )
endif()

_addLibrary( ${_libName}
    ${_jagDrawHeaderFiles}
    ${_jagDrawSourceFiles}
    ${_jagDrawGLEWSourceFiles}
    JAG_LIBRARIES
        jagBase
)
