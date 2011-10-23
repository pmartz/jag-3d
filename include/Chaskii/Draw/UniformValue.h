/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_DRAW_UNIFORM_VALUE_DEF
#define CHASKII_DRAW_UNIFORM_VALUE_DEF 1

#include <Chaskii/Export.h>
#include <Chaskii/OpenGL.h>
#include <Chaskii/Math/vec.h>
#include <Chaskii/Math/matrix.h>

namespace iiDraw {

class II_EXPORT UniformValue 
{
    public:

        UniformValue();
        UniformValue( const UniformValue &v );

        UniformValue( GLint i );
        UniformValue( iiMath::vec2i v2i );
        UniformValue( iiMath::vec3i v3i );
        UniformValue( iiMath::vec4i v4i );

        UniformValue( GLfloat f );
        UniformValue( iiMath::vec2f v2f );
        UniformValue( iiMath::vec3f v3f );
        UniformValue( iiMath::vec4f v4f );

        UniformValue( iiMath::matrix3f mat3f );
        UniformValue( iiMath::matrix4f mat4f );

        void apply(GLint loc, bool transpose=false) const;

        GLenum getType() { return m_type; }

    //private:
        GLenum m_type;
        union {

            GLint   i;
            GLint   v2i[2]; 
            GLint   v3i[3]; 
            GLint   v4i[4]; 

            GLfloat f;
            GLfloat v2f[2];
            GLfloat v3f[3];
            GLfloat v4f[4];

            GLfloat mat3f[3][3];
            GLfloat mat4f[4][4];
        }m_val;
};

}


#endif
