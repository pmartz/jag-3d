/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

#include <jagDraw/Texture.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>


namespace jagDraw {


Texture::Texture()
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( "jag.draw.tex" ),
    _target( GL_NONE )
{
}
Texture::Texture( const GLenum target, ImagePtr image )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( "jag.draw.tex" ),
    _target( target ),
    _image( image )
{
}
Texture::Texture( const Texture& rhs )
  : DrawablePrep( rhs ),
    FramebufferAttachable( rhs ),
    jagBase::LogBase( rhs ),
    _target( rhs._target ),
    _image( rhs._image )
{
}
Texture::~Texture()
{
    // TBD Handle object deletion
}


void Texture::operator()( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    // TBD need to get this from somewhere, but it doesn't make sense to
    // store it internally in the Texture object.
    glActiveTexture( GL_TEXTURE0 );

    glBindTexture( _target, getID( contextID ) );

    JAG3D_ERROR_CHECK( "Texture::operator()" );
}

GLuint Texture::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void Texture::attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment )
{
    JAG3D_TRACE( "attachToFBO" );
    glFramebufferTexture( _fboTarget, attachment, getID( contextID ), _fboTextureLevel );
}


void Texture::internalInit( const unsigned int contextID )
{
    if( _image == NULL )
        return;

    glGenTextures( 1, &( _ids[ contextID ] ) );
    const GLint id( _ids[ contextID ] );

    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "internalInit(): ContextID: " << contextID <<
            ", object ID: " << id << std::endl;
    }

    glBindTexture( _target, id );

    glTexParameterf( _target, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameterf( _target, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    glTexParameterf( _target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( _target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );


    GLint level;
    GLenum internalFormat;
    GLsizei width, height, depth;
    GLint border;
    GLenum format;
    GLenum type;
    jagBase::BufferPtr data;
    _image->get( level, internalFormat, width, height, depth,
        border, format, type, data );

    const void* dataAddress( ( data != NULL ) ? data->data() : NULL );

    // If the image has a pixel store object, send the pixel
    // store parameters to OpenGL.
    if( _image->getPixelStore() != NULL )
        _image->getPixelStore()->unpack();

    switch( _target )
    {
    case GL_TEXTURE_1D:
        glTexImage1D( _target, level, internalFormat,
            width, border, format, type, dataAddress );
        break;
    case GL_TEXTURE_2D:
        glTexImage2D( _target, level, internalFormat,
            width, height, border, format, type, dataAddress );
        break;
    case GL_TEXTURE_3D:
        glTexImage3D( _target, level, internalFormat,
            width, height, depth, border, format, type, dataAddress );
        break;
    }

    //glGenerateMipmap( _target );

    glBindTexture( _target, 0 );

    JAG3D_ERROR_CHECK( "Texture::internalInit()" );
}


// jagDraw
}
