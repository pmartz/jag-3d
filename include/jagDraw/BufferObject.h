/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_DRAW_BUFFER_OBJECT_DEF
#define CHASKII_DRAW_BUFFER_OBJECT_DEF 1

#include <vector>

#include <Chaskii/OpenGL.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Mem/Buffer.h>
//#include <Chaskii/Draw/DrawContext.h>
#include <Chaskii/Draw/DrawableAttribute.h>

namespace iiDraw {

class II_EXPORT BufferObject: public DrawableAttribute
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
        BufferObject( Target, iiMem::Buffer_ptr b, Usage usage );
        BufferObject( const BufferObject &b );

        virtual ~BufferObject();

        void setBuffer( iiMem::Buffer_ptr b );
        iiMem::Buffer_ptr getBuffer();
        size_t getBufferSize() { return m_buffer_size; }

        Target getTarget() { return m_target; }

        void setUsage( Usage usage );
        Usage getUsage();

        virtual void apply( DrawContext &);
        virtual void gfxInit( DrawContext & );

        void subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid * );


        GLbyte *map( Access access );
        void unmap();

    protected:
        Target  m_target;
        bool    m_gfxInited;
        Usage   m_usage;
        iiMem::Buffer_ptr m_buffer;
        GLenum  m_id;
        size_t  m_buffer_size;

        bool p_gfxInit();
};


class II_EXPORT ArrayBuffer: public BufferObject
{
    public:
        ArrayBuffer():
            BufferObject( BufferObject::ArrayBuffer )
        {}
        ArrayBuffer( iiMem::Buffer_ptr buffer, BufferObject::Usage usage ):
            BufferObject( BufferObject::ArrayBuffer, buffer, usage )
        {}
};

class II_EXPORT ElementArrayBuffer: public BufferObject
{
    public:
        ElementArrayBuffer():
            BufferObject( BufferObject::ElementArrayBuffer )
        {}
        ElementArrayBuffer( iiMem::Buffer_ptr buffer, BufferObject::Usage usage ):
            BufferObject( BufferObject::ElementArrayBuffer, buffer, usage )
        {}
};

class II_EXPORT PixelPackBuffer: public BufferObject
{
    public:
        PixelPackBuffer():
            BufferObject( BufferObject::PixelPackBuffer )
        {}
        PixelPackBuffer( iiMem::Buffer_ptr buffer, BufferObject::Usage usage ):
            BufferObject( BufferObject::PixelPackBuffer, buffer, usage )
        {}
};

class II_EXPORT PixelUnpackBuffer: public BufferObject
{
    public:
        PixelUnpackBuffer():
            BufferObject( BufferObject::PixelUnpackBuffer )
        {}
        PixelUnpackBuffer( iiMem::Buffer_ptr buffer, BufferObject::Usage usage ):
            BufferObject( BufferObject::PixelUnpackBuffer, buffer, usage )
        {}
};

typedef iiMem::ptr<BufferObject>::shared_ptr BufferObject_ptr;
typedef std::vector<BufferObject_ptr> BufferObjectList;

}


#endif
