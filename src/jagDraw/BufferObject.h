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

#ifndef __JAGDRAW_BUFFER_OBJECT_H__
#define __JAGDRAW_BUFFER_OBJECT_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogBase.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>
#include <jagBase/Buffer.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

/** \class BufferObject BufferObject.h <jagDraw/BufferObject.h>
\brief A context-safe wrapper for OpenGL buffer objects.
\details \gl{section 2.9}.
*/
class JAGDRAW_EXPORT BufferObject : protected jagBase::LogBase,
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
    BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
    BufferObject( const BufferObject& rhs );

    virtual ~BufferObject();


    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    virtual bool isSameKind( const VertexArrayCommand& rhs ) const;

    void setBuffer( jagBase::BufferPtr b );
    const jagBase::BufferPtr& getBuffer() const;
    size_t getBufferSize();

    void setTarget( const GLenum target );
    GLenum getTarget() const;

    /** \brief Set the \c usage parameter for the \c glBufferData() call.
    \details Default value: GL_STATIC_DRAW. */
    void setUsage( const GLenum usage );
    GLenum getUsage() const;

    virtual void execute( DrawInfo& drawInfo );


    /** \brief Mark the entire buffer as dirty.
    \details During next BufferObject::execute(), send down
    entire buffer data with a glBufferData() call. */
    void setBufferDirty( const bool dirty=true );

    /** \brief Mark a segment of the buffer as dirty.
    \details During next BufferObject::execute(), send down
    the range of buffer data with a glBufferSubData() call.
    \param offset Offset from buffer start in bytes. 
    \param size Number of bytes.
    
    Note: Currently, BufferObject stores only a single dirty
    range. Need support for a list of dirty ranges. */
    void setBufferRangeDirty( const size_t offset, const size_t size, const bool dirty=true );


    // TBD need to get context ID, probably as a param?
    void subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid* data );
    // TBD need to get context ID, probably as a param?
    GLbyte* map( const GLenum access );
    // TBD need to get context ID, probably as a param?
    void unmap();

protected:
    void internalInit( const unsigned int contextID );

    void sendDirtyBufferData();

    /** Default value: _target = GL_NONE */
    GLenum _target;
    /** Default value: _usage = GL_STATIC_DRAW */
    GLenum _usage;

    jagBase::BufferPtr _buffer;

    bool _dirty;
    size_t _dirtyOffset, _dirtySize;
};

typedef jagBase::ptr< jagDraw::BufferObject >::shared_ptr BufferObjectPtr;
typedef std::vector< BufferObjectPtr > BufferObjectVec;


class JAGDRAW_EXPORT IndexedBufferObject : public BufferObject
{
public:
    IndexedBufferObject( const GLenum target );
    IndexedBufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
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
    ArrayBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_ARRAY_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::ArrayBuffer >::shared_ptr ArrayBufferPtr;
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
    CopyReadBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_COPY_READ_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::CopyReadBuffer >::shared_ptr CopyReadBufferPtr;
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
    CopyWriteBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_COPY_WRITE_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::CopyWriteBuffer >::shared_ptr CopyWriteBufferPtr;
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
    DrawIndirectBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_DRAW_INDIRECT_BUFFER, b, usage )
    {}
#endif
};

typedef jagBase::ptr< jagDraw::DrawIndirectBuffer >::shared_ptr DrawIndirectBufferPtr;
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
    ElementArrayBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_ELEMENT_ARRAY_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::ElementArrayBuffer >::shared_ptr ElementArrayBufferPtr;
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
    PixelPackBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_PIXEL_PACK_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::PixelPackBuffer >::shared_ptr PixelPackBufferPtr;
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
    PixelUnpackBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_PIXEL_UNPACK_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::PixelUnpackBuffer >::shared_ptr PixelUnpackBufferPtr;
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
    TextureBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_TEXTURE_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::TextureBuffer >::shared_ptr TextureBufferPtr;
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
    TransformFeedbackBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        IndexedBufferObject( GL_TRANSFORM_FEEDBACK_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::TransformFeedbackBuffer >::shared_ptr TransformFeedbackBufferPtr;
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
    UniformBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        IndexedBufferObject( GL_UNIFORM_BUFFER, b, usage )
    {}
};

typedef jagBase::ptr< jagDraw::UniformBuffer >::shared_ptr UniformBufferPtr;
typedef std::vector< UniformBufferPtr > UniformBufferVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_BUFFER_OBJECT_H__
#endif
