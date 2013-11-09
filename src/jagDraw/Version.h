/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
