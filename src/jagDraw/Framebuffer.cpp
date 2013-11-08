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


Framebuffer::Framebuffer( GLenum target, const std::string& logName )
  : DrawablePrep( Framebuffer_t ),
    jagBase::LogBase( logName.empty() ? "jag.draw.fbo" : logName ),
    _target( target ),
    _viewport( false ),
    _clear( false ),
    _vpX( 0 ),
    _vpY( 0 ),
    _vpWidth( 0 ),
    _vpHeight( 0 ),
    _clearMask( 0 )

	
{
	_clearColor[0] = 1.0;
	_clearColor[1] = 1.0;
	_clearColor[2] = 1.0;
	_clearColor[3]  = 0.0;

}
Framebuffer::Framebuffer( const Framebuffer& rhs )
  : DrawablePrep( rhs ),
    ObjectID( rhs ),
    jagBase::LogBase( rhs ),
    _target( rhs._target ),
    _viewport( rhs._viewport ),
    _clear( rhs._clear ),
    _vpX( rhs._vpX ),
    _vpY( rhs._vpY ),
    _vpWidth( rhs._vpWidth ),
    _vpHeight( rhs._vpHeight ),
    _clearMask( rhs._clearMask )
{
}
Framebuffer::~Framebuffer()
{
    // TBD Handle object deletion
}


void Framebuffer::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );
    const GLuint id( getID( contextID )  );
    glBindFramebuffer( _target, id );

    if( JAG3D_LOG_TRACE )
    {
        std::ostringstream ostr;
        ostr << "Target: " << std::hex << _target << ", ID: " << std::dec << id;
        JAG3D_TRACE( ostr.str() );
    }

    cleanDirtyAttachments( contextID );

    // TBD need better support for this.
    if( id == 0 )
        glDrawBuffer( GL_BACK );
    else
    {
        std::vector<GLenum> buffers;
        BOOST_FOREACH( AttachmentMap::value_type pair, _attachments ) 
        {
            //we need a better way of sorting these out . . .
            if( pair.first!=GL_DEPTH_ATTACHMENT )
                buffers.push_back(pair.first);
            else
                buffers.push_back(GL_NONE);
        }
        glDrawBuffers( (GLsizei)( buffers.size() ), &(buffers[0]));
    }

    if( _viewport )
        glViewport( _vpX, _vpY, _vpWidth, _vpHeight );
    if( _clear )
    {
        glClearColor( _clearColor[ 0 ], _clearColor[ 1 ], _clearColor[ 2 ], _clearColor[ 3 ] );
        glClear( _clearMask );
    }

    JAG3D_FBO_ERROR_CHECK( "Framebuffer::execute()" );
    JAG3D_ERROR_CHECK( "Framebuffer::execute()" );
}

GLuint Framebuffer::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _attachments.empty() )
    {
        // Default framebuffer.
        return( 0 );
    }

    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void Framebuffer::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );
    _dirtyAttachments._data.resize( numContexts );

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
    dirtyAllAttachments( contextID );

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


void Framebuffer::setViewport( const GLint x, const GLint y, const GLsizei width, const GLsizei height, const bool enable )
{
    _vpX = x;
    _vpY = y;
    _vpWidth = width;
    _vpHeight = height;
    _viewport = enable;
}
void Framebuffer::setClear( const GLbitfield mask, const bool enable )
{
    _clearMask = mask;
    _clear = enable;
}
void Framebuffer::setClearColor( const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a )
{
    _clearColor[ 0 ] = r;
    _clearColor[ 1 ] = g;
    _clearColor[ 2 ] = b;
    _clearColor[ 3 ] = a;
}



