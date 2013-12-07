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

#ifndef __JAG_DRAW_DRAW_COMMANDS_H__
#define __JAG_DRAW_DRAW_COMMANDS_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/ObjectID.h>
#include <jag/base/types.h>


namespace jag {
namespace draw {


// Forward declarations
struct DrawInfo;


/** \class DrawCommand DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retains and executes OpenGL drawing commands.
\details \gl{section 2.8.3}.

To implement a new type of draw command:
\li Add a new value to DrawCommandType (or use one of the existing UserDefined*
values).
\li Derive a class from DrawCommand.
\li Override and define the pure virtual execute() member function.

\specBegin DrawCommand

DrawCommand supports all OpenGL rendering operations. This includes:
\li Traditional drawing commands (\glshort{2.8.3});
\li Primitive restart functionality (\glshort{2.8.1});
\li Clearing operations (\glshort{4.2.3}).

\specTableBegin
\specLog{DrawCommand does not use jag::base::LogBase}
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
        DrawArraysInstancedBaseInstanceType,
        DrawArraysInstancedType,
        DrawArraysIndirectType,
        MultiDrawArraysType,
        DrawElementsType,
        DrawElementsInstancedBaseInstanceType,
        DrawElementsInstancedType,
        MultiDrawElementsType,
        DrawRangeElementsType,
        DrawElementsBaseVertexType,
        DrawRangeElementsBaseVertexType,
        DrawElementsInstancedBaseVertexType,
        DrawElementsInstancedBaseVertexBaseInstanceType,
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


    virtual void execute( DrawInfo& ) = 0;


    /** \name TypeDetermination Draw Command Type Determination
    \details Quick access to the draw command type.
    */
    /**@{*/

    /** \brief Get the DrawCommandType.
    */
    DrawCommandType getDrawCommandType() { return _drawCommandType; }

    /**@}*/


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

typedef jag::base::ptr< jag::draw::DrawCommand >::shared_ptr DrawCommandPtr;
typedef std::vector< DrawCommandPtr > DrawCommandVec;


/** \class DrawArraysBase DrawCommand.h <jag/draw/DrawCommand.h>
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

/** \class DrawElementsBase DrawCommand.h <jag/draw/DrawCommand.h>
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
                const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    \li DrawElementsBase::_elementBuffer = jag::draw::BufferObjectPtr()
    \specFuncEnd
    */
    void setElementBuffer( const jag::draw::BufferObjectPtr elementBuffer )
    {
        _elementBuffer = elementBuffer;
    }
    /** \brief Get DrawElementsBase::_elementBuffer. */
    const jag::draw::BufferObjectPtr getElementBuffer() const
    {
        return( _elementBuffer );
    }

protected:
    /** Initial value must be passed to constructor. */
    GLenum _type;
    /** Default value: (GLvoid*)0 */
    const GLvoid* _indices;

    /** Default value: jag::draw::BufferObjectPtr() */
    jag::draw::BufferObjectPtr _elementBuffer;
};

/** \class BaseVertexBase DrawCommand.h <jag/draw/DrawCommand.h>
\brief Base class for DrawCommands that issue gl*BaseVertex().
\details

\specBegin BaseVertexBase

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

/** \class BaseInstanceBase DrawCommand.h <jag/draw/DrawCommand.h>
\brief Base class for DrawCommands that issue gl*BaseInstance().
\details

\specBegin BaseInstanceBase

\specTableBegin
\specThread{ }
\specGL{ }
\specDepend{ }
\specUsage{ }
\specViolations{ }
\specTableEnd
\specEnd

*/
class BaseInstanceBase
{
public:
    BaseInstanceBase( const GLuint baseInstance )
      : _baseInstance( baseInstance )
    {}
    BaseInstanceBase( const BaseInstanceBase& rhs )
      : _baseInstance( rhs._baseInstance )
    {}
    virtual ~BaseInstanceBase()
    {}

