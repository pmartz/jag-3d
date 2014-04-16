/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
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

#ifndef __JAG_DRAW_TYPES_H__
#define __JAG_DRAW_TYPES_H__ 1


#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/PerContextData.h>

#include <vector>


namespace jag {
namespace draw {


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
    typedef jag::base::ptr< __type >::shared_array_ptr __type##Array;

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
typedef jag::base::ptr< GLvoid* >::shared_array_ptr GLvoidPtrArray;

// Just define PerContext* -- Other types defined in jag/base/types.h.
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


// namespace jag::draw::
}
}


// __JAG_DRAW_TYPES_H__
#endif
