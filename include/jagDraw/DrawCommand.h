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
\details \gl{section 2.8.3}.

To implement a new type of draw command:
\li Add a new value to DrawCommandType (or use one of the existing UserDefined*
values).
\li Derive a class from DrawCommand.
\li Override and define the pure virtual operator() member function.
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
        MultiDrawElementsBaseVertexType,

        // Commands that are added to Drawable list of DrawCommands, but
        // don't actually do any drawing, just set up work, state changes, etc.
        PrimitiveRestartEnableType,
        PrimitiveRestartDisableType,
        VertexArrayObjectUnbindType,

        // User defined types, future use, etc.
        UserDefined0,
        UserDefined1,
        UserDefined2,
        UserDefined3,
        UserDefined4,
        UserDefined5,
        UserDefined6,
        UserDefined7
    };

    DrawCommand( DrawCommandType drawCommandType, GLenum mode=GL_POINTS )
      : _drawCommandType( drawCommandType ),
        _mode( mode )
    {}
    virtual ~DrawCommand() 
    {}

    DrawCommandType getType() { return _drawCommandType; }

    /**
    */
    virtual void operator()( DrawInfo& ) = 0;

protected:
    DrawCommandType _drawCommandType;
    GLenum _mode;

    // TBD I really do not like having all this crap in the base class.
    // Possibly have multiple base classes employing multiple inheritance
    // to access such member variables.
    GLint _first;
    GLsizei _count;
    GLsizei _primcount;
    GLenum _type;
    GLvoid* _offset;
    GLuint _start, _end;
    GLvoid* _indirect;
    jagBase::GLintArray _firstArray;
    jagBase::GLsizeiArray _countArray;
    jagBase::GLvoidPtrArray _indicesArray;
    GLint _basevertex;
    jagBase::GLintArray _basevertexArray;
};

typedef jagBase::ptr< jagDraw::DrawCommand >::shared_ptr DrawCommandPtr;
typedef std::vector< DrawCommandPtr > DrawCommandList;


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

    virtual void operator()( DrawInfo& drawInfo )
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

    virtual void operator()( DrawInfo& drawInfo )
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

    virtual void operator()( DrawInfo& drawInfo )
    {
        // TBD Do we need explicit support for GL_DRAW_INDIRECT_BUFFER bindings?
        // Possible support that with a generic DrawCommand.
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

    virtual void operator()( DrawInfo& drawInfo )
    {
        glMultiDrawArrays( _mode, _firstArray.get(), _countArray.get(), _primcount );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawArrays >::shared_ptr MultiDrawArraysPtr;


/** \class DrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

Note the fourth parameter is \c offset rather than the spec name \c indices.
The intended use is with GL_ELEMENT_ARRAY_BUFFER. If used without an element
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

    virtual void operator()( DrawInfo& drawInfo )
    {
        glDrawElements( _mode, _count, _type, _offset );
    }
};

typedef jagBase::ptr< jagDraw::DrawElements >::shared_ptr DrawElementsPtr;


/** \class DrawElementsInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

Note the fourth parameter is \c offset rather than the spec name \c indices.
The intended use is with GL_ELEMENT_ARRAY_BUFFER. If used without an element
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

    virtual void operator()( DrawInfo& drawInfo )
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

    virtual void operator()( DrawInfo& drawInfo )
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
      : DrawCommand( DrawRangeElementsType, mode )
    {
        _count = count;
        _type = type;
        _start = start;
        _end = end;
        _offset = const_cast< GLvoid* >( offset );
    }

    virtual void operator()( DrawInfo& drawInfo )
    {
        glDrawRangeElements( _mode, _start, _end, _count, _type, _offset );
    }
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

    virtual void operator()( DrawInfo& drawInfo )
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
      : DrawCommand( DrawRangeElementsBaseVertexType, mode )
    {
        _count = count;
        _type = type;
        _start = start;
        _end = end;
        _offset = const_cast< GLvoid* >( offset );
        _basevertex = basevertex;
    }

    virtual void operator()( DrawInfo& drawInfo )
    {
        glDrawRangeElementsBaseVertex( _mode, _start, _end, _count, _type, _offset, _basevertex );
    }
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

    virtual void operator()( DrawInfo& drawInfo )
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

    virtual void operator()( DrawInfo& drawInfo )
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

    virtual void operator()( DrawInfo& drawInfo )
    {
        glMultiDrawElementsBaseVertex( _mode, _countArray.get(), _type, (const GLvoid**)( _indicesArray.get() ),
            _primcount, _basevertexArray.get() );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawElementsBaseVertex >::shared_ptr MultiDrawElementsBaseVertexPtr;


/** \class PrimitiveRestartEnable DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
\gl{section 2.8.1}.
*/
class PrimitiveRestartEnable : public DrawCommand
{
public:
    PrimitiveRestartEnable( GLuint index=0 )
      : DrawCommand( PrimitiveRestartEnableType ),
        _index( index )
    { 
    }

    virtual void operator()( DrawInfo& )
    {
        glEnable( GL_PRIMITIVE_RESTART );
        glPrimitiveRestartIndex( _index );
    }

    void setRestartIndex( GLuint index )
    {
        _index = index;
    }
    GLuint getRestartIndex() const
    {
        return( _index );
    }

protected:
    GLuint _index;
};

typedef jagBase::ptr< PrimitiveRestartEnable >::shared_ptr PrimitiveRestartEnablePtr;


/** \class PrimitiveRestartDisable DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
\gl{section 2.8.1}.
*/
class PrimitiveRestartDisable : public DrawCommand
{
public:
    PrimitiveRestartDisable()
      : DrawCommand( PrimitiveRestartDisableType )
    { 
    }

    virtual void operator()( DrawInfo& )
    {
        glDisable( GL_PRIMITIVE_RESTART );
    }
};

typedef jagBase::ptr< PrimitiveRestartDisable >::shared_ptr PrimitiveRestartDisablePtr;


/** \class VertexArrayObjectUnbind DrawCommand.h <jagDraw/DrawCommand.h>
\brief Binds the default vertex array object buffer.
\details If client code attaches a VertexArrayObject vertex array command to
a Drawable, then the client code can attach an instance of the
VertexArrayObjectUnbind DrawCommand to bind the default vertex array object.
*/
class VertexArrayObjectUnbind : public DrawCommand
{
public:
    VertexArrayObjectUnbind()
      : DrawCommand( VertexArrayObjectUnbindType )
    { 
    }

    virtual void operator()( DrawInfo& )
    {
        glBindVertexArray( 0 );
    }
};

typedef jagBase::ptr< VertexArrayObjectUnbind >::shared_ptr VertexArrayObjectUnbindPtr;


// jagDraw
}


// __JAGDRAW_DRAW_COMMANDS_H__
#endif
