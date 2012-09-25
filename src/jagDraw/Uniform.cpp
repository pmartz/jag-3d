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
#include <jagDraw/Error.h>

#include <string>


namespace jagDraw {


Uniform::Uniform( const std::string& name )
  : DrawablePrep(),
    jagBase::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
}
Uniform::Uniform( const std::string& name, const GLenum type )
  : DrawablePrep(),
    jagBase::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
    setType( type );
}
Uniform::Uniform( const std::string& name, const GLenum type, const GLint& v )
    : DrawablePrep(),
    jagBase::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
    setType( type );
    _value = v;
}

Uniform::Uniform( const Uniform& rhs )
  : DrawablePrep( rhs ),
    jagBase::LogBase( "jag.draw.uniform" ),
    _name( rhs._name ),
    _indexHash( rhs._indexHash ),
    _value( rhs._value ),
    _type( rhs._type ),
    _isSampler( rhs._isSampler ),
    _transpose( rhs._transpose )
{
}
Uniform::~Uniform()
{
}


void Uniform::setSampler( const GLint& v )
{
    if( !_isSampler )
    {
        JAG3D_ERROR( "Type mismatch. setSampler() requires sampler type." );
        return;
    }
    _value = v;
} \


#define TYPE_METHOD_BODIES(__type,__typeid) \
    Uniform::Uniform( const std::string& name, const __type& v ) \
      : DrawablePrep(), \
        jagBase::LogBase( "jag.draw.uniform" ) \
    { \
        internalInit( name ); \
        setType( __typeid ); \
        set( v ); \
    } \
    void Uniform::set( const __type& v ) \
    { \
        if( ( _type == __typeid ) || \
            (( __typeid == GL_INT ) && _isSampler ) ) \
        { \
            _value = v; \
            return; \
        } \
        JAG3D_ERROR( "Type mismatch." ); \
    } \
    void Uniform::get( __type& v ) \
    { \
        v = boost::any_cast< __type >( _value ); \
    }

TYPE_METHOD_BODIES( bool, GL_BOOL )
TYPE_METHOD_BODIES( GLint, GL_INT )
TYPE_METHOD_BODIES( GLuint, GL_UNSIGNED_INT )
TYPE_METHOD_BODIES( GLfloat, GL_FLOAT )
#ifdef GL_VERSION_4_0
TYPE_METHOD_BODIES( GLdouble, GL_DOUBLE )
#endif

TYPE_METHOD_BODIES( gmtl::Point2i, GL_INT_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3i, GL_INT_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4i, GL_INT_VEC4 )

TYPE_METHOD_BODIES( gmtl::Point2ui, GL_UNSIGNED_INT_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3ui, GL_UNSIGNED_INT_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4ui, GL_UNSIGNED_INT_VEC4 )

TYPE_METHOD_BODIES( gmtl::Point2f, GL_FLOAT_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3f, GL_FLOAT_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4f, GL_FLOAT_VEC4 )

#ifdef GL_VERSION_4_0
TYPE_METHOD_BODIES( gmtl::Point2d, GL_DOUBLE_VEC2 )
TYPE_METHOD_BODIES( gmtl::Point3d, GL_DOUBLE_VEC3 )
TYPE_METHOD_BODIES( gmtl::Point4d, GL_DOUBLE_VEC4 )
#endif

TYPE_METHOD_BODIES( gmtl::Matrix22f, GL_FLOAT_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix33f, GL_FLOAT_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix44f, GL_FLOAT_MAT4 )

TYPE_METHOD_BODIES( gmtl::Matrix23f, GL_FLOAT_MAT2x3 )
TYPE_METHOD_BODIES( gmtl::Matrix24f, GL_FLOAT_MAT2x4 )
TYPE_METHOD_BODIES( gmtl::Matrix32f, GL_FLOAT_MAT3x2 )
TYPE_METHOD_BODIES( gmtl::Matrix34f, GL_FLOAT_MAT3x4 )
TYPE_METHOD_BODIES( gmtl::Matrix42f, GL_FLOAT_MAT4x2 )
TYPE_METHOD_BODIES( gmtl::Matrix43f, GL_FLOAT_MAT4x3 )