    void setBaseInstance( const GLuint baseInstance ) { _baseInstance = baseInstance; }
    GLuint getBaseInstance() const { return( _baseInstance ); }

protected:
    GLuint _baseInstance;
};

/** \class RangeBase DrawCommand.h <jag/draw/DrawCommand.h>
\brief Base class for DrawCommands that issue glDrawRange*().

\specBegin RangeBase

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

/** \class MultiArrayBase DrawCommand.h <jag/draw/DrawCommand.h>
\brief Base class for DrawCommands that issue glMultiDraw*().

\specBegin MultiArrayBase

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
    MultiArrayBase( const jag::draw::GLsizeiVec countVec,
                const jag::draw::GLvoidPtrVec indicesVec=jag::draw::GLvoidPtrVec() )
      : _countVec( countVec ),
        _indicesVec( indicesVec )
    {}
    MultiArrayBase( const MultiArrayBase& rhs )
      : _countVec( rhs._countVec ),
        _indicesVec( rhs._indicesVec )
    {}
    virtual ~MultiArrayBase()
    {}


    void setCountArray( const jag::draw::GLsizeiVec countVec )
    {
        _countVec = countVec;
    }
    jag::draw::GLsizeiVec& getCountArray() { return( _countVec ); }
    const jag::draw::GLsizeiVec& getCountArray() const { return( _countVec ); }

    void setIndices( const jag::draw::GLvoidPtrVec indicesVec )
    {
        _indicesVec = indicesVec;
    }
    jag::draw::GLvoidPtrVec& getIndices() { return( _indicesVec ); }
    const jag::draw::GLvoidPtrVec& getIndices() const { return( _indicesVec ); }

protected:
    jag::draw::GLsizeiVec _countVec;
    jag::draw::GLvoidPtrVec _indicesVec;
};


/** \class IndirectBase DrawCommand.h <jag/draw/DrawCommand.h>
\brief Base class for DrawCommands that issue glDraw*Indirect().

\specBegin IndirectBase

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
                const jag::draw::BufferObjectPtr indirectBuffer=jag::draw::BufferObjectPtr() )
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

    void setIndirectBuffer( const jag::draw::BufferObjectPtr indirectBuffer )
    {
        _indirectBuffer = indirectBuffer;
    }
    const jag::draw::BufferObjectPtr getIndirectBuffer() const
    {
        return( _indirectBuffer );
    }

protected:
    const GLvoid* _indirect;

    jag::draw::BufferObjectPtr _indirectBuffer;
};




/** \class DrawArrays DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawArrays() command.
\details

\specBegin DrawArrays

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

typedef jag::base::ptr< jag::draw::DrawArrays >::shared_ptr DrawArraysPtr;


/** \class DrawArraysInstancedBaseInstance DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawArraysInstanceBaseInstance() command.
\details

\specBegin DrawArraysInstancedBaseInstance

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    glDrawArraysInstancedBaseInstance( _mode\, _first\, _count\, _primcount, _baseInstance );
    \endcode }
\specDepend{None}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
class DrawArraysInstancedBaseInstance : public DrawCommand,
            public DrawArraysBase, public BaseInstanceBase
{
public:
    DrawArraysInstancedBaseInstance( const GLenum mode, const GLint first, const GLsizei count, const GLsizei primcount, const GLuint baseInstance )
      : DrawCommand( DrawArraysInstancedBaseInstanceType, mode, count, primcount ),
        DrawArraysBase( first ),
        BaseInstanceBase( baseInstance )
    {}
    DrawArraysInstancedBaseInstance( const DrawArraysInstancedBaseInstance& rhs )
      : DrawCommand( rhs ),
        DrawArraysBase( rhs ),
        BaseInstanceBase( rhs )
    {}
    virtual ~DrawArraysInstancedBaseInstance()
    {}

    /** \brief TBD */
    virtual void execute( DrawInfo& drawInfo )
    {
#ifdef GL_VERSION_4_2
        glDrawArraysInstancedBaseInstance( _mode, _first, _count, _primcount, _baseInstance );
#endif
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        return( _first + counter );
    }
};

typedef jag::base::ptr< jag::draw::DrawArraysInstancedBaseInstance >::shared_ptr DrawArraysInstancedBaseInstancePtr;


