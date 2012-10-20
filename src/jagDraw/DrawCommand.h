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
#include <jagDraw/BufferObject.h>
#include <jagDraw/ObjectID.h>
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
class DrawCommand : public ObjectIDOwner
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
        PrimitiveRestartType,

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

    DrawCommand( const DrawCommandType drawCommandType, const GLenum mode=GL_POINTS, const GLsizei count=0, const GLsizei primcount=0 )
      : ObjectIDOwner(),
        _drawCommandType( drawCommandType ),
        _mode( mode ),
        _count( count ),
        _primcount( primcount )
    {}
    DrawCommand( const DrawCommand& rhs )
      : ObjectIDOwner( rhs ),
        _drawCommandType( rhs._drawCommandType ),
        _mode( rhs._mode ),
        _count( rhs._count ),
        _primcount( rhs._primcount )
    {}
    virtual ~DrawCommand() 
    {}


    /**
    */
    virtual void execute( DrawInfo& ) = 0;


    /**
    */
    DrawCommandType getDrawCommandType() { return _drawCommandType; }

    void setMode( const GLenum mode ) { _mode = mode; }
    GLenum getMode() const { return( _mode ); }

    void setCount( const GLsizei count ) { _count = count; }
    GLsizei getCount() const { return( _count ); }

    void setPrimcount( const GLsizei primcount ) { _primcount = primcount; }
    GLsizei getPrimcount() const { return( _primcount ); }


    /** \name Element Access
    \details Used by BufferIterator to access elements of a BufferObject used by a VertexAttrib.
    */
    /**@{*/
    virtual int getIndex( const unsigned int counter ) const
    {
        return( counter );
    }
    virtual unsigned int getNumIndices() const
    {
        return( _count );
    }
    /**@}*/

protected:
    DrawCommandType _drawCommandType;

    GLenum _mode;
    GLsizei _count;
    GLsizei _primcount;
};

typedef jagBase::ptr< jagDraw::DrawCommand >::shared_ptr DrawCommandPtr;
typedef std::vector< DrawCommandPtr > DrawCommandVec;


class DrawArraysBase
{
public:
    DrawArraysBase( const GLint first )
      : _first( first )
    {}
    DrawArraysBase( const DrawArraysBase& rhs )
      : _first( rhs._first )
    {}
    virtual ~DrawArraysBase()
    {}

    void setFirst( const GLint first ) { _first = first; }
    GLint getFirst() const { return( _first ); }

protected:
    GLint _first;
};

class DrawElementsBase
{
public:
    DrawElementsBase( const GLenum type, const GLvoid* indices=NULL,
                const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : _type( type ),
        _indices( indices ),
        _elementBuffer( elementBuffer )
    {}
    DrawElementsBase( const DrawElementsBase& rhs )
      : _type( rhs._type ),
        _indices( rhs._indices ),
        _elementBuffer( rhs._elementBuffer )
    {}
    virtual ~DrawElementsBase() {}

    void setType( GLenum type ) { _type = type; }
    GLenum getType() const { return( _type ); }

    void setIndices( const GLvoid* indices ) { _indices = indices; }
    const GLvoid* getIndices() const { return( _indices ); }

    void setElementBuffer( const jagDraw::BufferObjectPtr elementBuffer )
    {
        _elementBuffer = elementBuffer;
    }
    const jagDraw::BufferObjectPtr getElementBuffer() const
    {
        return( _elementBuffer );
    }

protected:
    GLenum _type;
    const GLvoid* _indices;

    jagDraw::BufferObjectPtr _elementBuffer;
};

class BaseVertexBase
{
public:
    BaseVertexBase( const GLint baseVertex )
      : _baseVertex( baseVertex )
    {}
    BaseVertexBase( const BaseVertexBase& rhs )
      : _baseVertex( rhs._baseVertex )
    {}
    virtual ~BaseVertexBase()
    {}

