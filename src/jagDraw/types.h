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

#ifndef __JAGDRAW_TYPES_H__
#define __JAGDRAW_TYPES_H__ 1


#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/PerContextData.h>

#include <vector>


namespace jagDraw {


/** \def DEFINE_TYPE_ARRAYS
\brief Define  PerContextData, std::vector, and shared_array_ptr
arrays for base OpenGL types. Examples:
\li PerContextGLenum: A PerContextData array of GLenum.
\li GLenumVec: A std::vector of GLenum.
\li GLenumArray: A shared_array_ptr of GLenum.
*/
#define DEFINE_TYPE_ARRAYS(__type) \
    typedef PerContextData< __type > PerContext##__type; \
    typedef std::vector< __type > __type##Vec; \
    typedef jagBase::ptr< __type >::shared_array_ptr __type##Array;

DEFINE_TYPE_ARRAYS(GLenum);
DEFINE_TYPE_ARRAYS(GLboolean);
DEFINE_TYPE_ARRAYS(GLbitfield);
DEFINE_TYPE_ARRAYS(GLbyte);
DEFINE_TYPE_ARRAYS(GLshort);
DEFINE_TYPE_ARRAYS(GLint);
DEFINE_TYPE_ARRAYS(GLsizei);
DEFINE_TYPE_ARRAYS(GLubyte);
DEFINE_TYPE_ARRAYS(GLushort);
DEFINE_TYPE_ARRAYS(GLuint);
DEFINE_TYPE_ARRAYS(GLhalf);
DEFINE_TYPE_ARRAYS(GLfloat);
DEFINE_TYPE_ARRAYS(GLclampf);
DEFINE_TYPE_ARRAYS(GLdouble);
DEFINE_TYPE_ARRAYS(GLclampd);

typedef PerContextData< GLvoid* > PerContextGLvoidPtr;
typedef std::vector< GLvoid* > GLvoidPtrVec;
typedef jagBase::ptr< GLvoid* >::shared_array_ptr GLvoidPtrArray;

#undef DEFINE_TYPE_ARRAYS


// jagDraw
}


// __JAGDRAW_TYPES_H__
#endif