#ifdef GL_VERSION_4_0
TYPE_METHOD_BODIES( gmtl::Matrix22d, GL_DOUBLE_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix33d, GL_DOUBLE_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix44d, GL_DOUBLE_MAT4 )

TYPE_METHOD_BODIES( gmtl::Matrix23d, GL_DOUBLE_MAT2x3 )
TYPE_METHOD_BODIES( gmtl::Matrix24d, GL_DOUBLE_MAT2x4 )
TYPE_METHOD_BODIES( gmtl::Matrix32d, GL_DOUBLE_MAT3x2 )
TYPE_METHOD_BODIES( gmtl::Matrix34d, GL_DOUBLE_MAT3x4 )
TYPE_METHOD_BODIES( gmtl::Matrix42d, GL_DOUBLE_MAT4x2 )
TYPE_METHOD_BODIES( gmtl::Matrix43d, GL_DOUBLE_MAT4x3 )
#endif


void Uniform::operator()( DrawInfo& drawInfo, const GLint loc ) const
{
    // This should NOT be necessary, as we should only be here if this
    // uniform is already in the drawInfo._uniformMap.
    //drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    if( _isSampler )
    {
        const GLint& v( boost::any_cast< const GLint >( _value ) );
        glUniform1i( loc, v );
    }

    else
    {
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
            CASE_CALL_UNIFORM( GL_UNSIGNED_INT, GLuint, glUniform1ui )
            CASE_CALL_UNIFORM( GL_FLOAT, GLfloat, glUniform1f )
#ifdef GL_VERSION_4_0
            CASE_CALL_UNIFORM( GL_DOUBLE, GLdouble, glUniform1d )
#endif

            CASE_CALL_UNIFORM_V( GL_INT_VEC2, gmtl::Point2i, glUniform2iv )
            CASE_CALL_UNIFORM_V( GL_INT_VEC3, gmtl::Point3i, glUniform3iv )
            CASE_CALL_UNIFORM_V( GL_INT_VEC4, gmtl::Point4i, glUniform4iv )

            CASE_CALL_UNIFORM_V( GL_UNSIGNED_INT_VEC2, gmtl::Point2ui, glUniform2uiv )
            CASE_CALL_UNIFORM_V( GL_UNSIGNED_INT_VEC3, gmtl::Point3ui, glUniform3uiv )
            CASE_CALL_UNIFORM_V( GL_UNSIGNED_INT_VEC4, gmtl::Point4ui, glUniform4uiv )

            CASE_CALL_UNIFORM_V( GL_FLOAT_VEC2, gmtl::Point2f, glUniform2fv )
            CASE_CALL_UNIFORM_V( GL_FLOAT_VEC3, gmtl::Point3f, glUniform3fv )
            CASE_CALL_UNIFORM_V( GL_FLOAT_VEC4, gmtl::Point4f, glUniform4fv )

#ifdef GL_VERSION_4_0
            CASE_CALL_UNIFORM_V( GL_DOUBLE_VEC2, gmtl::Point2d, glUniform2dv )
            CASE_CALL_UNIFORM_V( GL_DOUBLE_VEC3, gmtl::Point3d, glUniform3dv )
            CASE_CALL_UNIFORM_V( GL_DOUBLE_VEC4, gmtl::Point4d, glUniform4dv )
#endif

            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2, gmtl::Matrix22f, glUniformMatrix2fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3, gmtl::Matrix33f, glUniformMatrix3fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4, gmtl::Matrix44f, glUniformMatrix4fv )

            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2x3, gmtl::Matrix23f, glUniformMatrix2x3fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT2x4, gmtl::Matrix24f, glUniformMatrix2x4fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3x2, gmtl::Matrix32f, glUniformMatrix3x2fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT3x4, gmtl::Matrix34f, glUniformMatrix3x4fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4x2, gmtl::Matrix42f, glUniformMatrix4x2fv )
            CASE_CALL_MATRIXUNIFORM_V( GL_FLOAT_MAT4x3, gmtl::Matrix43f, glUniformMatrix4x3fv )

