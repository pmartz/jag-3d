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

#include <jagDraw/UniformValue.h>
#include <jagDraw/PlatformOpenGL.h>

#include <string>


namespace jagDraw {


UniformValue::UniformValue()
  : _transpose( false )
{
}

UniformValue::UniformValue( const UniformValue &v )
  : _type( v._type ),
    _transpose( v._transpose )
{
    switch( _type )
    {
        case GL_INT:        _value.i = v._value.i;                                                  break;
        case GL_INT_VEC2:   memcpy( (void *)_value.v2i, (const void *)v._value.v2i, sizeof(_value.v2i)); break;
        case GL_INT_VEC3:   memcpy( (void *)_value.v3i, (const void *)v._value.v3i, sizeof(_value.v3i));  break;
        case GL_INT_VEC4:   memcpy( (void *)_value.v4i, (const void *)v._value.v4i, sizeof(_value.v4i));  break;

        case GL_FLOAT:      _value.f = v._value.f;                                                  break;
        case GL_FLOAT_VEC2: memcpy( (void *)_value.v2f, (const void *)v._value.v2f, sizeof(_value.v2f)); break;
        case GL_FLOAT_VEC3: memcpy( (void *)_value.v3f, (const void *)v._value.v3f, sizeof(_value.v3f));  break;
        case GL_FLOAT_VEC4: memcpy( (void *)_value.v4f, (const void *)v._value.v4f, sizeof(_value.v4f));  break;


        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: memcpy( _value.mat3f, v._value.mat3f, sizeof(_value.mat3f) ); break;
        case GL_FLOAT_MAT4: memcpy( _value.mat4f, v._value.mat4f, sizeof(_value.mat4f) ); break;

        default:
            break;
    }
}


UniformValue::UniformValue( const GLint i )
{
    _type = GL_INT;
    _value.i = i;
}

/*
UniformValue::UniformValue( iiMath::vec2i v2i )
{
    _type = GL_INT_VEC2;
    memcpy( (void *)_value.v2i, (const void *)v2i.getData(), sizeof( _value.v2i ));
}

UniformValue::UniformValue( iiMath::vec3i v3i )
{
    _type = GL_INT_VEC3;
    memcpy( (void *)_value.v3i, (const void *)v3i.getData(), sizeof(_value.v3i ));
}

UniformValue::UniformValue( iiMath::vec4i v4i )
{
    _type = GL_INT_VEC4;
    memcpy( (void *)_value.v4i, (const void *) v4i.getData(), sizeof( _value.v4i ));
}

UniformValue::UniformValue( GLfloat f )
{
    _type = GL_FLOAT;
    _value.f = f;
}


UniformValue::UniformValue( iiMath::vec2f v2f )
{
    _type = GL_FLOAT_VEC2;
    memcpy( (void *)_value.v2f, (const void *)v2f.getData(), sizeof( _value.v2f));
}

UniformValue::UniformValue( iiMath::vec3f v3f )
{
    _type = GL_FLOAT_VEC3;
    memcpy( (void *)_value.v3f, (const void *)v3f.getData(), sizeof(_value.v3f) );
}

UniformValue::UniformValue( iiMath::vec4f v4f )
{
    _type = GL_FLOAT_VEC4;
    memcpy( (void *)_value.v4f, (const void *)v4f.getData(), sizeof(_value.v4f) );
}


UniformValue::UniformValue( iiMath::matrix3f mat3f )
{
    _type = GL_FLOAT_MAT3;
    memcpy( _value.mat3f, mat3f.getData(), sizeof(_value.mat3f) );
}

UniformValue::UniformValue( iiMath::matrix4f mat4f )
{
    _type = GL_FLOAT_MAT4;
    memcpy( _value.mat4f, mat4f.getData(), sizeof(_value.mat4f) );
}
*/

void UniformValue::apply( const GLint loc ) const
{
    switch( _type )
    {
        case GL_INT:        glUniform1i( loc, _value.i );         break;
        case GL_INT_VEC2:   glUniform2iv( loc, 1, _value.v2i );   break;
        case GL_INT_VEC3:   glUniform3iv( loc, 1, _value.v3i );   break;
        case GL_INT_VEC4:   glUniform4iv( loc, 1, _value.v4i );   break;

        case GL_FLOAT:      glUniform1f( loc, _value.f );         break;
        case GL_FLOAT_VEC2: glUniform2fv( loc, 1, _value.v2f );   break;
        case GL_FLOAT_VEC3: glUniform3fv( loc, 1, _value.v3f );   break;
        case GL_FLOAT_VEC4: glUniform4fv( loc, 1, _value.v4f );   break;

        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: glUniformMatrix3fv( loc, 1, _transpose ? GL_TRUE : GL_FALSE, &_value.mat3f[0][0] ); break;
        case GL_FLOAT_MAT4: glUniformMatrix4fv( loc, 1, _transpose ? GL_TRUE : GL_FALSE, &_value.mat4f[0][0] ); break;

        default:
            break;
    }
}

void UniformValue::apply() const
{
}


// jagDraw
}
