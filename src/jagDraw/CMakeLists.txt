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


set( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${_libName} )
set( _jagDrawHeaderFiles
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/BufferObject.h
    ${HEADER_PATH}/ContextSupport.h
    ${HEADER_PATH}/Drawable.h
    ${HEADER_PATH}/DrawablePrep.h
    ${HEADER_PATH}/DrawCommand.h
    ${HEADER_PATH}/DrawInfo.h
    ${HEADER_PATH}/DrawCommon.h
    ${HEADER_PATH}/Error.h
    ${HEADER_PATH}/ObjectID.h
    ${HEADER_PATH}/PerContextData.h
    ${HEADER_PATH}/PlatformOpenGL.h
    ${HEADER_PATH}/Program.h
    ${HEADER_PATH}/Shader.h
#    ${HEADER_PATH}/Texture.h
    ${HEADER_PATH}/Uniform.h
    ${HEADER_PATH}/VertexArrayCommand.h
    ${HEADER_PATH}/VertexArrayObject.h
    ${HEADER_PATH}/VertexAttrib.h
)

set( _jagDrawSourceFiles
    BufferObject.cpp
    ContextSupport.cpp
    Drawable.cpp
    DrawInfo.cpp
    Error.cpp
    Program.cpp
    Shader.cpp
    Uniform.cpp
    VertexArrayObject.cpp
)

# Using gl3w instead of GLEW for Windows GL3/4 support. See top-level
# CMakeLists.txt for info.
# unset( _jagDrawGLEWHeaderFiles )
# unset( _jagDrawGLEWSourceFiles )
# if( GLEW_FOUND )
#     set( _jagDrawGLEWHeaderFiles
#         ${HEADER_PATH}/PlatformGLEW.h
#         ${HEADER_PATH}/ContextSupportGLEW.h
#     )
#     set( _jagDrawGLEWSourceFiles
#         ContextSupportGLEW.cpp
#     )
# endif()

unset( _jagDrawGl3wHeaderFiles )
unset( _jagDrawGl3wSourceFiles )
if( JAG3D_USE_GL3W )
    set( _jagDrawGl3wHeaderFiles
        ${HEADER_PATH}/GL3/gl3.h
        ${HEADER_PATH}/GL3/gl3w.h
        ${HEADER_PATH}/ContextSupportGl3w.h
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
