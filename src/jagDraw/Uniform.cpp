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

#include <jagDraw/Uniform.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>
#include <jag/base/LogMacros.h>
#include <jagDraw/Error.h>

#include <string>


namespace jagDraw {


Uniform::Uniform()
  : Command( Uniform_t ),
    jag::base::LogBase( "jag.draw.uniform" ),
    _transpose( false )
{
}
Uniform::Uniform( const std::string& name )
  : Command( Uniform_t ),
    jag::base::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
}
Uniform::Uniform( const std::string& name, const GLenum type )
  : Command( Uniform_t ),
    jag::base::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
    setType( type );
}
Uniform::Uniform( const std::string& name, const GLenum type, const GLint& v )
  : Command( Uniform_t ),
    jag::base::LogBase( "jag.draw.uniform" )
{
    internalInit( name );
    setType( type );
    _value = v;
}

Uniform::Uniform( const Uniform& rhs )
  : Command( rhs ),
    jag::base::LogBase( rhs ),
    SHARED_FROM_THIS( Uniform )( rhs ),
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


void Uniform::setName( const std::string& name )
{
    internalInit( name );
}
std::string Uniform::getName()
{
    return( _name );
}


void Uniform::setSampler( const GLint& v )
{
    if( !_isSampler )
    {
        JAG3D_ERROR( "Type mismatch. setSampler() requires sampler type." );
        return;
    }
    _value = v;
}


void Uniform::copyValue( void* address )
{
#define CASE_COPY(__type,__typeid) \
    case __typeid: \
    { \
        __type value;  get( value ); \
        std::memcpy( address, &value, sizeof( __type ) ); \
        break; \
    }

    switch( getType() )
    {
    CASE_COPY( bool, GL_BOOL )
    CASE_COPY( GLint, GL_INT )
    CASE_COPY( GLuint, GL_UNSIGNED_INT )
    CASE_COPY( GLfloat, GL_FLOAT )
#ifdef GL_VERSION_4_0
    CASE_COPY( GLdouble, GL_DOUBLE )
#endif

    CASE_COPY( gmtl::Point2i, GL_INT_VEC2 )
    CASE_COPY( gmtl::Point3i, GL_INT_VEC3 )
    CASE_COPY( gmtl::Point4i, GL_INT_VEC4 )

    CASE_COPY( gmtl::Point2ui, GL_UNSIGNED_INT_VEC2 )
    CASE_COPY( gmtl::Point3ui, GL_UNSIGNED_INT_VEC3 )
    CASE_COPY( gmtl::Point4ui, GL_UNSIGNED_INT_VEC4 )

    CASE_COPY( gmtl::Point2f, GL_FLOAT_VEC2 )
    CASE_COPY( gmtl::Point3f, GL_FLOAT_VEC3 )
    CASE_COPY( gmtl::Point4f, GL_FLOAT_VEC4 )

#ifdef GL_VERSION_4_0
    CASE_COPY( gmtl::Point2d, GL_DOUBLE_VEC2 )
    CASE_COPY( gmtl::Point3d, GL_DOUBLE_VEC3 )
    CASE_COPY( gmtl::Point4d, GL_DOUBLE_VEC4 )
#endif

    CASE_COPY( gmtl::Matrix22f, GL_FLOAT_MAT2 )
    CASE_COPY( gmtl::Matrix33f, GL_FLOAT_MAT3 )
    CASE_COPY( gmtl::Matrix44f, GL_FLOAT_MAT4 )

    CASE_COPY( gmtl::Matrix23f, GL_FLOAT_MAT2x3 )
    CASE_COPY( gmtl::Matrix24f, GL_FLOAT_MAT2x4 )
    CASE_COPY( gmtl::Matrix32f, GL_FLOAT_MAT3x2 )
    CASE_COPY( gmtl::Matrix34f, GL_FLOAT_MAT3x4 )
    CASE_COPY( gmtl::Matrix42f, GL_FLOAT_MAT4x2 )
    CASE_COPY( gmtl::Matrix43f, GL_FLOAT_MAT4x3 )

#ifdef GL_VERSION_4_0
    CASE_COPY( gmtl::Matrix22d, GL_DOUBLE_MAT2 )
    CASE_COPY( gmtl::Matrix33d, GL_DOUBLE_MAT3 )
    CASE_COPY( gmtl::Matrix44d, GL_DOUBLE_MAT4 )

    CASE_COPY( gmtl::Matrix23d, GL_DOUBLE_MAT2x3 )
    CASE_COPY( gmtl::Matrix24d, GL_DOUBLE_MAT2x4 )
    CASE_COPY( gmtl::Matrix32d, GL_DOUBLE_MAT3x2 )
    CASE_COPY( gmtl::Matrix34d, GL_DOUBLE_MAT3x4 )
    CASE_COPY( gmtl::Matrix42d, GL_DOUBLE_MAT4x2 )
    CASE_COPY( gmtl::Matrix43d, GL_DOUBLE_MAT4x3 )
#endif
    }

#undef CASE_COPY
}



