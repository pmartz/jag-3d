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

#include <jagDraw/Uniform.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>

#include <string>


namespace jagDraw {


Uniform::Uniform( const std::string& name )
  : _transpose( false )
{
    internalInit( name );
}
Uniform::Uniform( const Uniform& u )
  : _name( u._name ),
    _indexHash( u._indexHash ),
    _type( u._type ),
    _transpose( u._transpose )
{
    _value = u._value;

    /*
    switch( _type )
    {
        case GL_INT:        _value.i = u._value.i;                                                  break;
        case GL_INT_VEC2:   memcpy( (void *)_value.v2i, (const void *)u._value.v2i, sizeof(_value.v2i)); break;
        case GL_INT_VEC3:   memcpy( (void *)_value.v3i, (const void *)u._value.v3i, sizeof(_value.v3i));  break;
        case GL_INT_VEC4:   memcpy( (void *)_value.v4i, (const void *)u._value.v4i, sizeof(_value.v4i));  break;

        case GL_FLOAT:      _value.f = u._value.f;                                                  break;
        case GL_FLOAT_VEC2: memcpy( (void *)_value.v2f, (const void *)u._value.v2f, sizeof(_value.v2f)); break;
        case GL_FLOAT_VEC3: memcpy( (void *)_value.v3f, (const void *)u._value.v3f, sizeof(_value.v3f));  break;
        case GL_FLOAT_VEC4: memcpy( (void *)_value.v4f, (const void *)u._value.v4f, sizeof(_value.v4f));  break;

        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: memcpy( _value.mat3f, u._value.mat3f, sizeof(_value.mat3f) ); break;
        case GL_FLOAT_MAT4: memcpy( _value.mat4f, u._value.mat4f, sizeof(_value.mat4f) ); break;

        default:
            break;
    }
    */
}


Uniform::Uniform( const std::string& name, const bool b )
{
    internalInit( name );
    _type = GL_BOOL;
    _value.b = b;
}

Uniform::Uniform( const std::string& name, const GLint i )
{
    internalInit( name );
    _type = GL_INT;
    _value.i = i;
}

Uniform::Uniform( const std::string& name, const GLfloat f )
{
    internalInit( name );
    _type = GL_FLOAT;
    _value.f = f;
}

Uniform::Uniform( const std::string& name, const gmtl::Point2f& p )
{
    internalInit( name );
    _type = GL_FLOAT_VEC2;
    _value.p2f[0] = p[0];
    _value.p2f[1] = p[1];
}

Uniform::Uniform( const std::string& name, const gmtl::Vec3f& p )
{
    internalInit( name );
    _type = GL_FLOAT_VEC3;
    _value.p3f[0] = p[0];
    _value.p3f[1] = p[1];
    _value.p3f[2] = p[2];
}
Uniform::Uniform( const std::string& name, const gmtl::Point3f& p )
{
    internalInit( name );
    _type = GL_FLOAT_VEC3;
    _value.p3f[0] = p[0];
    _value.p3f[1] = p[1];
    _value.p3f[2] = p[2];
}

Uniform::Uniform( const std::string& name, const gmtl::Point4f& p )
{
    internalInit( name );
    _type = GL_FLOAT_VEC4;
    _value.p4f[0] = p[0];
    _value.p4f[1] = p[1];
    _value.p4f[2] = p[2];
    _value.p4f[3] = p[3];
}

Uniform::Uniform( const std::string& name, const gmtl::Matrix33f& m )
{
    internalInit( name );
    _type = GL_FLOAT_MAT3;
    
    unsigned int idx( 0 ), r, c;
    for( r=0; r<3; r++ )
        for( c=0; c<3; c++ )
            _value.mat3f[ idx++ ] = m( r, c );
    /*
    const float* f( m.getData() );
    unsigned int idx;
    for( idx=0; idx<9; idx++ )
        _value.mat3f[ idx ] = f[ idx ];
        */
}
Uniform::Uniform( const std::string& name, const gmtl::Matrix44f& m )
{
    internalInit( name );
    _type = GL_FLOAT_MAT4;

    unsigned int idx( 0 ), r, c;
    for( r=0; r<4; r++ )
        for( c=0; c<4; c++ )
            _value.mat3f[ idx++ ] = m( r, c );
    /*
    const float* f( m.getData() );
    unsigned int idx;
    for( idx=0; idx<16; idx++ )
        _value.mat3f[ idx ] = f[ idx ];
        */
}

/*
Uniform::Uniform( iiMath::vec2i v2i )
{
    _type = GL_INT_VEC2;
    memcpy( (void *)_value.v2i, (const void *)v2i.getData(), sizeof( _value.v2i ));
}

Uniform::Uniform( iiMath::vec3i v3i )
{
    _type = GL_INT_VEC3;
    memcpy( (void *)_value.v3i, (const void *)v3i.getData(), sizeof(_value.v3i ));
}

Uniform::Uniform( iiMath::vec4i v4i )
{
    _type = GL_INT_VEC4;
    memcpy( (void *)_value.v4i, (const void *) v4i.getData(), sizeof( _value.v4i ));
}



Uniform::Uniform( iiMath::vec2f v2f )
{
    _type = GL_FLOAT_VEC2;
    memcpy( (void *)_value.v2f, (const void *)v2f.getData(), sizeof( _value.v2f));
}

Uniform::Uniform( iiMath::vec3f v3f )
{
    _type = GL_FLOAT_VEC3;
    memcpy( (void *)_value.v3f, (const void *)v3f.getData(), sizeof(_value.v3f) );
}

Uniform::Uniform( iiMath::vec4f v4f )
{
    _type = GL_FLOAT_VEC4;
    memcpy( (void *)_value.v4f, (const void *)v4f.getData(), sizeof(_value.v4f) );
}


Uniform::Uniform( iiMath::matrix3f mat3f )
{
    _type = GL_FLOAT_MAT3;
    memcpy( _value.mat3f, mat3f.getData(), sizeof(_value.mat3f) );
}

Uniform::Uniform( iiMath::matrix4f mat4f )
{
    _type = GL_FLOAT_MAT4;
    memcpy( _value.mat4f, mat4f.getData(), sizeof(_value.mat4f) );
}
*/

void Uniform::operator()( DrawInfo& drawInfo, const GLint loc ) const
{
    // This should NOT be necessary, as we should only be here if this
    // uniform is already in the drawInfo._uniformMap.
    //drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    switch( _type )
    {
        case GL_BOOL:       glUniform1i( loc, (GLint)( _value.b ) ); break;

        case GL_INT:        glUniform1i( loc, _value.i );         break;
        case GL_INT_VEC2:   glUniform2iv( loc, 1, _value.v2i );   break;
        case GL_INT_VEC3:   glUniform3iv( loc, 1, _value.v3i );   break;
        case GL_INT_VEC4:   glUniform4iv( loc, 1, _value.v4i );   break;

        case GL_FLOAT:      glUniform1f( loc, _value.f );         break;
        case GL_FLOAT_VEC2: glUniform2fv( loc, 1, _value.p2f );   break;
        case GL_FLOAT_VEC3: glUniform3fv( loc, 1, _value.p3f );   break;
        case GL_FLOAT_VEC4: glUniform4fv( loc, 1, _value.p4f );   break;

        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: glUniformMatrix3fv( loc, 1, _transpose ? GL_TRUE : GL_FALSE, _value.mat3f ); break;
        case GL_FLOAT_MAT4: glUniformMatrix4fv( loc, 1, _transpose ? GL_TRUE : GL_FALSE, _value.mat4f ); break;

        default:
            break;
    }
}

void Uniform::operator()( DrawInfo& drawInfo )
{
    // Add this uniform to the pool of potentially active uniforms
    // for the current frame and draw thread.
    drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    // Uniform::operator() could execute before ShaderProgram::operator(),
    // so only look up uniform location if a ShaderProgram is available.
    if( drawInfo._program != NULL )
    {
        GLint index( drawInfo._program->getUniformLocation( _indexHash ) );
        operator()( drawInfo, index );
    }
}


void Uniform::internalInit( const std::string& name )
{
    _name = name;
    _indexHash = ShaderProgram::createHash( _name );
}


// jagDraw
}