/** \class DrawArraysInstanced DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawArraysInstance() command.
\details

\specBegin DrawArraysInstanced

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

typedef jag::base::ptr< jag::draw::DrawArraysInstanced >::shared_ptr DrawArraysInstancedPtr;


/** \class DrawArraysIndirect DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawArraysIndirect() command.
\details

\specBegin DrawArraysIndirect

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
                const jag::draw::BufferObjectPtr indirectBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

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
                _indirectBuffer->getBuffer()->getData() );
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
                _indirectBuffer->getBuffer()->getData() );
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

typedef jag::base::ptr< jag::draw::DrawArraysIndirect >::shared_ptr DrawArraysIndirectPtr;


/** \class MultiDrawArrays DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glMultiDrawArrays() command.
\details

\specBegin MultiDrawArrays

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    glMultiDrawArrays( _mode\, &_firstVec[ 0 ]\, &_countVec[ 0 ]\, _primcount );
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
    MultiDrawArrays( const GLenum mode, const jag::draw::GLintVec& first,
            const jag::draw::GLsizeiVec& count, const GLsizei primcount )
      : DrawCommand( MultiDrawArraysType, mode, 0, primcount ),
        MultiArrayBase( count ),
        _firstVec( first )
    {}
    MultiDrawArrays( const MultiDrawArrays& rhs )
      : DrawCommand( rhs ),
        MultiArrayBase( rhs ),
        _firstVec( rhs._firstVec )
    {}
    virtual ~MultiDrawArrays()
    {}

    virtual void execute( DrawInfo& drawInfo )
    {
        glMultiDrawArrays( _mode, &_firstVec[ 0 ], &_countVec[ 0 ], _primcount );
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        GLsizei localCounter( counter );
        for( GLsizei idx=0; idx < _primcount; ++idx )
        {
            if( localCounter >= _countVec[ idx ] )
                localCounter -= _countVec[ idx ];
            else
                return( _firstVec[ idx ] + localCounter );
        }
        // TBD Went off the end of the array. I wonder if this might seg fault...
        return( _firstVec[ _primcount - 1 ] + _countVec[ _primcount - 1 ] + 1 );
    }
    virtual unsigned int getNumIndices() const
    {
        unsigned int total( 0 );
        for( GLsizei idx=0; idx < _primcount; ++idx )
        {
            total += _countVec[ idx ];
        }
        return( total );
    }

    jag::draw::GLintVec& getFirst()
    {
        return( _firstVec );
    }
    const jag::draw::GLintVec& getFirst() const
    {
        return( _firstVec );
    }

protected:
    jag::draw::GLintVec _firstVec;
};

typedef jag::base::ptr< jag::draw::MultiDrawArrays >::shared_ptr MultiDrawArraysPtr;


/** \class DrawElements DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElements

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

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
            const unsigned char* ptr( ( const unsigned char* ) _elementBuffer->getBuffer()->getData() + (ptrdiff_t) _indices );
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

typedef jag::base::ptr< jag::draw::DrawElements >::shared_ptr DrawElementsPtr;


/** \class DrawElementsInstancedBaseInstance DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsInstancedBaseInstance() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElementsInstancedBaseInstance

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElementsInstancedBaseInstance( _mode\, _count\, _type\, _indices\, _primcount, _baseInstance );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
class DrawElementsInstancedBaseInstance : public DrawCommand,
            public DrawElementsBase, public BaseInstanceBase
{
public:
    DrawElementsInstancedBaseInstance( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* indices, GLsizei primcount, const GLuint baseInstance,
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
      : DrawCommand( DrawElementsInstancedBaseInstanceType, mode, count, primcount ),
        DrawElementsBase( type, indices, elementBuffer ),
        BaseInstanceBase( baseInstance )
    {}
    DrawElementsInstancedBaseInstance( const DrawElementsInstancedBaseInstance& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        BaseInstanceBase( rhs )
    {}
    virtual ~DrawElementsInstancedBaseInstance()
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
#ifdef GL_VERSION_4_2
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsInstancedBaseInstance( _mode, _count, _type, _indices, _primcount, _baseInstance );
#endif
    }

    virtual int getIndex( const unsigned int counter ) const
    {
        if( _elementBuffer != NULL )
        {
            const unsigned char* ptr( ( const unsigned char* ) _elementBuffer->getBuffer()->getData() + (ptrdiff_t)_indices );
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

typedef jag::base::ptr< jag::draw::DrawElementsInstancedBaseInstance >::shared_ptr DrawElementsInstancedBaseInstancePtr;


/** \class DrawElementsInstanced DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsInstanced() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElementsInstanced

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

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
            const unsigned char* ptr( ( const unsigned char* ) _elementBuffer->getBuffer()->getData() + (ptrdiff_t)_indices );
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

typedef jag::base::ptr< jag::draw::DrawElementsInstanced >::shared_ptr DrawElementsInstancedPtr;


/** \class MultiDrawElements DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glMultiDrawElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin MultiDrawElements

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glMultiDrawElements( _mode\, &_countVec[ 0 ]\, _type\, (const GLvoid**)( &_indicesVec[ 0 ] )\, _primcount );
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
    MultiDrawElements( GLenum mode, const jag::draw::GLsizeiVec& count, GLenum type,
            const jag::draw::GLvoidPtrVec& indices, GLsizei primcount,
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glMultiDrawElements( _mode, &_countVec[ 0 ], _type, (const GLvoid**)( &_indicesVec[ 0 ] ), _primcount );
    }
};

typedef jag::base::ptr< jag::draw::MultiDrawElements >::shared_ptr MultiDrawElementsPtr;


/** \class DrawRangeElements DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawRangeElements() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawRangeElements

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawRangeElements( _mode, _start, _end, _count, _type, _indices );
    }
};

typedef jag::base::ptr< jag::draw::DrawRangeElements >::shared_ptr DrawRangeElementsPtr;


/** \class DrawElementsBaseVertex DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElementsBaseVertex

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsBaseVertex( _mode, _count, _type, _indices, _baseVertex );
    }
};

typedef jag::base::ptr< jag::draw::DrawElementsBaseVertex >::shared_ptr DrawElementsBaseVertexPtr;


/** \class DrawRangeElementsBaseVertex DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawRangeElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawRangeElementsBaseVertex

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawRangeElementsBaseVertex( _mode, _start, _end, _count, _type, _indices, _baseVertex );
    }
};

typedef jag::base::ptr< jag::draw::DrawRangeElementsBaseVertex >::shared_ptr DrawRangeElementsBaseVertexPtr;


/** \class DrawElementsInstancedBaseVertex DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsInstancedBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElementsInstancedBaseVertex

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
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsInstancedBaseVertex( _mode, _count, _type, _indices, _primcount, _baseVertex );
    }
};

typedef jag::base::ptr< jag::draw::DrawElementsInstancedBaseVertex >::shared_ptr DrawElementsInstancedBaseVertexPtr;


/** \class DrawElementsInstancedBaseVertexBaseInstance DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsInstancedBaseVertexBaseInstance() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin DrawElementsInstancedBaseVertexBaseInstance

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glDrawElementsInstancedBaseVertexBaseInstance( _mode\, _count\, _type\, _indices\, _primcount\, _baseVertex, _baseInstance );
    \endcode }
\specDepend{ElementArrayBuffer}
\specUsageBase{DrawCommand}
\specViolations{None}
\specTableEnd
\specEnd

*/
class DrawElementsInstancedBaseVertexBaseInstance : public DrawCommand,
            public DrawElementsBase, public BaseVertexBase, public BaseInstanceBase
{
public:
    DrawElementsInstancedBaseVertexBaseInstance( GLenum mode, GLsizei count, GLenum type,
            const GLvoid* offset, GLsizei primcount, GLint baseVertex, GLuint baseInstance,
            const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr() )
      : DrawCommand( DrawElementsInstancedBaseVertexBaseInstanceType, mode, count, primcount ),
        DrawElementsBase( type, offset, elementBuffer ),
        BaseVertexBase( baseVertex ),
        BaseInstanceBase( baseInstance )
    {}
    DrawElementsInstancedBaseVertexBaseInstance( const DrawElementsInstancedBaseVertexBaseInstance& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        BaseVertexBase( rhs ),
        BaseInstanceBase( rhs )
    {}
    virtual ~DrawElementsInstancedBaseVertexBaseInstance()
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
#ifdef GL_VERSION_4_2
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glDrawElementsInstancedBaseVertexBaseInstance( _mode, _count, _type, _indices, _primcount, _baseVertex, _baseInstance );
#endif
    }
};

