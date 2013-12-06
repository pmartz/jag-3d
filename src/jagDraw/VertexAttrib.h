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

#ifndef __JAGDRAW_VERTEX_ATTRIB_H__
#define __JAGDRAW_VERTEX_ATTRIB_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/Program.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

/** \class VertexAttrib VertexAttrib.h <jagDraw/VertexAttrib.h>
\brief
\details \gl{section 2.8}.
*/
class /*JAGDRAW_EXPORT*/ VertexAttrib : public VertexArrayCommand
{
public:
    VertexAttrib( const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset )
      : VertexArrayCommand( VertexArrayCommand::VertexAttrib_t ),
        _name( name ),
        _indexHash( Program::createHash( name ) ),
        _size( size ),
        _type( type ),
        _normalized( normalized ),
        _stride( stride ),
        _offset( offset )
    {}
    VertexAttrib( const VertexAttrib& rhs )
      : VertexArrayCommand( rhs ),
        _name( rhs._name ),
        _indexHash( rhs._indexHash ),
        _size( rhs._size ),
        _type( rhs._type ),
        _normalized( rhs._normalized ),
        _stride( rhs._stride ),
        _offset( rhs._offset )
    {}
    virtual ~VertexAttrib()
    {}

    virtual bool isSameKind( const VertexArrayCommand& rhs ) const
    {
        const VertexAttrib* va( static_cast< const VertexAttrib* >( &rhs ) );
        return( VertexArrayCommand::isSameKind( rhs ) &&
            ( _indexHash == va->_indexHash ) &&
            ( _size == va->_size ) &&
            ( _type == va->_type ) &&
            ( _normalized == va->_normalized ) &&
            ( _stride == va->_stride ) );
    }

    virtual void execute( DrawInfo& drawInfo )
    {
        // Program::execute() must be called prior to VertexAttrib::execute().
        // Note this is different from Uniform::execute(). This means we can simply
        // look up the vertex attrib hash index without bothering to check for
        // ( drawInfo._current[ Program_t ] != NULL ) first.

        // TBD We must support the following use case. Imagine a jagDraw::DrawNode that sets a
        // program and a VAO (and does some drawing), following by another Node that
        // changes only the program (and does some drawing). In this case, the old VAO
        // would be in effect. Would this render correctly?

        // TBD need an "explicit" mode where the location is set by the app
        // (because it uses glBindAttribLocation). This would avoid the table lookup.

        GLint index;
        if( drawInfo._current.contains( DrawablePrep::Program_t ) )
        {
            ProgramPtr prog( boost::dynamic_pointer_cast< Program >( drawInfo._current[ DrawablePrep::Program_t ] ) );
            index = prog->getVertexAttribLocation( _indexHash );
        }
        else
        {
            index = glGetAttribLocation( drawInfo._externalProgramID, _name.c_str() );
        }
        if( index == -1 )
        {
            // This vertex attrib isn't used in the shader.
            return;
        }

        // Note that we do NOT check for index == -1. Inactive vertex attribs
        // are not centerline usage. OpenGL will ignore these calls if
        // index == -1, so we avoid an extra conditional by not checking.
        glEnableVertexAttribArray( index );
        glVertexAttribPointer( index, _size, _type, _normalized, _stride, (GLvoid *)(_offset ) );
    }

    void getAll( GLint& size, GLenum& type, GLboolean& normalized, GLsizei& stride, GLuint& offset ) const
    {
        size = _size;
        type = _type;
        normalized = _normalized;
        stride = _stride;
        offset = (GLuint)( _offset );
    }
    void getSizeType( GLint& size, GLenum& type ) const
    {
        size = _size;
        type = _type;
    }
    GLsizeiptr getOffset() const
    {
        return( _offset );
    }
    GLsizei getActualStride() const
    {
        if( _stride != 0 )
            return( _stride );

        unsigned int bytesPerElement( 0 );
        switch( _type )
        {
        case GL_BYTE: bytesPerElement = sizeof( GLbyte ); break;
        case GL_UNSIGNED_BYTE: bytesPerElement = sizeof( GLubyte ); break;
        case GL_SHORT: bytesPerElement = sizeof( GLshort ); break;
        case GL_UNSIGNED_SHORT: bytesPerElement = sizeof( GLushort ); break;
        case GL_INT: bytesPerElement = sizeof( GLint ); break;
#ifdef GL_VERSION_3_3
        case GL_INT_2_10_10_10_REV: // GLuint
#endif
        case GL_UNSIGNED_INT_2_10_10_10_REV: // GLuint
        case GL_UNSIGNED_INT: bytesPerElement = sizeof( GLuint ); break;
        case GL_HALF_FLOAT: bytesPerElement = sizeof( GLhalf ); break;
        case GL_FLOAT: bytesPerElement = sizeof( GLfloat ); break;
        case GL_DOUBLE: bytesPerElement = sizeof( GLdouble ); break;
        default:
            // TBD;
            bytesPerElement = 1;
            break;
        }
        return( _size * bytesPerElement );
    }

protected:
    std::string _name;
    Program::HashValue _indexHash;
    GLint _size;
    GLenum _type;
    GLboolean _normalized;
    GLsizei _stride;
    GLsizeiptr _offset;
};

typedef jagBase::ptr< jagDraw::VertexAttrib >::shared_ptr VertexAttribPtr;
typedef std::vector< VertexAttribPtr > VertexAttribVec;


/** \class VertexAttribI VertexAttrib.h <jagDraw/VertexAttrib.h>
\brief
\details \gl{section 2.8}.
*/
class /*JAGDRAW_EXPORT*/ VertexAttribI : public VertexAttrib
{
public:
    VertexAttribI( const std::string& name, GLint size, GLenum type, GLsizei stride, GLuint offset )
      : VertexAttrib( name, size, type, false, stride, offset )
    {}
    VertexAttribI( const VertexAttribI& rhs )
      : VertexAttrib( rhs )
    {}
    virtual ~VertexAttribI()
    {}

    virtual void execute( DrawInfo& drawInfo )
    {
        // TBD need an "explicit" mode where the location is set by the app
        // (because it uses glBindAttribLocation). This would avoid the table lookup.
        ProgramPtr prog( boost::dynamic_pointer_cast< Program >( drawInfo._current[ DrawablePrep::Program_t ] ) );
        GLint index( prog->getVertexAttribLocation( _indexHash ) );

        // Note that we do NOT check for index == -1. Inactive vertex attribs
        // are not centerline usage. OpenGL will ignore these calls if
        // index == -1, so we avoid an extra conditional by not checking.
        glEnableVertexAttribArray( index );
        glVertexAttribIPointer( index, _size, _type, _stride, (GLvoid *)(_offset ) );
    }
};

typedef jagBase::ptr< jagDraw::VertexAttribI >::shared_ptr VertexAttribIPtr;
typedef std::vector< VertexAttribIPtr > VertexAttribIVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_H__
#endif
