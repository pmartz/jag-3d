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

#include <boost/foreach.hpp>


namespace jagDraw {


Texture::Texture( const std::string& logName )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( logName.empty() ? "jag.draw.tex" : logName ),
    _target( GL_NONE ),
    _bufferFormat( GL_NONE )
{
}
Texture::Texture( const GLenum target, ImagePtr image, const std::string& logName )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( logName.empty() ? "jag.draw.tex" : logName ),
    _target( target ),
    _bufferFormat( GL_NONE )
{
    _image.resize( 1 );
    _image[ 0 ] = image;
}
Texture::Texture( const GLenum target, ImagePtr image, SamplerPtr sampler, const std::string& logName )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( logName.empty() ? "jag.draw.tex" : logName ),
    _target( target ),
    _sampler( sampler ),
    _bufferFormat( GL_NONE )
{
    _image.resize( 1 );
    _image[ 0 ] = image;
}
Texture::Texture( const GLenum target, GLenum bufferFormat, TextureBufferPtr& textureBuffer, const std::string& logName )
  : DrawablePrep( Texture_t ),
    FramebufferAttachable(),
    jagBase::LogBase( logName.empty() ? "jag.draw.tex" : logName ),
    _target( target ),
    _bufferFormat( bufferFormat ),
    _textureBuffer( textureBuffer )
{
    if( _target != GL_TEXTURE_BUFFER )
        JAG3D_WARNING( "Texture buffer constructor: Invalid target parameter." );
}
Texture::Texture( const Texture& rhs )
  : DrawablePrep( rhs ),
    FramebufferAttachable( rhs ),
    ObjectID( rhs ),
    jagBase::LogBase( rhs ),
    _target( rhs._target ),
    _image( rhs._image ),
    _sampler( rhs._sampler ),
    _bufferFormat( rhs._bufferFormat ),
    _textureBuffer( rhs._textureBuffer )
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
#ifdef GL_VERSION_4_0
        ( _target == GL_PROXY_TEXTURE_CUBE_MAP_ARRAY ) ||
#endif
        ( _target == GL_PROXY_TEXTURE_2D_MULTISAMPLE ) ||
        ( _target == GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY ) );
}


void Texture::setImage( ImagePtr image, const GLenum cubeTarget )
{
    JAG3D_TRACE( "setImage() \"" + getUserDataName() + "\"" );

    markAllDirty();

    if( cubeTarget == GL_NONE )
    {
        _image.resize( 1 );
        _image[ 0 ] = image;
        return;
    }

    _image.resize( 6 );
    // Assume cube target enum values are sequential starting with
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X. This is not in the spec, but
    // was in the extension spec and they are defined sequentially in
    // glcorearb.h.
    int index( (int)cubeTarget - (int)GL_TEXTURE_CUBE_MAP_POSITIVE_X );
    if( ( index >= 0 ) && ( index < 6 ) )
    {
        _image[ index ] = image;
        return;
    }

    JAG3D_ERROR( "setImage: Invalid cubeTarget parameter." );
}
ImagePtr Texture::getImage( const GLenum cubeTarget ) const
{
    if( ( cubeTarget == GL_NONE ) && ( _image.size() == 1 ) )
        return( _image[ 0 ] );

    // Assume cube target enum values are sequential starting with
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X. This is not in the spec, but
    // was in the extension spec and they are defined sequentially in
    // glcorearb.h.
    int index( (int)cubeTarget - (int)GL_TEXTURE_CUBE_MAP_POSITIVE_X );
    if( ( index >= 0 ) && ( index < 6 ) && ( _image.size() == 6 ) )
        return( _image[ index ] );

    return( ImagePtr( (Image*)NULL ) );
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
    JAG3D_TRACE( "activate() \"" + getUserDataName() + "\"" );

    GLenum localUnit( ( unit >= GL_TEXTURE0 ) ? unit : GL_TEXTURE0 + unit );
    glActiveTexture( localUnit );

#ifdef GL_VERSION_3_3
    if( _sampler != NULL )
        _sampler->executeSampler( drawInfo._id, localUnit - GL_TEXTURE0 );
#endif
}

void Texture::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute() \"" + getUserDataName() + "\"" );

    const unsigned int contextID( drawInfo._id );

    glBindTexture( _target, getID( contextID ) );

    if( _dirty[ contextID ] == GL_TRUE )
        internalSpecifyTexImage( contextID );

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

    if( _textureBuffer != NULL )
        _textureBuffer->setMaxContexts( numContexts );

    _dirty._data.resize( numContexts );
    BOOST_FOREACH( GLboolean& dirty, _dirty._data )
    {
        dirty = GL_TRUE;
    }
}
void Texture::deleteID( const jagDraw::jagDrawContextID contextID )
{
    deleteID( contextID );
    if( _sampler != NULL )
        _sampler->deleteID( contextID );
}

