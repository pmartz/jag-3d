/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

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