#define TYPE_METHOD_BODIES(__type,__typeid) \
    Uniform::Uniform( const std::string& name, const __type& v ) \
      : Command( Uniform_t ), \
        jag::base::LogBase( "jag.draw.uniform" ) \
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

TYPE_METHOD_BODIES( gmtl::Matrix22f, GL_FLOAT_MAT2 )
TYPE_METHOD_BODIES( gmtl::Matrix33f, GL_FLOAT_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix44f, GL_FLOAT_MAT4 )

TYPE_METHOD_BODIES( gmtl::Matrix23f, GL_FLOAT_MAT2x3 )
TYPE_METHOD_BODIES( gmtl::Matrix24f, GL_FLOAT_MAT2x4 )
TYPE_METHOD_BODIES( gmtl::Matrix32f, GL_FLOAT_MAT3x2 )
TYPE_METHOD_BODIES( gmtl::Matrix34f, GL_FLOAT_MAT3x4 )
TYPE_METHOD_BODIES( gmtl::Matrix42f, GL_FLOAT_MAT4x2 )
TYPE_METHOD_BODIES( gmtl::Matrix43f, GL_FLOAT_MAT4x3 )

#ifdef GL_VERSION_4_0
TYPE_METHOD_BODIES( gmtl::Matrix22d, GL_DOUBLE_MAT2 )
TYPE_METHOD_BODIES( gmtl::Matrix33d, GL_DOUBLE_MAT3 )
TYPE_METHOD_BODIES( gmtl::Matrix44d, GL_DOUBLE_MAT4 )

TYPE_METHOD_BODIES( gmtl::Matrix23d, GL_DOUBLE_MAT2x3 )
TYPE_METHOD_BODIES( gmtl::Matrix24d, GL_DOUBLE_MAT2x4 )
TYPE_METHOD_BODIES( gmtl::Matrix32d, GL_DOUBLE_MAT3x2 )
TYPE_METHOD_BODIES( gmtl::Matrix34d, GL_DOUBLE_MAT3x4 )
TYPE_METHOD_BODIES( gmtl::Matrix42d, GL_DOUBLE_MAT4x2 )
TYPE_METHOD_BODIES( gmtl::Matrix43d, GL_DOUBLE_MAT4x3 )
#endif


void Uniform::execute( DrawInfo& drawInfo, const GLint loc ) const
{
    if( JAG3D_LOG_TRACE )
    {
        std::ostringstream ostr;
        ostr << "execute(): Loc: " << loc << ", Name \"" << _name << "\".";
        JAG3D_TRACE( ostr.str() );
    }

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
                JAG3D_ERROR( "execute(): Type unsupported." );
                break;
        }
    }

    const GLenum errorEnum( JAG3D_ERROR_CHECK( "Uniform::execute()" ) );
    if( JAG3D_LOG_CRITICAL && ( errorEnum == GL_INVALID_OPERATION ) )
    {
        JAG3D_CRITICAL( "\tVariable \"" + _name + "\": Possible type mismatch between host and shader." );
    }
}

void Uniform::executeWithoutMap( DrawInfo& drawInfo ) const
{
    GLint loc;
    if( drawInfo._current.contains( Program_t ) )
    {
        ProgramPtr prog( boost::dynamic_pointer_cast< Program >( drawInfo._current[ Program_t ] ) );
        loc = prog->getUniformLocation( _indexHash );
    }
    else if( drawInfo._externalProgramID == 0 )
    {
        // This is an error. During the first frame, we are executing the
        // TransformCallback before we bind the Program. This needs to be
        // fixed. TBD.
        loc = -1;
    }
    else
    {
        // No current program. Assume external program is in use.
        loc = glGetUniformLocation( drawInfo._externalProgramID, _name.c_str() );
    }
    if( loc != -1 )
        execute( drawInfo, loc );
}

void Uniform::execute( DrawInfo& drawInfo )
{
    // Add this uniform to the pool of potentially active uniforms
    // for the current frame and draw thread.
    drawInfo._uniformMap[ _indexHash ] = shared_from_this();

    // Actually set the value. Handles the case where there is a
    // current Program that has already executed.
    executeWithoutMap( drawInfo );
}


void Uniform::setType( const GLenum type )
{
    if( _type == type )
        return;

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
