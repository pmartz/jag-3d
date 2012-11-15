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
Texture::Texture( const GLenum target, ImagePtr image, SamplerPtr sampler )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( "jag.draw.tex" ),
    _target( target ),
    _image( image ),
    _sampler( sampler )
{
}
Texture::Texture( const Texture& rhs )
  : DrawablePrep( rhs ),
    FramebufferAttachable( rhs ),
    jagBase::LogBase( rhs ),
    _target( rhs._target ),
    _image( rhs._image ),
    _sampler( rhs._sampler )
{
}
Texture::~Texture()
{
    // TBD Handle object deletion
}


void Texture::setTarget( const GLenum target )
{
    _target = target;
}
GLenum Texture::getTarget() const
{
    return( _target );
}
bool Texture::isProxy() const
{
    return( ( _target == GL_PROXY_TEXTURE_1D ) ||
        ( _target == GL_PROXY_TEXTURE_2D ) ||
        ( _target == GL_PROXY_TEXTURE_3D ) ||
        ( _target == GL_PROXY_TEXTURE_CUBE_MAP ) ||
        ( _target == GL_PROXY_TEXTURE_1D_ARRAY ) ||
        ( _target == GL_PROXY_TEXTURE_2D_ARRAY ) ||
        ( _target == GL_PROXY_TEXTURE_RECTANGLE ) ||
        ( _target == GL_PROXY_TEXTURE_CUBE_MAP_ARRAY ) ||
        ( _target == GL_PROXY_TEXTURE_2D_MULTISAMPLE ) ||
        ( _target == GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY ) );
}


void Texture::setImage( ImagePtr image )
{
    _image = image;
}
ImagePtr Texture::getImage() const
{
    return( _image );
}

void Texture::setSampler( SamplerPtr sampler )
{
    _sampler = sampler;
}
SamplerPtr Texture::getSampler() const
{
    return( _sampler );
}



void Texture::activate( DrawInfo& drawInfo, const unsigned int unit )
{
    JAG3D_TRACE( "activate" );

    GLenum localUnit( ( unit >= GL_TEXTURE0 ) ? unit : GL_TEXTURE0 + unit );
    glActiveTexture( localUnit );

#ifdef GL_VERSION_3_3
    if( _sampler != NULL )
        _sampler->executeSampler( drawInfo._id, localUnit - GL_TEXTURE0 );
#endif
}

void Texture::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute" );

    const unsigned int contextID( drawInfo._id );

    glBindTexture( _target, getID( contextID ) );

    JAG3D_ERROR_CHECK( "Texture::execute()" );

#ifndef GL_VERSION_3_3
    if( _sampler != NULL )
        _sampler->executeTexture( _target );
#endif
}

GLuint Texture::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}
void Texture::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );
    if( _sampler != NULL )
        _sampler->setMaxContexts( numContexts );
}
void Texture::deleteID( const jagDraw::jagDrawContextID contextID )
{
    deleteID( contextID );
    if( _sampler != NULL )
        _sampler->deleteID( contextID );
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

    JAG3D_TRACE( "internalInit" );

    glGenTextures( 1, &( _ids[ contextID ] ) );
    const GLint id( _ids[ contextID ] );

    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "internalInit(): ContextID: " << contextID <<
            ", object ID: " << id << std::endl;
    }

    glBindTexture( _target, id );

#ifndef GL_VERSION_3_3
    if( _sampler != NULL )
        _sampler->executeTexture( _target );
#endif


    GLint level( 0 );
    GLenum internalFormat( GL_NONE );
    GLsizei width( 0 ), height( 0 ), depth( 0 );
    GLint border( 0 );
    GLenum format( GL_NONE );
    GLenum type( GL_NONE );
    GLsizei imageSize( 0 );
    jagBase::BufferPtr data;
    const bool compressed( _image->getCompressed() );
    if( compressed )
        _image->getCompressed( level, internalFormat, width, height, depth,
            border, imageSize, data );
    else
        _image->get( level, internalFormat, width, height, depth,
            border, format, type, data );

    const void* dataAddress( ( data != NULL ) ? data->data() : NULL );

    // If the image has a pixel store object, send the pixel
    // store parameters to OpenGL.
    if( _image->getPixelStore() != NULL )
        // TBD support for pixel unpack buffer objects.
        _image->getPixelStore()->unpack();

    switch( _target )
    {
    case GL_TEXTURE_1D:
    case GL_PROXY_TEXTURE_1D:
        if( compressed )
            glCompressedTexImage1D( _target, level, internalFormat,
                width, border, imageSize, dataAddress );
        else
            glTexImage1D( _target, level, internalFormat,
                width, border, format, type, dataAddress );
        break;

    case GL_TEXTURE_RECTANGLE:
        if( compressed )
        {
            JAG3D_WARNING( "internalInit(): OpenGL prohibits compressed GL_TEXTURE_RECTANGLE textures." );
        }
        // No 'break': Intentional fall-through.
    case GL_TEXTURE_2D:
    case GL_TEXTURE_1D_ARRAY:
    case GL_PROXY_TEXTURE_2D:
    case GL_PROXY_TEXTURE_1D_ARRAY:
    case GL_PROXY_TEXTURE_RECTANGLE:
    case GL_PROXY_TEXTURE_CUBE_MAP:
        if( compressed )
            glCompressedTexImage2D( _target, level, internalFormat,
                width, height, border, imageSize, dataAddress );
        else
            glTexImage2D( _target, level, internalFormat,
                width, height, border, format, type, dataAddress );
        break;

    case GL_TEXTURE_CUBE_MAP:
        JAG3D_ERROR( "internalInit: GL_TEXTURE_CUBE_MAP not yet implemented." );
        JAG3D_ERROR( "\tProbably should allow Image to be a 6-element array." );
        break;

    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
    case GL_PROXY_TEXTURE_3D:
    case GL_PROXY_TEXTURE_2D_ARRAY:
        if( compressed )
            glCompressedTexImage3D( _target, level, internalFormat,
                width, height, depth, border, imageSize, dataAddress );
        else
            glTexImage3D( _target, level, internalFormat,
                width, height, depth, border, format, type, dataAddress );
        break;
#ifdef GL_VERSION_4_0
    case GL_TEXTURE_CUBE_MAP_ARRAY:
    case GL_PROXY_TEXTURE_CUBE_MAP_ARRAY:
        glTexImage3D( _target, level, internalFormat,
            width, height, depth, border, format, type, dataAddress );
        break;
#endif

    default:
        JAG3D_ERROR( "internalInit: Invalid or unsupported texture target." );
        break;
    }

    //glGenerateMipmap( _target );

    glBindTexture( _target, 0 );

    JAG3D_ERROR_CHECK( "Texture::internalInit()" );
}


// jagDraw
}
