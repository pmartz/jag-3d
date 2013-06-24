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


typedef PerContextData< GLboolean > PerContextGLboolean;
typedef PerContextData< GLint > PerContextGLint;
typedef PerContextData< GLuint > PerContextGLuint;
typedef PerContextData< GLfloat > PerContextGLfloat;
typedef PerContextData< GLdouble > PerContextGLdouble;


typedef std::vector< GLboolean > GLbooleanVec;
typedef std::vector< GLubyte > GLubyteVec;
typedef std::vector< GLushort > GLushortVec;
typedef std::vector< GLint > GLintVec;
typedef std::vector< GLuint > GLuintVec;
typedef std::vector< GLfloat > GLfloatVec;
typedef std::vector< GLdouble > GLdoubleVec;

typedef jagBase::ptr< GLboolean >::shared_array_ptr GLbooleanArray;
typedef jagBase::ptr< GLint >::shared_array_ptr GLintArray;
typedef jagBase::ptr< GLuint >::shared_array_ptr GLuintArray;
typedef jagBase::ptr< GLsizei >::shared_array_ptr GLsizeiArray;
typedef jagBase::ptr< GLvoid* >::shared_array_ptr GLvoidPtrArray;


// jagDraw
}


// __JAGDRAW_TYPES_H__
#endif
