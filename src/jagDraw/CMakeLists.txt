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

set( _libName jagDraw )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGDRAW_LIBRARY )
endif()


set( _jagDrawHeaderFiles
    Export.h
    Bound.h
    BufferObject.h
    CommandMap.h
    Common.h
    ContextSupport.h
    Drawable.h
    DrawablePrep.h
    DrawCommand.h
    DrawGraph.h
    DrawInfo.h
    Error.h
    Framebuffer.h
    FramebufferAttachable.h
    Image.h
    Node.h
    NodeContainer.h
    ObjectID.h
    PerContextData.h
    PixelStore.h
    PlatformOpenGL.h
    Program.h
    Sampler.h
    Shader.h
    Texture.h
    types.h
    Uniform.h
    UniformBlock.h
    Version.h
    VertexArrayCommand.h
    VertexArrayObject.h
    VertexAttrib.h
    VertexAttribContainer.h
)

set( _jagDrawSourceFiles
    BufferObject.cpp
    ContextSupport.cpp
    Drawable.cpp
    DrawGraph.cpp
    DrawInfo.cpp
    Error.cpp
    Framebuffer.cpp
    Image.cpp
    Node.cpp
    NodeContainer.cpp
    PixelStore.cpp
    Program.cpp
    Sampler.cpp
    Shader.cpp
    Texture.cpp
    Uniform.cpp
    UniformBlock.cpp
    Version.cpp
    VertexArrayObject.cpp
)

unset( _jagDrawGl3wHeaderFiles )
unset( _jagDrawGl3wSourceFiles )
if( JAG3D_USE_GL3W )
    set( _jagDrawGl3wHeaderFiles
        GL3/gl3w.h
        ContextSupportGl3w.h
    )
    set( _jagDrawGl3wSourceFiles
        gl3w.c
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
