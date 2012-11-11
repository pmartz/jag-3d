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
\brief Retains and executes OpenGL drawing commands.
\details \gl{section 2.8.3}.

To implement a new type of draw command:
\li Add a new value to DrawCommandType (or use one of the existing UserDefined*
values).
\li Derive a class from DrawCommand.
\li Override and define the pure virtual execute() member function.

\specBegin
DrawCommand supports all OpenGL rendering operations. This includes:
\li Traditional drawing commands (\glshort{2.8.3});
\li Primitive restart functionality (\glshort{2.8.1});
\li Clearing operations (\glshort{4.2.3}).

\specTableBegin
\specThread{Context Safe}
\specGL{See derived classes}
\specDepend{See derived classes}
\specUsage{DrawCommand should be attached to Drawable using
the Drawable::addDrawCommand() method. }
\specViolations{Clearing operations (\glshort{4.2.3}) are currently unsupported.}
\specTableEnd

See derived classes for additional requirements.
\specEnd

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


/** \class DrawArraysBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawArrays and DrawArraysInstanced.
\details Provides the DrawArraysBase::_first member variable
used by the derived classes, and associated accessor methods.
*/
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

    /** \brief Set DrawArraysBase::_first.
    \details DrawArraysBase::_first is passed as a parameter to the OpenGL commands
    issued by the derived classes DrawArrays and DrawArraysInstanced.

    \specFuncBegin
    See DrawCommand.

    Member variable defaults:
    \li DrawArraysBase::_first = 0
    \specFuncEnd
    */
    void setFirst( const GLint first ) { _first = first; }
    /** \brief Get DrawArraysBase::_first.
    */
    GLint getFirst() const { return( _first ); }

protected:
    /** Default value: 0 */
    GLint _first;
};

/** \class DrawElementsBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawElements and related classes.
\details Provides the DrawElementsBase::_type,
DrawElementsBase::_indices, and DrawElementsBase::_elementBuffer
member variables used by the derived classes, and associated
accessor methods.
*/
class DrawElementsBase
{
public:
    DrawElementsBase( const GLenum type, const GLvoid* indices=(GLvoid*)0,
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

    /** \brief Set DrawElementsBase::_type.
    \details DrawElementsBase::_type is passed as a parameter to the OpenGL
    glDrawElements*() family of commands.
    */
    void setType( GLenum type ) { _type = type; }
    /** \brief Get DrawElementsBase::_type. */
    GLenum getType() const { return( _type ); }

    /** \brief Set DrawElementsBase::_indices.
    \details DrawElementsBase::_indices is passed as a parameter to the OpenGL
    glDrawElements*() family of commands.

    \specFuncBegin
    See DrawCommand.

    Member variable defaults:
    \li DrawElementsBase::_indices = (GLvoid*)0
    \specFuncEnd
    */
    void setIndices( const GLvoid* indices ) { _indices = indices; }
    /** \brief Get DrawElementsBase::_indices. */
    const GLvoid* getIndices() const { return( _indices ); }

    /** \brief Set DrawElementsBase::_elementBuffer.
    \details If DrawElementsBase::_elementBuffer != NULL, the execute() method
    in DrawElementsBase derived classes call _elementBuffer::execute() prior
    to issuing their OpenGL drawing command.

    \specFuncBegin
    See DrawCommand.

    Member variable defaults:
    \li DrawElementsBase::_elementBuffer = jagDraw::BufferObjectPtr()
    \specFuncEnd
    */
    void setElementBuffer( const jagDraw::BufferObjectPtr elementBuffer )
    {
        _elementBuffer = elementBuffer;
    }
    /** \brief Get DrawElementsBase::_elementBuffer. */
    const jagDraw::BufferObjectPtr getElementBuffer() const
    {
        return( _elementBuffer );
    }

protected:
    /** Initial value must be passed to constructor. */
    GLenum _type;
    /** Default value: (GLvoid*)0 */
    const GLvoid* _indices;

    /** Default value: jagDraw::BufferObjectPtr() */
    jagDraw::BufferObjectPtr _elementBuffer;
};

/** \class BaseVertexBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawCommands that issue gl*BaseVertex().
\details

\specBegin
\specTableBegin
\specThread{ }
\specGL{ }
\specDepend{ }
\specUsage{ }
\specViolations{ }
\specTableEnd
\specEnd

*/
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

/** \class RangeBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawCommands that issue glDrawRange*().

\specBegin
\specTableBegin
\specThread{ }
\specGL{ }
\specDepend{ }
\specUsage{ }
\specViolations{ }
\specTableEnd
\specEnd

*/
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

/** \class MultiArrayBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawCommands that issue glMultiDraw*().

\specBegin
\specTableBegin
\specThread{ }
\specGL{ }
\specDepend{ }
\specUsage{ }
\specViolations{ }
\specTableEnd
\specEnd

*/
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


/** \class IndirectBase DrawCommand.h <jagDraw/DrawCommand.h>
\brief Base class for DrawCommands that issue glDraw*Indirect().

\specBegin
\specTableBegin
\specThread{ }
\specGL{ }
\specDepend{ }
\specUsage{ }
\specViolations{ }
\specTableEnd
\specEnd

*/
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




/** \class DrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawArrays() command.
\details

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    glDrawArrays( _mode\, _first\, _count );
    \endcode }
\specDepend{None}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

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

    /** \brief TBD */
    virtual void execute( DrawInfo& drawInfo )
    {
        glDrawArrays( _mode, _first, _count );
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        return( _first + counter );
    }
};

