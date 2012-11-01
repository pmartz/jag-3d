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

This function automatically logs OpenGL version information to the
Logger named "jag.draw.version" at priority "info".

NOTE: This function requires a current context. */
std::string JAGDRAW_EXPORT getOpenGLVersionString();

/*@}*/


// jagDraw
}


// __JAGDRAW_VERSION_H__
#endif
