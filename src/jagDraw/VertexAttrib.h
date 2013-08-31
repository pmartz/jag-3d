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

        // TBD We must support the following use case. Imagine a jagDraw::Node that sets a
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


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_H__
#endif
