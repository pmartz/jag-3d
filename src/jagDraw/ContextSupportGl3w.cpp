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

#include <jagDraw/ContextSupportGl3w.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogMacros.h>
#include <jagDraw/Error.h>


namespace jagDraw {


ContextSupportGl3w::ContextSupportGl3w()
{
}
ContextSupportGl3w::~ContextSupportGl3w()
{
}

bool ContextSupportGl3w::initContext()
{
    if( !( ContextSupport::initContext() ) )
        return( false );

    int result;
    {
        boost::mutex::scoped_lock lock( _mutex );
        result = gl3wInit();
    }

    if( result == 1 )
    {
        JAG3D_ERROR( "Error return from gl3wInit()" );
    }
    JAG3D_ERROR_CHECK( "ContextSupportGl3w::initContext()" );

    return( result == 0 );
}


// jagDraw
}
