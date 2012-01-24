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
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>
#include <jagBase/Buffer.h>
//#include <Chaskii/Draw/DrawableAttribute.h>

#include <vector>


namespace jagDraw {


/** \class BufferObject BufferObject.h <jagDraw/BufferObject.h>
\brief A context-safe wrapper for OpenGL buffer objects.
*/
class JAGDRAW_EXPORT BufferObject /* : public DrawableAttribute */
{
public:
    BufferObject( const GLenum target );
    BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage=GL_STATIC_DRAW );
    BufferObject( const BufferObject &rhs );

    virtual ~BufferObject();

    void setBuffer( jagBase::BufferPtr b );
    jagBase::BufferPtr getBuffer() { return( _buffer ); }
    size_t getBufferSize();

    GLenum getTarget() { return _target; }

    void setUsage( const GLenum usage );
    GLenum getUsage() { return( _usage ); }

    // TBD need to get context ID, probably as a param?
    virtual void bind();

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

typedef jagBase::ptr< jagDraw::BufferObject >::shared_ptr BufferObjectPtr;
typedef std::vector< BufferObjectPtr > BufferObjectList;


// jagDraw
}


// __JAGDRAW_BUFFER_OBJECT_H__
#endif