void Texture::attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment )
{
    JAG3D_TRACE( "attachToFBO() \"" + getUserDataName() + "\"" );

    const GLuint texID( getID( contextID ) );
    if( _dirty[ contextID ] == GL_TRUE )
    {
        glBindTexture( _target, texID );
        internalSpecifyTexImage( contextID );
        glBindTexture( _target, 0 );
    }

    glFramebufferTexture( _fboTarget, attachment, texID, _fboTextureLevel );
}


void Texture::internalInit( const unsigned int contextID )
{
    JAG3D_TRACE( "internalInit() \"" + getUserDataName() + "\"" );

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

    internalSpecifyTexImage( contextID );

    glBindTexture( _target, 0 );

    JAG3D_ERROR_CHECK( "Texture::internalInit()" );
}

void Texture::internalSpecifyTexImage( const unsigned int contextID )
{
    JAG3D_TRACE( "internalSpecifyTexImage() \"" + getUserDataName() + "\"" );

    if( !( _image.empty() ) )
    {
        if( ( _target != GL_TEXTURE_CUBE_MAP ) &&
            ( _target != GL_PROXY_TEXTURE_CUBE_MAP ) )
        {
            internalSpecifyTexImage( _target, _image[ 0 ] );
        }
        else
        {
            for( unsigned int idx=0; idx<6; idx++ )
            {
                GLenum target;
                switch( idx )
                {
                case 0: target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
                case 1: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
                case 2: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
                case 3: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
                case 4: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
                case 5: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
                }
                internalSpecifyTexImage( target, _image[ 0 ] );
            }
        }
    }
    else if( _target == GL_TEXTURE_BUFFER )
    {
        if( _textureBuffer != NULL )
            glTexBuffer( _target, _bufferFormat, _textureBuffer->getID( contextID ) );
    }

    _dirty[ contextID ] = GL_FALSE;
}

void Texture::internalSpecifyTexImage( const GLenum target, ImagePtr image )
{
    JAG3D_TRACE( "internalSpecifyTexImage(ImagePtr) \"" + getUserDataName() + "\"" );

    GLint level( 0 );
    GLenum internalFormat( GL_NONE );
    GLsizei width( 0 ), height( 0 ), depth( 0 );
    GLint border( 0 );
    GLenum format( GL_NONE );
    GLenum type( GL_NONE );
    GLsizei imageSize( 0 );
    unsigned char* data;
    const bool compressed( image->getCompressed() );
    if( compressed )
        image->getCompressed( level, internalFormat, width, height, depth,
            border, imageSize, &data );
    else
        image->get( level, internalFormat, width, height, depth,
            border, format, type, &data );

    // If the image has a pixel store object, send the pixel
    // store parameters to OpenGL.
    if( image->getPixelStore() != NULL )
        // TBD support for pixel unpack buffer objects.
        image->getPixelStore()->unpack();

    switch( target )
    {
    case GL_TEXTURE_1D:
    case GL_PROXY_TEXTURE_1D:
        if( compressed )
            glCompressedTexImage1D( target, level, internalFormat,
                width, border, imageSize, data );
        else
            glTexImage1D( target, level, internalFormat,
                width, border, format, type, data );
        break;

    case GL_TEXTURE_RECTANGLE:
        if( compressed )
        {
            JAG3D_WARNING( "internalSpecifyTexImage(): OpenGL prohibits compressed GL_TEXTURE_RECTANGLE textures." );
        }
        // No 'break': Intentional fall-through.
    case GL_TEXTURE_2D:
    case GL_TEXTURE_1D_ARRAY:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    case GL_PROXY_TEXTURE_2D:
    case GL_PROXY_TEXTURE_1D_ARRAY:
    case GL_PROXY_TEXTURE_RECTANGLE:
        if( compressed )
            glCompressedTexImage2D( target, level, internalFormat,
                width, height, border, imageSize, data );
        else
            glTexImage2D( target, level, internalFormat,
                width, height, border, format, type, data );
        break;

    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
#ifdef GL_VERSION_4_0
    case GL_TEXTURE_CUBE_MAP_ARRAY:
#endif
    case GL_PROXY_TEXTURE_3D:
    case GL_PROXY_TEXTURE_2D_ARRAY:
#ifdef GL_VERSION_4_0
    case GL_PROXY_TEXTURE_CUBE_MAP_ARRAY:
#endif
        if( compressed )
            glCompressedTexImage3D( target, level, internalFormat,
                width, height, depth, border, imageSize, data );
        else
            glTexImage3D( target, level, internalFormat,
                width, height, depth, border, format, type, data );
        break;

    default:
        JAG3D_ERROR( "internalSpecifyTexImage: Invalid or unsupported texture target." );
        break;
    }
}

void Texture::markAllDirty()
{
    JAG3D_TRACE( "markAllDirty() \"" + getUserDataName() + "\"" );

    BOOST_FOREACH( GLboolean& item, _dirty._data )
    {
        item = GL_TRUE;
    }
}
bool Texture::isDirty( const unsigned int contextID ) const
{
    if( contextID < _dirty._data.size() )
        return( _dirty._data[ contextID ] == GL_TRUE );
    else
        return( GL_TRUE );
}




// jagDraw
}
