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


namespace jagDraw {


Framebuffer::Framebuffer()
  : DrawablePrep(),
    jagBase::LogBase( "jag.draw.fbo" )
{
}
Framebuffer::Framebuffer( const Framebuffer& rhs )
  : DrawablePrep( rhs ),
    jagBase::LogBase( rhs )
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


    glBindFramebuffer( _target, 0 );

    JAG3D_ERROR_CHECK( "Framebuffer::internalInit()" );
}


// jagDraw
}