    void setBaseVertex( const GLint baseVertex ) { _baseVertex = baseVertex; }
    GLint getBaseVertex() const { return( _baseVertex ); }

protected:
    GLint _baseVertex;
};

class RangeBase
{
public:
    RangeBase( const GLuint start, const GLuint end )
      : _start( start ),
        _end( end )
    {}
    RangeBase( const RangeBase& rhs )
      : _start( rhs._start ),
        _end( rhs._end )
    {}
    virtual ~RangeBase()
    {}

    void setStart( const GLuint start ) { _start = start; }
    GLuint getStart() const { return( _start ); }

    void setEnd( const GLuint end ) { _end = end; }
    GLuint getEnd() const { return( _end ); }

protected:
    GLuint _start, _end;
};

class MultiArrayBase
{
public:
    MultiArrayBase( const jagDraw::GLsizeiArray countArray,
                const jagDraw::GLvoidPtrArray indicesArray=jagDraw::GLvoidPtrArray() )
      : _countArray( countArray ),
        _indicesArray( indicesArray )
    {}
    MultiArrayBase( const MultiArrayBase& rhs )
      : _countArray( rhs._countArray ),
        _indicesArray( rhs._indicesArray )
    {}
    virtual ~MultiArrayBase()
    {}


    void setCountArray( const jagDraw::GLsizeiArray countArray )
    {
        _countArray = countArray;
    }
    jagDraw::GLsizeiArray& getCountArray() { return( _countArray ); }
    const jagDraw::GLsizeiArray& getCountArray() const { return( _countArray ); }

    void setIndicesArray( const jagDraw::GLvoidPtrArray indicesArray )
    {
        _indicesArray = indicesArray;
    }
    jagDraw::GLvoidPtrArray& getIndicesArray() { return( _indicesArray ); }
    const jagDraw::GLvoidPtrArray& getIndicesArray() const { return( _indicesArray ); }

protected:
    jagDraw::GLsizeiArray _countArray;
    jagDraw::GLvoidPtrArray _indicesArray;
};

#ifdef GL_VERSION_4_0
class IndirectBase
{
public:
    IndirectBase( const GLvoid* indirect=NULL,
                const jagDraw::BufferObjectPtr indirectBuffer=jagDraw::BufferObjectPtr() )
      : _indirect( indirect ),
        _indirectBuffer( indirectBuffer )
    {}
    IndirectBase( const IndirectBase& rhs )
      : _indirect( rhs._indirect ),
        _indirectBuffer( rhs._indirectBuffer )
    {}
    virtual ~IndirectBase()
    {}

    void setIndirect( const GLvoid* indirect ) { _indirect = indirect; }
    const GLvoid* getIndirect() const { return( _indirect ); }

    void setIndirectBuffer( const jagDraw::BufferObjectPtr indirectBuffer )
    {
        _indirectBuffer = indirectBuffer;
    }
    const jagDraw::BufferObjectPtr getIndirectBuffer() const
    {
        return( _indirectBuffer );
    }

protected:
    const GLvoid* _indirect;

