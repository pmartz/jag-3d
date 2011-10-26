// Copyright

#ifndef __JAG3D_DRAW_BUFFER_OBJECT_H__
#define __JAG3D_DRAW_BUFFER_OBJECT_H__ 1

#include <vector>

#include <jagDraw/Export.h>
#include <jagBase/PlatformOpenGL.h>
#include <jagBase/ptr.h>
#include <jagBase/Buffer.h>
//#include <Chaskii/Draw/DrawableAttribute.h>


namespace jagDraw {


/** \class BufferObject BufferObject.h <jagDraw/BufferObject.h>
\brief A context-safe wrapper for OpenGL buffer objects.
*/
class JAGDRAW_EXPORT BufferObject /* : public DrawableAttribute */
{
public:
    enum Target {
        ArrayBuffer         = GL_ARRAY_BUFFER,
        ElementArrayBuffer  = GL_ELEMENT_ARRAY_BUFFER,
        PixelPackBuffer     = GL_PIXEL_PACK_BUFFER,
        PixelUnpackBuffer   = GL_PIXEL_UNPACK_BUFFER
    };
    enum Usage {
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY
    };

    enum Access {
        ReadOnly = GL_READ_ONLY,
        WriteOnly = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };

    BufferObject( Target );
    BufferObject( Target, jagBase::BufferPtr b, Usage usage );
    BufferObject( const BufferObject &b );

    virtual ~BufferObject();

    void setBuffer( jagBase::BufferPtr b );
    jagBase::BufferPtr getBuffer();
    size_t getBufferSize() { return _bufferSize; }

    Target getTarget() { return _target; }

    void setUsage( Usage usage );
    Usage getUsage();

    virtual void apply();
    virtual void gfxInit();

    void subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid * );


    GLbyte *map( Access access );
    void unmap();

protected:
    bool p_gfxInit();
    bool _gfxInited;

    Target _target;
    Usage _usage;
    jagBase::BufferPtr _buffer;

    GLenum _id;
    size_t _bufferSize;
};


class JAGDRAW_EXPORT ArrayBuffer: public BufferObject
{
public:
    ArrayBuffer():
        BufferObject( BufferObject::ArrayBuffer )
    {}
    ArrayBuffer( jagBase::BufferPtr buffer, BufferObject::Usage usage ):
        BufferObject( BufferObject::ArrayBuffer, buffer, usage )
    {}
};

class JAGDRAW_EXPORT ElementArrayBuffer: public BufferObject
{
public:
    ElementArrayBuffer():
        BufferObject( BufferObject::ElementArrayBuffer )
    {}
    ElementArrayBuffer( jagBase::BufferPtr buffer, BufferObject::Usage usage ):
        BufferObject( BufferObject::ElementArrayBuffer, buffer, usage )
    {}
};

class JAGDRAW_EXPORT PixelPackBuffer: public BufferObject
{
public:
    PixelPackBuffer():
        BufferObject( BufferObject::PixelPackBuffer )
    {}
    PixelPackBuffer( jagBase::BufferPtr buffer, BufferObject::Usage usage ):
        BufferObject( BufferObject::PixelPackBuffer, buffer, usage )
    {}
};

class JAGDRAW_EXPORT PixelUnpackBuffer: public BufferObject
{
public:
    PixelUnpackBuffer():
        BufferObject( BufferObject::PixelUnpackBuffer )
    {}
    PixelUnpackBuffer( jagBase::BufferPtr buffer, BufferObject::Usage usage ):
        BufferObject( BufferObject::PixelUnpackBuffer, buffer, usage )
    {}
};

typedef jagBase::ptr< jagDraw::BufferObject >::shared_ptr BufferObjectPtr;
typedef std::vector< BufferObjectPtr > BufferObjectList;


// jagDraw
}


// __JAG3D_DRAW_BUFFER_OBJECT_H__
#endif
