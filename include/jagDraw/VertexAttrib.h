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
#include <jagDraw/ShaderProgram.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class VertexAttribI VertexAttrib.h <jagDraw/VertexAttrib.h>
\brief
\details \gl{section 2.8}.
*/
class /*JAGDRAW_EXPORT*/ VertexAttribI
{
public:
    VertexAttribI( const std::string& name, GLint size, GLenum type, GLsizei stride, GLuint offset )
      : _name( name ),
        _indexHash( ShaderProgram::createHash( name ) ),
        _size( size ),
        _type( type ),
        _stride( stride ),
        _offset( offset )
    {}
    virtual ~VertexAttribI()
    {}

    virtual void operator()( jagDraw::DrawInfo& drawInfo )
    {
        GLint index( drawInfo._shader->getVertexAttribLocation( _indexHash ) );
        if( index != -1 )
        {
            glEnableVertexAttribArray( index );
            glVertexAttribIPointer( index, _size, _type, _stride, (GLvoid *)(_offset ) );
        }
    }

protected:
    std::string _name;
    std::size_t _indexHash;
    GLint  _size;
    GLenum _type;
    GLsizei _stride;
    GLuint _offset;
};

typedef jagBase::ptr< jagDraw::VertexAttribI >::shared_ptr VertexAttribIPtr;
typedef std::vector< VertexAttribIPtr > VertexAttribIList;


/** \class VertexAttrib VertexAttrib.h <jagDraw/VertexAttrib.h>
\brief
\details \gl{section 2.8}.
*/
class /*JAGDRAW_EXPORT*/ VertexAttrib : public VertexAttribI
{
public:
    VertexAttrib( const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset )
      : VertexAttribI( name, size, type, stride, offset ),
        _normalized( normalized )
    {}
    virtual ~VertexAttrib()
    {}

    virtual void operator()( jagDraw::DrawInfo& drawInfo )
    {
        GLint index( drawInfo._shader->getVertexAttribLocation( _indexHash ) );
        if( index != -1 )
        {
            glEnableVertexAttribArray( index );
            glVertexAttribPointer( index, _size, _type, _normalized, _stride, (GLvoid *)(_offset ) );
        }
    }

protected:
    GLboolean _normalized;
};

typedef jagBase::ptr< jagDraw::VertexAttrib >::shared_ptr VertexAttribPtr;
typedef std::vector< VertexAttribPtr > VertexAttribList;


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_H__
#endif
