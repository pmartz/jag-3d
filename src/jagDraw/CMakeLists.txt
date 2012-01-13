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
    ${HEADER_PATH}/common.h
#    ${HEADER_PATH}/Drawable.h
#    ${HEADER_PATH}/DrawableAttribute.h
#    ${HEADER_PATH}/DrawContext.h
    ${HEADER_PATH}/Error.h
    ${HEADER_PATH}/init.h
    ${HEADER_PATH}/PerContextData.h
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
    Error.cpp
    init.cpp
    Shader.cpp
    ShaderProgram.cpp
#    stats.cpp
    UniformValue.cpp
)

unset( _jagDrawGLEWHeaderFiles )
unset( _jagDrawGLEWSourceFiles )
if( GLEW_FOUND )
    set( _jagDrawGLEWHeaderFiles
        ${HEADER_PATH}/PlatformGLEW.h
    )
    set( _jagDrawGLEWSourceFiles
        GLEW.cpp
    )
endif()

_addLibrary( ${_libName}
    ${_jagDrawHeaderFiles}
    ${_jagDrawGLEWHeaderFiles}
    ${_jagDrawSourceFiles}
    ${_jagDrawGLEWSourceFiles}
    JAG_LIBRARIES
        jagBase
)
