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

#ifndef __JAGDRAW_DRAW_COMMANDS_H__
#define __JAGDRAW_DRAW_COMMANDS_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/types.h>


namespace jagDraw {


struct DrawInfo;



/** \class DrawCommand DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawCommand //: public DrawableAttribute
{
public:
    enum DrawCommandType{
        DrawArraysType,
        DrawArraysInstancedType,
        DrawArraysIndirectType,
        MultiDrawArraysType,
        DrawElementsType,
        DrawElementsInstancedType,
        MultiDrawElementsType,
        DrawRangeElementsType,
        DrawElementsBaseVertexType,
        DrawRangeElementsBaseVertexType,
        DrawElementsInstancedBaseVertexType,
        DrawElementsIndirectType,
        MultiDrawElementsBaseVertexType
    };

    DrawCommand( DrawCommandType drawCommandType, GLenum mode )
      : _drawCommandType( drawCommandType ),
        _mode( mode )
    {}
    virtual ~DrawCommand() 
    {}


    DrawCommandType getType() { return _drawCommandType; }
    virtual void operator()( DrawInfo& ) {}

protected:
    DrawCommandType _drawCommandType;
    GLenum _mode;

    GLint _first;
    GLsizei _count;
    GLsizei _primcount;
    GLenum _type;
    GLvoid* _offset;
    GLvoid* _indirect;
    jagBase::GLintArray _firstArray;
    jagBase::GLsizeiArray _countArray;
    jagBase::GLvoidPtrArray _indicesArray;
    GLint _basevertex;
    jagBase::GLintArray _basevertexArray;
};

typedef jagBase::ptr< jagDraw::DrawCommand >::shared_ptr DrawCommandPtr;


/** \class DrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArrays : public DrawCommand
{
public:
    DrawArrays( GLenum mode, GLint first, GLsizei count )
      : DrawCommand( DrawArraysType, mode )
    {
        _first = first;
        _count = count;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawArrays( _mode, _first, _count );
    }
};

typedef jagBase::ptr< jagDraw::DrawArrays >::shared_ptr DrawArraysPtr;


/** \class DrawArraysInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArraysInstanced : public DrawCommand
{
public:
    DrawArraysInstanced( GLenum mode, GLint first, GLsizei count, GLsizei primcount )
      : DrawCommand( DrawArraysInstancedType, mode )
    {
        _first = first;
        _count = count;
        _primcount = primcount;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawArraysInstanced( _mode, _first, _count, _primcount );
    }
};

typedef jagBase::ptr< jagDraw::DrawArraysInstanced >::shared_ptr DrawArraysInstancedPtr;


/** \class DrawArraysIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArraysIndirect : public DrawCommand
{
public:
    DrawArraysIndirect( GLenum mode, const GLvoid* indirect )
      : DrawCommand( DrawArraysIndirectType, mode )
    {
        _indirect = const_cast< GLvoid* >( indirect );
    }

    virtual void operator()( DrawInfo& )
    {
        // TBD Do we need explicit support for DRAW_INDIRECT_BUFFER bundings?
        glDrawArraysIndirect( _mode, _indirect );
    }
};

typedef jagBase::ptr< jagDraw::DrawArraysIndirect >::shared_ptr DrawArraysIndirectPtr;


/** \class MultiDrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class MultiDrawArrays : public DrawCommand
{
public:
    MultiDrawArrays( GLenum mode, const jagBase::GLintArray& first,
            const jagBase::GLsizeiArray& count, GLsizei primcount )
      : DrawCommand( MultiDrawArraysType, mode )
    {
        _firstArray = first;
        _countArray = count;
        _primcount = primcount;
    }

    virtual void operator()( DrawInfo& )
    {
        glMultiDrawArrays( _mode, _firstArray.get(), _countArray.get(), _primcount );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawArrays >::shared_ptr MultiDrawArraysPtr;


/** \class DrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

Note the fourth parameter is \c offset rather than the spec name \c indices.
The intended use is with an element buffer object. If used without an element
buffer object, be aware that DrawElements does not store \c offset in a
smart pointer. */
class DrawElements : public DrawCommand
{
public:
    DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* offset )
      : DrawCommand( DrawElementsType, mode )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawElements( _mode, _count, _type, _offset );
    }
};

typedef jagBase::ptr< jagDraw::DrawElements >::shared_ptr DrawElementsPtr;