void Framebuffer::addAttachment( const GLenum attachment, FramebufferAttachablePtr buffer )
{
    _attachments[ attachment ] = buffer;
    dirtyAttachmentForAllContexts( attachment );
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
    attachAll( contextID );
    glBindFramebuffer( _target, 0 );

    JAG3D_ERROR_CHECK( "Framebuffer::internalInit()" );
}
void Framebuffer::attachAll( const unsigned int contextID )
{
    BOOST_FOREACH( AttachmentMap::value_type pair, _attachments )
    {
        pair.second->attachToFBO( contextID, pair.first );
    }
    // Mark all attachments clean.
    dirtyAllAttachments( contextID, false );

    JAG3D_FBO_ERROR_CHECK( "Framebuffer::internalInit()" );
}

void Framebuffer::dirtyAttachmentForAllContexts( const GLenum attachment )
{
    BOOST_FOREACH( DirtyAttachmentMap& dirtyMap, _dirtyAttachments._data )
    {
        dirtyMap[ attachment ] = true;
    }
}
void Framebuffer::dirtyAllAttachments( const unsigned int contextID, const bool dirty )
{
    if( contextID >= _dirtyAttachments._data.size() )
    {
        JAG3D_WARNING( "dirtyAllAttachments() got incorrect contextID." );
        return;
    }

    DirtyAttachmentMap& dirtyMap( _dirtyAttachments._data[ contextID ] );
    BOOST_FOREACH( DirtyAttachmentMap::value_type& data, dirtyMap )
    {
        data.second = dirty;
    }
}
bool Framebuffer::anyDirty(  const unsigned int contextID  ) const
{
    if( contextID >= _dirtyAttachments._data.size() )
    {
        JAG3D_WARNING( "anyDirty() got incorrect contextID." );
        return( false );
    }

    const DirtyAttachmentMap& dirty( _dirtyAttachments._data[ contextID ] );
    BOOST_FOREACH( const DirtyAttachmentMap::value_type& data, dirty )
    {
        if( data.second )
            return( true );
    }
    return( false );
}
void Framebuffer::cleanDirtyAttachments( const unsigned int contextID )
{
    if( anyDirty( contextID ) )
        attachAll( contextID );
    else
    {
        // If any one of the attached images claims to be dirty, attach it.
        BOOST_FOREACH( AttachmentMap::value_type pair, _attachments )
        {
            if( pair.second->isDirty( contextID ) )
                pair.second->attachToFBO( contextID, pair.first );
        }
    }

    JAG3D_FBO_ERROR_CHECK( "Framebuffer::cleanDirtyAttachments()" );
}




Renderbuffer::Renderbuffer( const std::string& logName )
    : FramebufferAttachable(),
    jagBase::LogBase( logName ),
    _samples( 0 ),
    _internalFormat( GL_NONE ),
    _width( 0 ),
    _height( 0 )
{
}
Renderbuffer::Renderbuffer( const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei samples, const std::string& logName )
  : FramebufferAttachable(),
    jagBase::LogBase( logName.empty() ? "jag.draw.fbo.rb" : logName ),
    _samples( samples ),
    _internalFormat( internalFormat),
    _width( width ),
    _height( height )
{
}
Renderbuffer::Renderbuffer( const Renderbuffer& rhs )
  : ObjectID( rhs ),
    FramebufferAttachable( rhs ),
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


void Renderbuffer::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindRenderbuffer( GL_RENDERBUFFER, getID( contextID ) );

    JAG3D_ERROR_CHECK( "Renderbuffer::execute()" );
}

GLuint Renderbuffer::getID( const jagDraw::jagDrawContextID contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}
void Renderbuffer::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );

    _dirty._data.resize( numContexts );
    BOOST_FOREACH( GLboolean& dirty, _dirty._data )
    {
        dirty = GL_TRUE;
    }
}

void Renderbuffer::attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment )
{
    JAG3D_TRACE( "attachToFBO" );
    glFramebufferRenderbuffer( _fboTarget, attachment, GL_RENDERBUFFER, getID( contextID ) );

    _dirty._data[ contextID ] = GL_FALSE;
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

bool Renderbuffer::isDirty( const unsigned int contextID ) const
{
    if( contextID < _dirty._data.size() )
        return( _dirty._data[ contextID ] == GL_TRUE );
    else
        return( GL_TRUE );
}


// jagDraw
}