typedef jagBase::ptr< jagDraw::DrawArrays >::shared_ptr DrawArraysPtr;


/** \class DrawArraysInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawArraysInstance() command.
\details

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    glDrawArraysInstanced( _mode\, _first\, _count\, _primcount );
    \endcode }
\specDepend{None}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

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

    /** \brief TBD */
    virtual void execute( DrawInfo& drawInfo )
    {
        glDrawArraysInstanced( _mode, _first, _count, _primcount );
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        return( _first + counter );
    }
};

typedef jagBase::ptr< jagDraw::DrawArraysInstanced >::shared_ptr DrawArraysInstancedPtr;


/** \class DrawArraysIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawArraysIndirect() command.
\details

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_INDIRECT_BUFFER bind (if necessary; see DrawIndirectBuffer).
    glDrawArraysIndirect( _mode\, _indirect );
    \endcode }
\specDepend{DrawIndirectBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

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
#ifdef GL_VERSION_4_0
        if( _indirectBuffer != NULL )
            _indirectBuffer->execute( drawInfo );
        glDrawArraysIndirect( _mode, _indirect );
#endif
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        GLuint first;
        if( _indirectBuffer != NULL )
        {
            const DrawArraysIndirectCommand* data( ( const DrawArraysIndirectCommand* )
                _indirectBuffer->getBuffer()->ptr() );
            first = data->first;
        }
        else
        {
            first = 0;
        }
        return( first + counter );
    }
    virtual unsigned int getNumIndices() const
    {
        GLuint count;
        if( _indirectBuffer != NULL )
        {
            const DrawArraysIndirectCommand* data( ( const DrawArraysIndirectCommand* )
                _indirectBuffer->getBuffer()->ptr() );
            count = data->count;
        }
        else
        {
            count = 0;
        }
        return( count );
    }

protected:
    typedef struct {
        GLuint count;
        GLuint primCount;
        GLuint first;
        GLuint reservedMustBeZero;
    } DrawArraysIndirectCommand;
};

typedef jagBase::ptr< jagDraw::DrawArraysIndirect >::shared_ptr DrawArraysIndirectPtr;


/** \class MultiDrawArrays DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glMultiDrawArrays() command.
\details

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    glMultiDrawArrays( _mode\, _firstArray.get()\, _countArray.get()\, _primcount );
    \endcode }
\specDepend{DrawIndirectBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

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

    virtual int getIndex( const unsigned int counter ) const
    {
        unsigned int localCounter( counter );
        for( unsigned int idx=0; idx < _primcount; ++idx )
        {
            if( localCounter >= _countArray[ idx ] )
                localCounter -= _countArray[ idx ];
            else
                return( _firstArray[ idx ] + localCounter );
        }
        // TBD Went off the end of the array. I wonder if this might seg fault...
        return( _firstArray[ _primcount - 1 ] + _countArray[ _primcount - 1 ] + 1 );
    }
    virtual unsigned int getNumIndices() const
    {
        unsigned int total( 0 );
        for( unsigned int idx=0; idx < _primcount; ++idx )
        {
            total += _countArray[ idx ];
        }
        return( total );
    }

protected:
    jagDraw::GLintArray _firstArray;
};

typedef jagBase::ptr< jagDraw::MultiDrawArrays >::shared_ptr MultiDrawArraysPtr;


/** \class DrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElements( _mode\, _count\, _type\, _indices );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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

    virtual int getIndex( const unsigned int counter ) const
    {
        if( _elementBuffer != NULL )
        {
            const unsigned char* ptr( ( const unsigned char* ) _elementBuffer->getBuffer()->ptr() + (ptrdiff_t) _indices );
            switch( _type ) {
                case GL_UNSIGNED_BYTE: return( ptr[ counter ] ); break;
                case GL_UNSIGNED_SHORT: return( ((unsigned short*)ptr)[ counter ] ); break;
                case GL_UNSIGNED_INT: return( ((unsigned int*)ptr)[ counter ] ); break;
                default: return( -1 );
            }
        }
        return( -1 );
    }
};

typedef jagBase::ptr< jagDraw::DrawElements >::shared_ptr DrawElementsPtr;


/** \class DrawElementsInstanced DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawElementsInstanced() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElementsInstanced( _mode\, _count\, _type\, _indices\, _primcount );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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

    virtual int getIndex( const unsigned int counter ) const
    {
        if( _elementBuffer != NULL )
        {
            const unsigned char* ptr( ( const unsigned char* ) _elementBuffer->getBuffer()->ptr() + (ptrdiff_t)_indices );
            switch( _type ) {
                case GL_UNSIGNED_BYTE: return( ptr[ counter ] ); break;
                case GL_UNSIGNED_SHORT: return( ((unsigned short*)ptr)[ counter ] ); break;
                case GL_UNSIGNED_INT: return( ((unsigned int*)ptr)[ counter ] ); break;
                default: return( -1 );
            }
        }
        return( -1 );
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsInstanced >::shared_ptr DrawElementsInstancedPtr;


/** \class MultiDrawElements DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glMultiDrawElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glMultiDrawElements( _mode\, _countArray.get()\, _type\, (const GLvoid**)( _indicesArray.get() )\, _primcount );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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
\brief Retained storage for the glDrawRangeElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawRangeElements( _mode\, _start\, _end\, _count\, _type\, _indices );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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
\brief Retained storage for the glDrawElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElementsBaseVertex( _mode\, _count\, _type\, _indices\, _baseVertex );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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
\brief Retained storage for the glDrawRangeElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawRangeElementsBaseVertex( _mode\, _start\, _end\, _count\, _type\, _indices\, _baseVertex );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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
\brief Retained storage for the glDrawElementsInstancedBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElementsInstancedBaseVertex( _mode\, _count\, _type\, _indices\, _primcount\, _baseVertex );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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


/** \class DrawElementsIndirect DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glDrawElementsIndirect() command.
\details

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    // GL_INDIRECT_BUFFER bind (if necessary; see DrawIndirectBuffer).
    glDrawElementsIndirect( _mode\, _type\, _indirect );
    \endcode }
\specDepend{ElementArrayBuffer\, DrawIndirectBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

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
#ifdef GL_VERSION_4_0
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        if( _indirectBuffer != NULL )
            _indirectBuffer->execute( drawInfo );
        glDrawElementsIndirect( _mode, _type, _indirect );
#endif
    }
};

typedef jagBase::ptr< jagDraw::DrawElementsIndirect >::shared_ptr DrawElementsIndirectPtr;


/** \class MultiDrawElementsBaseVertex DrawCommand.h <jagDraw/DrawCommand.h>
\brief Retained storage for the glMultiDrawElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glMultiDrawElementsBaseVertex( _mode\, _countArray.get()\, _type\, (const GLvoid**)( _indicesArray.get() )\,
            _primcount\, _basevertexArray.get() );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
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
\brief Retained storage for the glPrimitiveRestart() command.
\details
\gl{section 2.8.1}.

\specBegin
\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():<br>
If enabled:
    \code
    glEnable( GL_PRIMITIVE_RESTART );
    glPrimitiveRestartIndex( _index );
    \endcode
If disabled:
    \code
    glDisable( GL_PRIMITIVE_RESTART );
    \endcode }
\specDepend{None}
\specUsage{In typical usage\, client code will attach two instance of PrimitiveRestart\,
one to enable the feature and one to disable it\, and attach them to the same Drawable
with intervening traditional drawing commands (such as DrawElements). }
\specViolations{None}
\specTableEnd
\specEnd

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
