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
    BufferObject.h
    ContextSupport.h
    Drawable.h
    DrawablePrep.h
    DrawCommand.h
    DrawInfo.h
    DrawCommon.h
    Error.h
    Image.h
    ObjectID.h
    PerContextData.h
    PlatformOpenGL.h
    Program.h
    Shader.h
    Texture.h
    types.h
    Uniform.h
    VertexArrayCommand.h
    VertexArrayObject.h
    VertexAttrib.h
)

set( _jagDrawSourceFiles
    BufferObject.cpp
    ContextSupport.cpp
    Drawable.cpp
    DrawInfo.cpp
    Error.cpp
    Image.cpp
    Program.cpp
    Shader.cpp
    Texture.cpp
    Uniform.cpp
    VertexArrayObject.cpp
)

# Using gl3w instead of GLEW for Windows GL3/4 support. See top-level
# CMakeLists.txt for info.
# unset( _jagDrawGLEWHeaderFiles )
# unset( _jagDrawGLEWSourceFiles )
# if( GLEW_FOUND )
#     set( _jagDrawGLEWHeaderFiles
#         PlatformGLEW.h
#         ContextSupportGLEW.h
#     )
#     set( _jagDrawGLEWSourceFiles
#         ContextSupportGLEW.cpp
#     )
# endif()

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
    ${_jagDrawGLEWHeaderFiles}
    ${_jagDrawGl3wHeaderFiles}
    ${_jagDrawSourceFiles}
    ${_jagDrawGLEWSourceFiles}
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