    jagDraw::BufferObjectPtr _indirectBuffer;
};
#endif


/** \class DrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArrays : public DrawCommand,
            public DrawArraysBase
{
public:
    DrawArrays( const GLenum mode, const GLint first, const GLsizei count )
      : DrawCommand( DrawArraysType, mode, count ),
        DrawArraysBase( first )
    {}
    DrawArrays( const DrawArrays& rhs )
      : DrawCommand( rhs ),
        DrawArraysBase( rhs )
    {}
    virtual ~DrawArrays()
    {}

    virtual void execute( DrawInfo& drawInfo )
    {
        glDrawArrays( _mode, _first, _count );
    }
};

typedef jagBase::ptr< jagDraw::DrawArrays >::shared_ptr DrawArraysPtr;


/** \class DrawArraysInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArraysInstanced : public DrawCommand,
            public DrawArraysBase
{
public:
    DrawArraysInstanced( const GLenum mode, const GLint first, const GLsizei count, const GLsizei primcount )
      : DrawCommand( DrawArraysInstancedType, mode, count, primcount ),
        DrawArraysBase( first )
    {}
    DrawArraysInstanced( const DrawArraysInstanced& rhs )
      : DrawCommand( rhs ),
        DrawArraysBase( rhs )
    {}
    virtual ~DrawArraysInstanced()
    {}

    virtual void execute( DrawInfo& drawInfo )
    {
        glDrawArraysInstanced( _mode, _first, _count, _primcount );
    }
};

typedef jagBase::ptr< jagDraw::DrawArraysInstanced >::shared_ptr DrawArraysInstancedPtr;


#ifdef GL_VERSION_4_0
/** \class DrawArraysIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawArraysIndirect : public DrawCommand,
            public IndirectBase
{
public:
    DrawArraysIndirect( const GLenum mode, const GLvoid* indirect=0,
                const jagDraw::BufferObjectPtr indirectBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawArraysIndirectType, mode ),
        IndirectBase( indirect, indirectBuffer )
    {}
    DrawArraysIndirect( const DrawArraysIndirect& rhs )
      : DrawCommand( rhs ),
        IndirectBase( rhs )
    {}
    virtual ~DrawArraysIndirect()
    {}

    /** \brief Tell the indirect buffer object how many contexts to expect. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _indirectBuffer != NULL )
            _indirectBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _indirectBuffer != NULL )
            _indirectBuffer->execute( drawInfo );
        glDrawArraysIndirect( _mode, _indirect );
    }
};

typedef jagBase::ptr< jagDraw::DrawArraysIndirect >::shared_ptr DrawArraysIndirectPtr;
#endif

/** \class MultiDrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class MultiDrawArrays : public DrawCommand,
            public MultiArrayBase
{
public:
    MultiDrawArrays( const GLenum mode, const jagDraw::GLintArray& first,
            const jagDraw::GLsizeiArray& count, const GLsizei primcount )
      : DrawCommand( MultiDrawArraysType, mode, 0, primcount ),
        MultiArrayBase( count ),
        _firstArray( first )
    {}
    MultiDrawArrays( const MultiDrawArrays& rhs )
      : DrawCommand( rhs ),
        MultiArrayBase( rhs ),
        _firstArray( rhs._firstArray )
    {}
    virtual ~MultiDrawArrays()
    {}

    virtual void execute( DrawInfo& drawInfo )
    {
        glMultiDrawArrays( _mode, _firstArray.get(), _countArray.get(), _primcount );
    }

protected:
    jagDraw::GLintArray _firstArray;
};

typedef jagBase::ptr< jagDraw::MultiDrawArrays >::shared_ptr MultiDrawArraysPtr;


/** \class DrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawElements : public DrawCommand,
            public DrawElementsBase
{
public:
    DrawElements( const GLenum mode, const GLsizei count, const GLenum type, const GLvoid* indices,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawElementsType, mode, count ),
        DrawElementsBase( type, indices, elementBuffer )
    {}
    DrawElements( const DrawElements& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs )
    {}
    virtual ~DrawElements() {}

    /** \brief Tell the element buffer object how many contexts to expect. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElements( _mode, _count, _type, _indices );
    }
};

typedef jagBase::ptr< jagDraw::DrawElements >::shared_ptr DrawElementsPtr;


/** \class DrawElementsInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawElementsInstanced : public DrawCommand,
            public DrawElementsBase
{
public:
    DrawElementsInstanced( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* indices, GLsizei primcount,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawElementsInstancedType, mode, count, primcount ),
        DrawElementsBase( type, indices, elementBuffer )
    {}
    DrawElementsInstanced( const DrawElementsInstanced& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs )
    {}
    virtual ~DrawElementsInstanced()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsInstanced( _mode, _count, _type, _indices, _primcount );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsInstanced >::shared_ptr DrawElementsInstancedPtr;


/** \class MultiDrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class MultiDrawElements : public DrawCommand,
            public DrawElementsBase, public MultiArrayBase
{
public:
    MultiDrawElements( GLenum mode, const jagDraw::GLsizeiArray& count, GLenum type,
            const jagDraw::GLvoidPtrArray& indices, GLsizei primcount,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( MultiDrawElementsType, mode, 0, primcount ),
        DrawElementsBase( type, 0, elementBuffer ),
        MultiArrayBase( count, indices )
    {}
    MultiDrawElements( const MultiDrawElements& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        MultiArrayBase( rhs )
    {}
    virtual ~MultiDrawElements()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glMultiDrawElements( _mode, _countArray.get(), _type, (const GLvoid**)( _indicesArray.get() ), _primcount );
    }
};

typedef jagBase::ptr< jagDraw::MultiDrawElements >::shared_ptr MultiDrawElementsPtr;


/** \class DrawRangeElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawRangeElements : public DrawCommand,
            public DrawElementsBase, public RangeBase
{
public:
    DrawRangeElements( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
            const GLvoid* offset,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawRangeElementsType, mode, count ),
        DrawElementsBase( type, offset, elementBuffer ),
        RangeBase( start, end )
    {}
    DrawRangeElements( const DrawRangeElements& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        RangeBase( rhs )
    {}
    virtual ~DrawRangeElements()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawRangeElements( _mode, _start, _end, _count, _type, _indices );
    }
};

typedef jagBase::ptr< jagDraw::DrawRangeElements >::shared_ptr DrawRangeElementsPtr;


/** \class DrawElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawElementsBaseVertex : public DrawCommand,
            public DrawElementsBase, public BaseVertexBase
{
public:
    DrawElementsBaseVertex( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* offset, GLint baseVertex,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawElementsBaseVertexType, mode, count ),
        DrawElementsBase( type, offset, elementBuffer ),
        BaseVertexBase( baseVertex )
    {}
    DrawElementsBaseVertex( const DrawElementsBaseVertex& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        BaseVertexBase( rhs )
    {}
    virtual ~DrawElementsBaseVertex()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsBaseVertex( _mode, _count, _type, _indices, _baseVertex );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsBaseVertex >::shared_ptr DrawElementsBaseVertexPtr;


/** \class DrawRangeElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawRangeElementsBaseVertex : public DrawCommand,
            public DrawElementsBase, public RangeBase, public BaseVertexBase
{
public:
    DrawRangeElementsBaseVertex( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
            const GLvoid* offset, GLint baseVertex,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawRangeElementsBaseVertexType, mode, count ),
        DrawElementsBase( type, offset, elementBuffer ),
        RangeBase( start, end ),
        BaseVertexBase( baseVertex )
    {}
    DrawRangeElementsBaseVertex( const DrawRangeElementsBaseVertex& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        RangeBase( rhs ),
        BaseVertexBase( rhs )
    {}
    virtual ~DrawRangeElementsBaseVertex()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawRangeElementsBaseVertex( _mode, _start, _end, _count, _type, _indices, _baseVertex );
    }
};

typedef jagBase::ptr< jagDraw::DrawRangeElementsBaseVertex >::shared_ptr DrawRangeElementsBaseVertexPtr;


/** \class DrawElementsInstancedBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class DrawElementsInstancedBaseVertex : public DrawCommand,
            public DrawElementsBase, public BaseVertexBase
{
public:
    DrawElementsInstancedBaseVertex( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* offset, GLsizei primcount, GLint baseVertex,
            const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawElementsInstancedBaseVertexType, mode, count, primcount ),
        DrawElementsBase( type, offset, elementBuffer ),
        BaseVertexBase( baseVertex )
    {}
    DrawElementsInstancedBaseVertex( const DrawElementsInstancedBaseVertex& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        BaseVertexBase( rhs )
    {}
    virtual ~DrawElementsInstancedBaseVertex()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsInstancedBaseVertex( _mode, _count, _type, _indices, _primcount, _baseVertex );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsInstancedBaseVertex >::shared_ptr DrawElementsInstancedBaseVertexPtr;


#ifdef GL_VERSION_4_0
/** \class DrawElementsIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
*/
class DrawElementsIndirect : public DrawCommand,
            public DrawElementsBase, public IndirectBase
{
public:
    DrawElementsIndirect( GLenum mode, GLenum type, const GLvoid* indirect,
                const jagDraw::BufferObjectPtr elementBuffer=jagDraw::BufferObjectPtr(),
                const jagDraw::BufferObjectPtr indirectBuffer=jagDraw::BufferObjectPtr() )
      : DrawCommand( DrawElementsIndirectType, mode ),
        DrawElementsBase( type, 0, elementBuffer ),
        IndirectBase( indirect, indirectBuffer )
    {}
    DrawElementsIndirect( const DrawElementsIndirect& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        IndirectBase( rhs )
    {}
    virtual ~DrawElementsIndirect()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
        if( _indirectBuffer != NULL )
            _indirectBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        if( _indirectBuffer != NULL )
            _indirectBuffer->execute( drawInfo );
        glDrawElementsIndirect( _mode, _type, _indirect );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsIndirect >::shared_ptr DrawElementsIndirectPtr;
#endif

/** \class MultiDrawElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER. */
class MultiDrawElementsBaseVertex : public DrawCommand,
            public DrawElementsBase, public MultiArrayBase
{
public:
    MultiDrawElementsBaseVertex( GLenum mode, const jagDraw::GLsizeiArray& count, 
            GLenum type, const jagDraw::GLvoidPtrArray& indices, GLsizei primcount,
            const jagDraw::GLintArray& basevertex )
      : DrawCommand( MultiDrawElementsBaseVertexType, mode, 0, primcount ),
        DrawElementsBase( type ),
        MultiArrayBase( count, indices ),
        _basevertexArray( basevertex )
    {}
    MultiDrawElementsBaseVertex( const MultiDrawElementsBaseVertex& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        MultiArrayBase( rhs )
    {
        _basevertexArray = rhs._basevertexArray;
    }
    virtual ~MultiDrawElementsBaseVertex()
    {}

    /** \brief Tell the DrawCommand how many contexts to expect.
    \details Invokes setMaxContexts() on any BufferObjects that the DrawCommand owns. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->setMaxContexts( numContexts );
    }

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glMultiDrawElementsBaseVertex( _mode, _countArray.get(), _type, (const GLvoid**)( _indicesArray.get() ),
            _primcount, _basevertexArray.get() );
    }

protected:
    jagDraw::GLintArray _basevertexArray;
};

typedef jagBase::ptr< jagDraw::MultiDrawElementsBaseVertex >::shared_ptr MultiDrawElementsBaseVertexPtr;


/** \class PrimitiveRestart DrawCommand.h <jagDraw/DrawCommand.h>
\brief
\details
\gl{section 2.8.1}.
*/
class PrimitiveRestart : public DrawCommand
{
public:
    PrimitiveRestart( GLuint index=0, bool enable=true )
      : DrawCommand( PrimitiveRestartType ),
        _enable( enable ),
        _index( index )
    {}
    PrimitiveRestart( const PrimitiveRestart& rhs )
      : DrawCommand( rhs ),
        _enable( rhs._enable ),
        _index( rhs._index )
    {}
    virtual ~PrimitiveRestart()
    {}

    virtual void execute( DrawInfo& )
    {
        if( _enable )
        {
            glEnable( GL_PRIMITIVE_RESTART );
            glPrimitiveRestartIndex( _index );
        }
        else
        {
            glDisable( GL_PRIMITIVE_RESTART );
        }
    }

    void setEnable( bool enable=true ) { _enable = enable; }
    bool getEnable() const { return( _enable ); }

    void setRestartIndex( GLuint index ) { _index = index; }
    GLuint getRestartIndex() const { return( _index ); }

protected:
    bool _enable;
    GLuint _index;
};

typedef jagBase::ptr< PrimitiveRestart >::shared_ptr PrimitiveRestartPtr;


// jagDraw
}


// __JAGDRAW_DRAW_COMMANDS_H__
#endif