/** \class DrawElementsInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

Note the fourth parameter is \c offset rather than the spec name \c indices.
The intended use is with an element buffer object. If used without an element
buffer object, be aware that DrawElements does not store \c offset in a
smart pointer. */
class DrawElementsInstanced : public DrawCommand
{
public:
    DrawElementsInstanced( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* offset, GLsizei primcount )
      : DrawCommand( DrawElementsInstancedType, mode )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
        _primcount = primcount;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawElementsInstanced( _mode, _count, _type, _offset, _primcount );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsInstanced >::shared_ptr DrawElementsInstancedPtr;


/** \class MultiDrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class MultiDrawElements : public DrawCommand
{
public:
    MultiDrawElements( GLenum mode, const jagBase::GLsizeiArray& count, GLenum type,
            const jagBase::GLvoidPtrArray& indices, GLsizei primcount)
      : DrawCommand( MultiDrawElementsType, mode )
    { 
        _countArray = count;
        _type = type;
        _indicesArray = indices;
        _primcount = primcount;
    }

    virtual void operator()( DrawInfo& )
    {
        glMultiDrawElements( _mode, _countArray.get(), _type, (const GLvoid**)( _indicesArray.get() ), _primcount );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawElements >::shared_ptr MultiDrawElementsPtr;


/** \class DrawRangeElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawRangeElements : public DrawCommand
{
public:
    DrawRangeElements( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
            const GLvoid* offset )
      : DrawCommand( DrawRangeElementsType, mode ),
        _start( start ),
        _end( end )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawRangeElements( _mode, _start, _end, _count, _type, _offset );
    }

protected:
    GLuint _start, _end;
};

typedef jagBase::ptr< jagDraw::DrawRangeElements >::shared_ptr DrawRangeElementsPtr;


/** \class DrawElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawElementsBaseVertex : public DrawCommand
{
public:
    DrawElementsBaseVertex( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* offset, GLint basevertex )
      : DrawCommand( DrawElementsBaseVertexType, mode )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
        _basevertex = basevertex;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawElementsBaseVertex( _mode, _count, _type, _offset, _basevertex );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsBaseVertex >::shared_ptr DrawElementsBaseVertexPtr;


/** \class DrawRangeElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawRangeElementsBaseVertex : public DrawCommand
{
public:
    DrawRangeElementsBaseVertex( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
            const GLvoid* offset, GLint basevertex )
      : DrawCommand( DrawRangeElementsBaseVertexType, mode ),
        _start( start ),
        _end( end )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
        _basevertex = basevertex;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawRangeElementsBaseVertex( _mode, _start, _end, _count, _type, _offset, _basevertex );
    }

protected:
    GLuint _start, _end;
};

typedef jagBase::ptr< jagDraw::DrawRangeElementsBaseVertex >::shared_ptr DrawRangeElementsBaseVertexPtr;


/** \class DrawElementsInstancedBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawElementsInstancedBaseVertex : public DrawCommand
{
public:
    DrawElementsInstancedBaseVertex( GLenum mode, GLsizei count, GLenum type,
        const GLvoid* offset, GLsizei primcount, GLint basevertex )
      : DrawCommand( DrawElementsInstancedBaseVertexType, mode )
    {
        _count = count;
        _type = type;
        _offset = const_cast< GLvoid* >( offset );
        _primcount = primcount;
        _basevertex = basevertex;
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawElementsInstancedBaseVertex( _mode, _count, _type, _offset, _primcount, _basevertex );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsInstancedBaseVertex >::shared_ptr DrawElementsInstancedBaseVertexPtr;


/** \class DrawElementsIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawElementsIndirect : public DrawCommand
{
public:
    DrawElementsIndirect( GLenum mode, GLenum type, const GLvoid* indirect )
      : DrawCommand( DrawElementsIndirectType, mode )
    {
        _type = type;
        _indirect = const_cast< GLvoid* >( indirect );
    }

    virtual void operator()( DrawInfo& )
    {
        glDrawElementsIndirect( _mode, _type, _indirect );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsIndirect >::shared_ptr DrawElementsIndirectPtr;


/** \class MultiDrawElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class MultiDrawElementsBaseVertex : public DrawCommand
{
public:
    MultiDrawElementsBaseVertex( GLenum mode, const jagBase::GLsizeiArray& count, 
            GLenum type, const jagBase::GLvoidPtrArray& indices, GLsizei primcount,
            const jagBase::GLintArray& basevertex )
      : DrawCommand( MultiDrawElementsBaseVertexType, mode )
    { 
        _countArray = count;
        _type = type;
        _indicesArray = indices;
        _primcount = primcount;
        _basevertexArray = basevertex;
    }

    virtual void operator()( DrawInfo& )
    {
        glMultiDrawElementsBaseVertex( _mode, _countArray.get(), _type, (const GLvoid**)( _indicesArray.get() ),
            _primcount, _basevertexArray.get() );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawElementsBaseVertex >::shared_ptr MultiDrawElementsBaseVertexPtr;



/** \class DrawCommandList DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawCommandList : public std::vector< DrawCommandPtr >
{
public:
    DrawCommandList()
    {}
    virtual ~DrawCommandList()
    {}

    virtual void operator()( DrawInfo& drawInfo )
    {
        for( iterator p = begin(); p != end(); p++ )
        {
            (**p)( drawInfo );
        }
    }
};


// jagDraw
}


// __JAGDRAW_DRAW_COMMANDS_H__
#endif
