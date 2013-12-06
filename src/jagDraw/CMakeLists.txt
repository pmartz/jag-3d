set( _libName jagDraw )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGDRAW_LIBRARY )
endif()


set( _jagDrawHeaderFiles
    Export.h
    Bound.h
    BoundOwner.h
    BufferObject.h
    Command.h
    CommandMap.h
    Common.h
    ContextSupport.h
    Drawable.h
    DrawCommand.h
    DrawGraph.h
    DrawInfo.h
    DrawNode.h
    DrawNodeContainer.h
    Error.h
    Framebuffer.h
    FramebufferAttachable.h
    Image.h
    ObjectID.h
    PerContextData.h
    PixelStore.h
    PlatformOpenGL.h
    Program.h
    Sampler.h
    Shader.h
    Texture.h
    TransformCallback.h
    types.h
    Uniform.h
    UniformBlock.h
    Version.h
    VertexArrayCommand.h
    VertexArrayObject.h
    VertexAttrib.h
    VertexAttribContainer.h
    TriangleSurfer.h
    Visitor.h
)

set( _jagDrawSourceFiles
    BufferObject.cpp
    ContextSupport.cpp
    Drawable.cpp
    DrawGraph.cpp
    DrawInfo.cpp
    DrawNode.cpp
    DrawNodeContainer.cpp
    Error.cpp
    Framebuffer.cpp
    Image.cpp
    PixelStore.cpp
    Program.cpp
    Sampler.cpp
    Shader.cpp
    Texture.cpp
    TransformCallback.cpp
    Uniform.cpp
    UniformBlock.cpp
    Version.cpp
    VertexArrayObject.cpp
)

unset( _jagDrawGl3wHeaderFiles )
unset( _jagDrawGl3wSourceFiles )
if( JAG3D_USE_GL3W )
    set( _jagDrawGl3wHeaderFiles
        ${PROJECT_SOURCE_DIR}/src/jagDraw/gl3w.h
        glExtensions.h
        ContextSupportGl3w.h
    )
    set( _jagDrawGl3wSourceFiles
        ${PROJECT_SOURCE_DIR}/src/jagDraw/gl3w.c
        ContextSupportGl3w.cpp
    )
endif()

_addLibrary( ${_libName}
    ${_jagDrawHeaderFiles}
    ${_jagDrawGl3wHeaderFiles}
    ${_jagDrawSourceFiles}
    ${_jagDrawGl3wSourceFiles}
    JAG_LIBRARIES
        jagBase
)


if( JAG3D_USE_GL3W AND JAG3D_USE_GL3W_AUTOGENERATE )
    # Generate gl3w.c and gl3w.h if out of data.
    if( PYTHONINTERP_FOUND )
        add_custom_command( 
            OUTPUT
                ${PROJECT_SOURCE_DIR}/src/jagDraw/gl3w.c
                ${PROJECT_SOURCE_DIR}/src/jagDraw/gl3w.h
            COMMAND
                ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/gl3w/gl3w_gen.py
            MAIN_DEPENDENCY glExtensions.h
            DEPENDS ${GL3_GLCOREARB_HEADER}/GL/glcorearb.h
            COMMENT "Executing Python-based GL3W source code generation:" )
    else()
        message( STATUS "Unable to generate GL3W source because Python was not found." )
    endif()
endif()

install(
    DIRECTORY .
    DESTINATION include/jagDraw
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