#ifdef GL_VERSION_4_0
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT2, gmtl::Matrix22d, glUniformMatrix2dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT3, gmtl::Matrix33d, glUniformMatrix3dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT4, gmtl::Matrix44d, glUniformMatrix4dv )

            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT2x3, gmtl::Matrix23d, glUniformMatrix2x3dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT2x4, gmtl::Matrix24d, glUniformMatrix2x4dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT3x2, gmtl::Matrix32d, glUniformMatrix3x2dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT3x4, gmtl::Matrix34d, glUniformMatrix3x4dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT4x2, gmtl::Matrix42d, glUniformMatrix4x2dv )
            CASE_CALL_MATRIXUNIFORM_V( GL_DOUBLE_MAT4x3, gmtl::Matrix43d, glUniformMatrix4x3dv )
#endif


            default:
                JAG3D_ERROR( "operator(): Type unsupported." );
                break;
        }
    }

    const GLenum errorEnum( JAG3D_ERROR_CHECK( "Uniform::operator()" ) );
    if( JAG3D_LOG_CRITICAL && ( errorEnum == GL_INVALID_OPERATION ) )
    {
        JAG3D_CRITICAL( "\tVariable \"" + _name + "\": Possible type mismatch between host and shader." );
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


void Uniform::setType( const GLenum type )
{
    _type = type;
    _isSampler = (
        ( _type == GL_SAMPLER_1D ) ||
        ( _type == GL_SAMPLER_2D ) ||
        ( _type == GL_SAMPLER_3D ) ||
        ( _type == GL_SAMPLER_CUBE ) ||
        ( _type == GL_SAMPLER_1D_SHADOW ) ||
        ( _type == GL_SAMPLER_2D_SHADOW ) ||
        ( _type == GL_SAMPLER_1D_ARRAY ) ||
        ( _type == GL_SAMPLER_2D_ARRAY ) ||
        ( _type == GL_SAMPLER_1D_ARRAY_SHADOW ) ||
        ( _type == GL_SAMPLER_2D_ARRAY_SHADOW ) ||
        ( _type == GL_SAMPLER_CUBE_SHADOW ) ||
        ( _type == GL_SAMPLER_2D_RECT ) ||
        ( _type == GL_SAMPLER_2D_RECT_SHADOW ) ||
        ( _type == GL_SAMPLER_BUFFER ) ||
        ( _type == GL_SAMPLER_2D_MULTISAMPLE ) ||
        ( _type == GL_SAMPLER_2D_MULTISAMPLE_ARRAY ) ||

#ifdef GL_VERSION_4_0
        ( _type == GL_SAMPLER_BINDING ) ||
#endif

        ( _type == GL_INT_SAMPLER_1D ) ||
        ( _type == GL_INT_SAMPLER_2D ) ||
        ( _type == GL_INT_SAMPLER_3D ) ||
        ( _type == GL_INT_SAMPLER_CUBE ) ||
        ( _type == GL_INT_SAMPLER_1D_ARRAY ) ||
        ( _type == GL_INT_SAMPLER_2D_ARRAY ) ||
        ( _type == GL_INT_SAMPLER_2D_RECT ) ||
        ( _type == GL_INT_SAMPLER_BUFFER ) ||
        ( _type == GL_INT_SAMPLER_2D_MULTISAMPLE ) ||
        ( _type == GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_1D ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_2D ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_3D ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_CUBE ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_1D_ARRAY ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_2D_ARRAY ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_2D_RECT ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_BUFFER ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY )
#ifdef GL_VERSION_4_0
        ||
        ( _type == GL_SAMPLER_CUBE_MAP_ARRAY ) ||
        ( _type == GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW ) ||
        ( _type == GL_INT_SAMPLER_CUBE_MAP_ARRAY ) ||
        ( _type == GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY )
#endif
        );
}

void Uniform::internalInit( const std::string& name )
{
    _name = name;
    _indexHash = Program::createHash( _name );
    _transpose = false;
}


// jagDraw
}
