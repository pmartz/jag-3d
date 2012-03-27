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
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>
#include <jagBase/Buffer.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class BufferObject BufferObject.h <jagDraw/BufferObject.h>
\brief A context-safe wrapper for OpenGL buffer objects.
\details \gl{section 2.9}.
*/
class JAGDRAW_EXPORT BufferObject : public VertexArrayCommand,
            public jagBase::LogBase
{
public:
    /**
    \glparam{target,table 2.8}
    */
    BufferObject( const GLenum target );
    /**
    \glparam{target,table 2.8}
    \param b BufferPtr
    \glparam{usage,section 2.9.2}
    */
    BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
    BufferObject( const BufferObject& rhs );

    virtual ~BufferObject();

    GLint getId( const unsigned int contextID );

    /** \brief Tell the BufferObject how many contexts to expect.
    \details Resizes the _ids variable */
    void setMaxContexts( const unsigned int numContexts );

    void setBuffer( jagBase::BufferPtr b );
    jagBase::BufferPtr getBuffer() { return( _buffer ); }
    size_t getBufferSize();

    GLenum getTarget() { return _target; }

    void setUsage( const GLenum usage );
    GLenum getUsage() { return( _usage ); }

    virtual void operator()( DrawInfo& drawInfo );

    // TBD need to get context ID, probably as a param?
    void subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid * );


    // TBD need to get context ID, probably as a param?
    GLbyte* map( const GLenum access );
    // TBD need to get context ID, probably as a param?
    void unmap();

protected:
    void internalInit( const unsigned int contextID );

    GLenum _target;
    GLenum _usage;
    jagBase::BufferPtr _buffer;

    PerContextGLuint _ids;
};

typedef jagBase::ptr< jagDraw::BufferObject >::shared_ptr BufferObjectPtr;
typedef std::vector< BufferObjectPtr > BufferObjectList;



/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
*/
class JAGDRAW_EXPORT DrawIndirectBuffer : public BufferObject
{
public:
    DrawIndirectBuffer():
        BufferObject( GL_DRAW_INDIRECT_BUFFER )
    {}
    DrawIndirectBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_DRAW_INDIRECT_BUFFER, b, usage )
    {}
};

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
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

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
*/
class JAGDRAW_EXPORT TransformFeedbackBuffer : public BufferObject
{
public:
    TransformFeedbackBuffer():
        BufferObject( GL_TRANSFORM_FEEDBACK_BUFFER )
    {}
    TransformFeedbackBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_TRANSFORM_FEEDBACK_BUFFER, b, usage )
    {}
};

/**
\details One class for each buffer bind target. See \gl{table 2.8}.
*/
class JAGDRAW_EXPORT UniformBuffer : public BufferObject
{
public:
    UniformBuffer():
        BufferObject( GL_UNIFORM_BUFFER )
    {}
    UniformBuffer( const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW ):
        BufferObject( GL_UNIFORM_BUFFER, b, usage )
    {}
};


// jagDraw
}


// __JAGDRAW_BUFFER_OBJECT_H__
#endif
