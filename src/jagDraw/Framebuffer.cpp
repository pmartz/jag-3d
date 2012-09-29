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

#include <jagDraw/Framebuffer.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


Framebuffer::Framebuffer( GLenum target )
  : DrawablePrep(),
    jagBase::LogBase( "jag.draw.fbo" ),
    _target( target )
{
}
Framebuffer::Framebuffer( const Framebuffer& rhs )
  : DrawablePrep( rhs ),
    jagBase::LogBase( rhs ),
    _target( rhs._target )
{
}
Framebuffer::~Framebuffer()
{
    // TBD Handle object deletion
}


void Framebuffer::operator()( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );
    glBindFramebuffer( _target, getID( contextID ) );

    JAG3D_FBO_ERROR_CHECK( "Framebuffer::operator()" );
    JAG3D_ERROR_CHECK( "Framebuffer::operator()" );
}

GLuint Framebuffer::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void Framebuffer::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );

    BOOST_FOREACH( AttachmentMap::value_type pair, _attachments )
    {
        // FramebufferAttachable is either a Texture or a Renderbuffer,
        // so probably derives from ObjectID. But have a check for
        // ObjectIDOwner anyhow (for possible future OpenGL changes).
        ObjectIDPtr objID( boost::dynamic_pointer_cast< ObjectID >( pair.second ) );
        if( objID != NULL )
            objID->setMaxContexts( numContexts );
        else
        {
            ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< ObjectIDOwner >( pair.second ) );
            if( objIDOwner != NULL )
                objIDOwner->setMaxContexts( numContexts );
        }
    }
}

void Framebuffer::deleteID( const jagDraw::jagDrawContextID contextID )
{
    ObjectID::deleteID( contextID );

    BOOST_FOREACH( AttachmentMap::value_type pair, _attachments )
    {
        // FramebufferAttachable is either a Texture or a Renderbuffer,
        // so probably derives from ObjectID. But have a check for
        // ObjectIDOwner anyhow (for possible future OpenGL changes).
        ObjectIDPtr objID( boost::dynamic_pointer_cast< ObjectID >( pair.second ) );
        if( objID != NULL )
            objID->deleteID( contextID );
        else
        {
            ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< ObjectIDOwner >( pair.second ) );
            if( objIDOwner != NULL )
                objIDOwner->deleteID( contextID );
        }
    }
}


void Framebuffer::addAttachment( const GLenum attachment, FramebufferAttachablePtr buffer )
{
    _attachments[ attachment ] = buffer;
}
FramebufferAttachablePtr Framebuffer::getAttachment( const GLenum attachment )
{
    AttachmentMap::iterator found;
    if( ( found = _attachments.find( attachment ) ) != _attachments.end() )
        return( found->second );
    else
        return( FramebufferAttachablePtr( (FramebufferAttachable*)NULL ) );
}


void Framebuffer::internalInit( const unsigned int contextID )
{
    glGenFramebuffers( 1, &( _ids[ contextID ] ) );
    const GLint id( _ids[ contextID ] );

    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "internalInit(): ContextID: " << contextID <<
            ", object ID: " << id << std::endl;
    }

    glBindFramebuffer( _target, id );

    BOOST_FOREACH( AttachmentMap::value_type pair, _attachments )
    {
        pair.second->attachToFBO( contextID, pair.first );
    }

    JAG3D_FBO_ERROR_CHECK( "Framebuffer::internalInit()" );

    glBindFramebuffer( _target, 0 );

    JAG3D_ERROR_CHECK( "Framebuffer::internalInit()" );
}



Renderbuffer::Renderbuffer( const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei samples )
  : FramebufferAttachable(),
    jagBase::LogBase( "jag.draw.fbo.rb" ),
    _samples( samples ),
    _internalFormat( internalFormat),
    _width( width ),
    _height( height )
{
}
Renderbuffer::Renderbuffer( const Renderbuffer& rhs )
  : FramebufferAttachable( rhs ),
    jagBase::LogBase( rhs ),
    _samples( rhs._samples ),
    _internalFormat( rhs._internalFormat),
    _width( rhs._width ),
    _height( rhs._height )
{
}
Renderbuffer::~Renderbuffer()
{
    // TBD Handle object deletion
}


void Renderbuffer::operator()( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindRenderbuffer( GL_RENDERBUFFER, getID( contextID ) );

    JAG3D_ERROR_CHECK( "Renderbuffer::operator()" );
}

GLuint Renderbuffer::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void Renderbuffer::attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment )
{
    JAG3D_TRACE( "attachToFBO" );
    glFramebufferRenderbuffer( _fboTarget, attachment, GL_RENDERBUFFER, getID( contextID ) );
}

void Renderbuffer::internalInit( const unsigned int contextID )
{
    glGenRenderbuffers( 1, &( _ids[ contextID ] ) );
    const GLint id( _ids[ contextID ] );

    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "internalInit(): ContextID: " << contextID <<
            ", object ID: " << id << std::endl;
    }

    glBindRenderbuffer( GL_RENDERBUFFER, id );

    glRenderbufferStorageMultisample( GL_RENDERBUFFER,
        _samples, _internalFormat, _width, _height );

    glBindRenderbuffer( GL_RENDERBUFFER, 0 );

    JAG3D_ERROR_CHECK( "Renderbuffer::internalInit()" );
}


// jagDraw
}