typedef jag::base::ptr< jag::draw::DrawElementsInstancedBaseVertexBaseInstance >::shared_ptr DrawElementsInstancedBaseVertexBaseInstancePtr;


/** \class DrawElementsIndirect DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glDrawElementsIndirect() command.
\details

\specBegin DrawElementsIndirect

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
                const jag::draw::BufferObjectPtr elementBuffer=jag::draw::BufferObjectPtr(),
                const jag::draw::BufferObjectPtr indirectBuffer=jag::draw::BufferObjectPtr() )
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

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

typedef jag::base::ptr< jag::draw::DrawElementsIndirect >::shared_ptr DrawElementsIndirectPtr;


/** \class MultiDrawElementsBaseVertex DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glMultiDrawElementsBaseVertex() command.
\details

All DrawElements commands must be used with GL_ELEMENT_ARRAY_BUFFER (OpenGL v4.0
core profile spec). If an instance of this object is created with \c indices == NULL,
it is the calling code's responsibility to ensure that the active VertexArrayObject
has a buffer object bound to GL_ELEMENT_ARRAY_BUFFER.

\specBegin MultiDrawElementsBaseVertex

\specTableBegin
\specThreadBase{DrawCommand}
\specGL{On each call to execute():
    \code
    // GL_ELEMENT_ARRAY_BUFFER bind (if necessary; see ElementArrayBuffer).
    glMultiDrawElementsBaseVertex( _mode\, &_countVec[ 0 ]\, _type\, (const GLvoid**)( &_indicesVec[ 0 ] )\,
            _primcount\, &_basevertices[ 0 ] );
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
    MultiDrawElementsBaseVertex( GLenum mode, const jag::draw::GLsizeiVec& count, 
            GLenum type, const jag::draw::GLvoidPtrVec& indices, GLsizei primcount,
            const jag::draw::GLintVec& basevertex )
      : DrawCommand( MultiDrawElementsBaseVertexType, mode, 0, primcount ),
        DrawElementsBase( type ),
        MultiArrayBase( count, indices ),
        _basevertices( basevertex )
    {}
    MultiDrawElementsBaseVertex( const MultiDrawElementsBaseVertex& rhs )
      : DrawCommand( rhs ),
        DrawElementsBase( rhs ),
        MultiArrayBase( rhs ),
        _basevertices( rhs._basevertices )
    {
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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID ) {}

    virtual void execute( DrawInfo& drawInfo )
    {
        if( _elementBuffer != NULL )
            _elementBuffer->execute( drawInfo );
        glMultiDrawElementsBaseVertex( _mode, &_countVec[ 0 ], _type, (const GLvoid**)( &_indicesVec[ 0 ] ),
            _primcount, &_basevertices[ 0 ] );
    }

protected:
    jag::draw::GLintVec _basevertices;
};

typedef jag::base::ptr< jag::draw::MultiDrawElementsBaseVertex >::shared_ptr MultiDrawElementsBaseVertexPtr;


/** \class PrimitiveRestart DrawCommand.h <jag/draw/DrawCommand.h>
\brief Retained storage for the glPrimitiveRestart() command.
\details
\gl{section 2.8.1}.

\specBegin PrimitiveRestart

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

typedef jag::base::ptr< PrimitiveRestart >::shared_ptr PrimitiveRestartPtr;


// namespace jag::draw::
}
}


// __JAG_DRAW_DRAW_COMMANDS_H__
#endif
