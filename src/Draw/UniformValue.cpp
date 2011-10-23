#include <string.h>
#include <Chaskii/OpenGL.h>
#include <Chaskii/Draw/UniformValue.h>

namespace iiDraw {

UniformValue::UniformValue()
{
}

UniformValue::UniformValue( const UniformValue &v ):
    m_type(v.m_type)
{
    switch( m_type )
    {
        case GL_INT:        m_val.i = v.m_val.i;                                                  break;
        case GL_INT_VEC2:   memcpy( (void *)m_val.v2i, (const void *)v.m_val.v2i, sizeof(m_val.v2i)); break;
        case GL_INT_VEC3:   memcpy( (void *)m_val.v3i, (const void *)v.m_val.v3i, sizeof(m_val.v3i));  break;
        case GL_INT_VEC4:   memcpy( (void *)m_val.v4i, (const void *)v.m_val.v4i, sizeof(m_val.v4i));  break;

        case GL_FLOAT:      m_val.f = v.m_val.f;                                                  break;
        case GL_FLOAT_VEC2: memcpy( (void *)m_val.v2f, (const void *)v.m_val.v2f, sizeof(m_val.v2f)); break;
        case GL_FLOAT_VEC3: memcpy( (void *)m_val.v3f, (const void *)v.m_val.v3f, sizeof(m_val.v3f));  break;
        case GL_FLOAT_VEC4: memcpy( (void *)m_val.v4f, (const void *)v.m_val.v4f, sizeof(m_val.v4f));  break;


        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: memcpy( m_val.mat3f, v.m_val.mat3f, sizeof(m_val.mat3f) ); break;
        case GL_FLOAT_MAT4: memcpy( m_val.mat4f, v.m_val.mat4f, sizeof(m_val.mat4f) ); break;

        default:
            break;
    }
}


UniformValue::UniformValue( GLint i )
{
    m_type = GL_INT;
    m_val.i = i;
}

UniformValue::UniformValue( iiMath::vec2i v2i )
{
    m_type = GL_INT_VEC2;
    memcpy( (void *)m_val.v2i, (const void *)v2i.getData(), sizeof( m_val.v2i ));
}

UniformValue::UniformValue( iiMath::vec3i v3i )
{
    m_type = GL_INT_VEC3;
    memcpy( (void *)m_val.v3i, (const void *)v3i.getData(), sizeof(m_val.v3i ));
}

UniformValue::UniformValue( iiMath::vec4i v4i )
{
    m_type = GL_INT_VEC4;
    memcpy( (void *)m_val.v4i, (const void *) v4i.getData(), sizeof( m_val.v4i ));
}

UniformValue::UniformValue( GLfloat f )
{
    m_type = GL_FLOAT;
    m_val.f = f;
}


UniformValue::UniformValue( iiMath::vec2f v2f )
{
    m_type = GL_FLOAT_VEC2;
    memcpy( (void *)m_val.v2f, (const void *)v2f.getData(), sizeof( m_val.v2f));
}

UniformValue::UniformValue( iiMath::vec3f v3f )
{
    m_type = GL_FLOAT_VEC3;
    memcpy( (void *)m_val.v3f, (const void *)v3f.getData(), sizeof(m_val.v3f) );
}

UniformValue::UniformValue( iiMath::vec4f v4f )
{
    m_type = GL_FLOAT_VEC4;
    memcpy( (void *)m_val.v4f, (const void *)v4f.getData(), sizeof(m_val.v4f) );
}


UniformValue::UniformValue( iiMath::matrix3f mat3f )
{
    m_type = GL_FLOAT_MAT3;
    memcpy( m_val.mat3f, mat3f.getData(), sizeof(m_val.mat3f) );
}

UniformValue::UniformValue( iiMath::matrix4f mat4f )
{
    m_type = GL_FLOAT_MAT4;
    memcpy( m_val.mat4f, mat4f.getData(), sizeof(m_val.mat4f) );
}

void UniformValue::apply(GLint loc, bool transpose) const
{
    switch( m_type )
    {
        case GL_INT:        glUniform1i( loc, m_val.i );         break;
        case GL_INT_VEC2:   glUniform2iv( loc, 1, m_val.v2i );   break;
        case GL_INT_VEC3:   glUniform3iv( loc, 1, m_val.v3i );   break;
        case GL_INT_VEC4:   glUniform4iv( loc, 1, m_val.v4i );   break;

        case GL_FLOAT:      glUniform1f( loc, m_val.f );         break;
        case GL_FLOAT_VEC2: glUniform2fv( loc, 1, m_val.v2f );   break;
        case GL_FLOAT_VEC3: glUniform3fv( loc, 1, m_val.v3f );   break;
        case GL_FLOAT_VEC4: glUniform4fv( loc, 1, m_val.v4f );   break;

        case GL_FLOAT_MAT2:     break; // unsupported
        case GL_FLOAT_MAT2x3:   break; // unsupported
        case GL_FLOAT_MAT2x4:   break; // unsupported
        case GL_FLOAT_MAT3x2:   break; // unsupported
        case GL_FLOAT_MAT3x4:   break; // unsupported
        case GL_FLOAT_MAT4x2:   break; // unsupported
        case GL_FLOAT_MAT4x3:   break; // unsupported

        case GL_FLOAT_MAT3: glUniformMatrix3fv( loc, 1, transpose?GL_TRUE:GL_FALSE, &m_val.mat3f[0][0] ); break;
        case GL_FLOAT_MAT4: glUniformMatrix4fv( loc, 1, transpose?GL_TRUE:GL_FALSE, &m_val.mat4f[0][0] ); break;

        default:
            break;
    }
}


}
