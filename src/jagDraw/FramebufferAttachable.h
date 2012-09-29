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

#ifndef __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#define __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__ 1


#include <jagDraw/Export.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \class FramebufferAttachable FramebufferAttachable.h <jagDraw/FramebufferAttachable.h>
\brief TBD
\details TBD
*/
class JAGDRAW_EXPORT FramebufferAttachable
{
public:
    typedef enum {
        TYPE_RENDERBUFFER,
        TYPE_TEXTURE
    } BufferType;

    FramebufferAttachable( BufferType bufferType )
    {}
    FramebufferAttachable( const FramebufferAttachable& rhs )
      : _bufferType( rhs._bufferType )
    {}
    virtual ~FramebufferAttachable()
    {}

    const BufferType getBufferType() const
    {
        return( _bufferType );
    }

protected:
    BufferType _bufferType;
};

typedef jagBase::ptr< jagDraw::FramebufferAttachable >::shared_ptr FramebufferAttachablePtr;
typedef std::vector< FramebufferAttachablePtr > FramebufferAttachableVec;


// jagDraw
}


// __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#endif
