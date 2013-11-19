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
#define DEFINE_PER_CTX(__type,__name ) \
    typedef PerContextData< __type > PerContext##__name;
#define DEFINE_TYPE_ARRAYS(__type) \
    DEFINE_PER_CTX(__type,__type) \
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

// Just define PerContext* -- Other types defined in jagBase/types.h.
DEFINE_PER_CTX(unsigned char,UChar);
DEFINE_PER_CTX(unsigned int,UInt);
DEFINE_PER_CTX(float,Float);
DEFINE_PER_CTX(double,Double);
// WARNING: PerContextBool is identical to PerContextUCharVec.
// This is so that T& operator[]() semantics will work
// (they would fail in, e.g., std::vector<bool>).
DEFINE_PER_CTX(unsigned char,Bool);

#undef DEFINE_TYPE_ARRAYS
#undef DEFINE_PER_CTX


// jagDraw
}


// __JAGDRAW_TYPES_H__
#endif
