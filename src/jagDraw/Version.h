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

#ifndef __JAGDRAW_VERSION_H__
#define __JAGDRAW_VERSION_H__ 1

#include <jagDraw/Export.h>
#include <string>


namespace jagDraw {


/** \addtogroup jagVersion Version Utilities */
/*@{*/

/** \brief OpenGL version number as an integer.
\details Returns the OpenGL version number as an integer computed
from the formula major * 10000 + minor * 100 + patch. For "3.1", the
returned value is 30100, for example.

NOTE: This function requires a current context. Returns -1 if the
OpenGL GL_VERSION string is empty. */
int JAGDRAW_EXPORT getOpenGLVersionNumber();

/** \brief OpenGL version number as a string
\details Returns the GL_VERSION string.

If \c silent is false, this function automatically logs OpenGL version information to the
Logger named "jag.draw.version" at priority "info".

NOTE: This function requires a current context. */
std::string JAGDRAW_EXPORT getOpenGLVersionString( const bool silent=false );

/*@}*/


// jagDraw
}


// __JAGDRAW_VERSION_H__
#endif
