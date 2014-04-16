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

#ifndef __JAG_DRAW_BUFFER_OBJECT_H__
#define __JAG_DRAW_BUFFER_OBJECT_H__ 1

#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/base/LogBase.h>
#include <jag/draw/VertexArrayCommand.h>
#include <jag/draw/ObjectID.h>
#include <jag/draw/PerContextData.h>
#include <jag/base/ptr.h>
#include <jag/base/Buffer.h>

#include <vector>


namespace jag {
namespace draw {


struct DrawInfo;


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

/** \class BufferObject BufferObject.h <jag/draw/BufferObject.h>
\brief A context-safe wrapper for OpenGL buffer objects.
\details \gl{section 2.9}.
*/
class JAGDRAW_EXPORT BufferObject : protected jag::base::LogBase,
            public ObjectID, public VertexArrayCommand
{
public:
    /**
    \glparam{target,table 2.8}
    \param target defaults to \c GL_NONE only when GL_VERSION_4_0 is not defined
    and DrawIndirectBuffer is instantiated.
    
    With this constructor, _usage defaults to GL_STATIC_DRAW. */
    BufferObject( const GLenum target=GL_NONE, const std::string& logName=std::string( "" ) );
    /**
    \glparam{target,table 2.8}
    \param b BufferPtr
    \glparam{usage,section 2.9.2}
    */
    BufferObject( const GLenum target, const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
    BufferObject( const BufferObject& rhs );

    virtual ~BufferObject();


    /** \brief Override from ObjectID.
    \details TBD */
    virtual void setMaxContexts( const unsigned int numContexts );

    virtual GLuint getID( const jag::draw::jagDrawContextID contextID );

    virtual bool isSameKind( const VertexArrayCommand& rhs ) const;

    void setBuffer( jag::base::BufferPtr b );
    const jag::base::BufferPtr& getBuffer() const;
    size_t getBufferSize();

    void setTarget( const GLenum target );
    GLenum getTarget() const;

    /** \brief Set the \c usage parameter for the \c glBufferData() call.
    \details Default value: GL_STATIC_DRAW. */
    void setUsage( const GLenum usage );
    GLenum getUsage() const;

    virtual void execute( DrawInfo& drawInfo );


    /** \brief Mark the entire buffer dirty for all contexts.
    \details During next BufferObject::execute(), send down
    entire buffer data with a glBufferData() call. */
    void markAllDirty( const bool dirty=true );

    /** \brief Mark a segment of the buffer as dirty.
    \details During next BufferObject::execute(), send down
    the range of buffer data with a glBufferSubData() call.
    \param offset Offset from buffer start in bytes. 
    \param size Number of bytes.
    
    Note: Currently, BufferObject stores only a single dirty
    range. Need support for a list of dirty ranges. */
    void setBufferRangeDirty( const size_t offset, const size_t size, const bool dirty=true );

    /** \brief Override from base class VertexArrayCommand.
    \details Return true if any part of the buffer object is dirty. */
    virtual bool isDirty( const unsigned int contextID );


    // TBD need to get context ID, probably as a param?
    void subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid* data );
    // TBD need to get context ID, probably as a param?
    GLbyte* map( const GLenum access );
    // TBD need to get context ID, probably as a param?
    void unmap();

protected:
    void internalInit( const unsigned int contextID );

    void sendDirtyBufferData( const unsigned int contextID );

    /** Default value: _target = GL_NONE */
    GLenum _target;
    /** Default value: _usage = GL_STATIC_DRAW */
    GLenum _usage;

    jag::base::BufferPtr _buffer;

    PerContextBool _dirty;
    size_t _dirtyOffset, _dirtySize;
};

typedef jag::base::ptr< jag::draw::BufferObject >::shared_ptr BufferObjectPtr;
typedef std::vector< BufferObjectPtr > BufferObjectVec;


class JAGDRAW_EXPORT IndexedBufferObject : public BufferObject
{
public:
    IndexedBufferObject( const GLenum target );
    IndexedBufferObject( const GLenum target, const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
    IndexedBufferObject( const BufferObject& rhs );

    IndexedBufferObject( const IndexedBufferObject& rhs );
    virtual ~IndexedBufferObject();

    virtual void execute( DrawInfo& drawInfo );

    void setIndex( GLuint index ) { _index = index; }
    GLuint getIndex() const { return( _index ); }

protected:
    GLuint _index;
};



/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT ArrayBuffer: public BufferObject
{
public:
    ArrayBuffer():
        BufferObject( GL_ARRAY_BUFFER )
    {}
    ArrayBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_ARRAY_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::ArrayBuffer >::shared_ptr ArrayBufferPtr;
typedef std::vector< ArrayBufferPtr > ArrayBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT CopyReadBuffer : public BufferObject
{
public:
    CopyReadBuffer():
        BufferObject( GL_COPY_READ_BUFFER )
    {}
    CopyReadBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_COPY_READ_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::CopyReadBuffer >::shared_ptr CopyReadBufferPtr;
typedef std::vector< CopyReadBufferPtr > CopyReadBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT CopyWriteBuffer : public BufferObject
{
public:
    CopyWriteBuffer():
        BufferObject( GL_COPY_WRITE_BUFFER )
    {}
    CopyWriteBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_COPY_WRITE_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::CopyWriteBuffer >::shared_ptr CopyWriteBufferPtr;
typedef std::vector< CopyWriteBufferPtr > CopyWriteBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT DrawIndirectBuffer : public BufferObject
{
public:
#ifdef GL_VERSION_4_0
    DrawIndirectBuffer() :
        BufferObject( GL_DRAW_INDIRECT_BUFFER )
    {}
    DrawIndirectBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_DRAW_INDIRECT_BUFFER, b, usage )
    {}
#endif
};

typedef jag::base::ptr< jag::draw::DrawIndirectBuffer >::shared_ptr DrawIndirectBufferPtr;
typedef std::vector< DrawIndirectBufferPtr > DrawIndirectBufferVec;

    
/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT ElementArrayBuffer: public BufferObject
{
public:
    ElementArrayBuffer():
        BufferObject( GL_ELEMENT_ARRAY_BUFFER )
    {}
    ElementArrayBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_ELEMENT_ARRAY_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::ElementArrayBuffer >::shared_ptr ElementArrayBufferPtr;
typedef std::vector< ElementArrayBufferPtr > ElementArrayBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT PixelPackBuffer: public BufferObject
{
public:
    PixelPackBuffer():
        BufferObject( GL_PIXEL_PACK_BUFFER )
    {}
    PixelPackBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_PIXEL_PACK_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::PixelPackBuffer >::shared_ptr PixelPackBufferPtr;
typedef std::vector< PixelPackBufferPtr > PixelPackBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT PixelUnpackBuffer: public BufferObject
{
public:
    PixelUnpackBuffer():
        BufferObject( GL_PIXEL_UNPACK_BUFFER )
    {}
    PixelUnpackBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_PIXEL_UNPACK_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::PixelUnpackBuffer >::shared_ptr PixelUnpackBufferPtr;
typedef std::vector< PixelUnpackBufferPtr > PixelUnpackBufferVec;
    
/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT TextureBuffer : public BufferObject
{
public:
    TextureBuffer():
        BufferObject( GL_TEXTURE_BUFFER )
    {}
    TextureBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_TEXTURE_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::TextureBuffer >::shared_ptr TextureBufferPtr;
typedef std::vector< TextureBufferPtr > TextureBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT TransformFeedbackBuffer : public IndexedBufferObject
{
public:
    TransformFeedbackBuffer():
        IndexedBufferObject( GL_TRANSFORM_FEEDBACK_BUFFER )
    {}
    TransformFeedbackBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        IndexedBufferObject( GL_TRANSFORM_FEEDBACK_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::TransformFeedbackBuffer >::shared_ptr TransformFeedbackBufferPtr;
typedef std::vector< TransformFeedbackBufferPtr > TransformFeedbackBufferVec;


/**
\details One class for each buffer bind target. \gl{table 2.8}.
*/
class JAGDRAW_EXPORT UniformBuffer : public IndexedBufferObject
{
public:
    UniformBuffer():
        IndexedBufferObject( GL_UNIFORM_BUFFER )
    {}
    UniformBuffer( const jag::base::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        IndexedBufferObject( GL_UNIFORM_BUFFER, b, usage )
    {}
};

typedef jag::base::ptr< jag::draw::UniformBuffer >::shared_ptr UniformBufferPtr;
typedef std::vector< UniformBufferPtr > UniformBufferVec;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_BUFFER_OBJECT_H__
#endif
