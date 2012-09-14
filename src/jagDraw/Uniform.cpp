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
#include <jagBase/LogMacros.h>

#include <string>


namespace jagDraw {


Uniform::Uniform( const std::string& name )
{
    internalInit( name );
}
Uniform::Uniform( const Uniform& rhs )
  : _name( rhs._name ),
    _indexHash( rhs._indexHash ),
    _value( rhs._value ),
    _type( rhs._type ),
    _transpose( rhs._transpose )
{
}
Uniform::~Uniform()
{
}

#define TYPE_METHOD_BODIES(__type,__typeid) \
    Uniform::Uniform( const std::string& name, const __type& v ) \
    { \
        internalInit( name ); \
        _type = __typeid; \
        set( v ); \
    } \
    void Uniform::set( const __type& v ) \
    { \
        if( _type != __typeid ) \
        { \
            JAG3D_ERROR_STATIC( "jag.draw.uniform", "Type mismatch." ); \
            return; \
        } \
        _value = v; \
    } \
    void Uniform::get( __type& v ) \
    { \
        v = boost::any_cast< __type >( _value ); \
    }

TYPE_METHOD_BODIES( bool, GL_BOOL )
TYPE_METHOD_BODIES( GLint, GL_INT )
TYPE_METHOD_BODIES( GLfloat, GL_FLOAT )

TYPE_METHOD_BODIES( gmtl::Point2i, GL_INT_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3i, GL_INT_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4i, GL_INT_VEC4 )

TYPE_METHOD_BODIES( gmtl::Point2f, GL_FLOAT_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3f, GL_FLOAT_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4f, GL_FLOAT_VEC4 )

TYPE_METHOD_BODIES( gmtl::Matrix33f, GL_FLOAT_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix44f, GL_FLOAT_MAT4 )

TYPE_METHOD_BODIES( gmtl::Matrix22f, GL_FLOAT_MAT2 )
TYPE_METHOD_BODIES( gmtl::Matrix23f, GL_FLOAT_MAT2x3 )
//TYPE_METHOD_BODIES( gmtl::Matrix24f, GL_FLOAT_MAT2x4 )
//TYPE_METHOD_BODIES( gmtl::Matrix32f, GL_FLOAT_MAT3x2 )
TYPE_METHOD_BODIES( gmtl::Matrix34f, GL_FLOAT_MAT3x4 )
//TYPE_METHOD_BODIES( gmtl::Matrix42f, GL_FLOAT_MAT4x2 )
//TYPE_METHOD_BODIES( gmtl::Matrix43f, GL_FLOAT_MAT4x3 )


void Uniform::operator()( DrawInfo& drawInfo, const GLint loc ) const
{
    // This should NOT be necessary, as we should only be here if this
    // uniform is already in the drawInfo._uniformMap.
    //drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    switch( _type )
    {
#define CASE_CALL_UNIFORM(__typeid,__type,__func) \
        case __typeid: \
        { \
            const __type& v( boost::any_cast< const __type >( _value ) ); \
            __func( loc, v ); \
            break; \
        }
#define CASE_CALL_UNIFORM_V(__typeid,__type,__func) \
        case __typeid: \
        { \
            const __type& v( boost::any_cast< const __type >( _value ) ); \
            __func( loc, 1, v.getData() ); \
            break; \
        }
#define CASE_CALL_MATRIXUNIFORM_V(__typeid,__type,__func) \
        case __typeid: \
        { \
            const __type& v( boost::any_cast< const __type >( _value ) ); \
            __func( loc, 1, _transpose ? GL_TRUE : GL_FALSE, v.getData() ); \
            break; \
        }

        CASE_CALL_UNIFORM( GL_BOOL, bool, glUniform1i )
        CASE_CALL_UNIFORM( GL_INT, GLint, glUniform1i )
        CASE_CALL_UNIFORM( GL_FLOAT, GLfloat, glUniform1f )

        CASE_CALL_UNIFORM_V( GL_INT_VEC2, gmtl::Point2i, glUniform2iv )
        CASE_CALL_UNIFORM_V( GL_INT_VEC3, gmtl::Point3i, glUniform3iv )
        CASE_CALL_UNIFORM_V( GL_INT_VEC4, gmtl::Point4i, glUniform4iv )

        CASE_CALL_UNIFORM_V( GL_FLOAT_VEC2, gmtl::Point2f, glUniform2fv )
        CASE_CALL_UNIFORM_V( GL_FLOAT_VEC3, gmtl::Point3f, glUniform3fv )
        CASE_CALL_UNIFORM_V( GL_FLOAT_VEC4, gmtl::Point4f, glUniform4fv )

        CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2, gmtl::Matrix22f, glUniformMatrix2fv )
        CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3, gmtl::Matrix33f, glUniformMatrix3fv )
        CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4, gmtl::Matrix44f, glUniformMatrix4fv )

        CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2x3, gmtl::Matrix23f, glUniformMatrix2x3fv )
        //CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2x4, gmtl::Matrix24f, glUniformMatrix2x4fv )
        //CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3x2, gmtl::Matrix32f, glUniformMatrix3x2fv )
        CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3x4, gmtl::Matrix34f, glUniformMatrix3x4fv )
        //CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4x2, gmtl::Matrix42f, glUniformMatrix4x2fv )
        //CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4x3, gmtl::Matrix43f, glUniformMatrix4x3fv )


        default:
            break;
    }
}

void Uniform::operator()( DrawInfo& drawInfo )
{
    // Add this uniform to the pool of potentially active uniforms
    // for the current frame and draw thread.
    drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    // Uniform::operator() could execute before Program::operator(),
    // so only look up uniform location if a Program is available.
    if( drawInfo._program != NULL )
    {
        GLint index( drawInfo._program->getUniformLocation( _indexHash ) );
        operator()( drawInfo, index );
    }
}


void Uniform::internalInit( const std::string& name )
{
    _name = name;
    _indexHash = Program::createHash( _name );
    _transpose = false;
}


// jagDraw
}
