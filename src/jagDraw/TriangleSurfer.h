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

#ifndef __JAGDRAW_TRIANGLE_CONTAINER_H__
#define __JAGDRAW_TRIANGLE_CONTAINER_H__ 1


#include <jagBase/Buffer.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/VertexAttribContainer.h>
#include <jagDraw/DrawCommand.h>

#include <iterator>
#include <iostream>


namespace jagDraw {




/** \class TriangleSurfer TriangleSurfer.h <jagDraw/Triangle.h>
\brief TBD
\details efficiently surfs vertex buffers and draw primitives to access all triangles in a drawable */
template< typename T >
class TriangleSurfer
{
public:
    TriangleSurfer(const BufferObjectPtr bop, const VertexAttribPtr vap, const DrawCommandPtr dcp ) 
    : _buffer( bop->getBuffer() ),
        _vap( vap ),
        _dcp( dcp ),
        _step( _vap->getActualStride() ),
        _offset( _vap->getOffset() )
        
    {
        
    }
    TriangleSurfer() {}
    unsigned int getNumTriangles() {
        switch(_dcp->getMode())  {
        case GL_TRIANGLES:
            return( _dcp->getNumIndices()/3);
            break;
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLE_STRIP:
            return(  _dcp->getNumIndices()-2);

            break; 
            return 0;
        }
        return 0;
    }
    void getTriangle(int& i, T* &a, T* &b, T* &c) {
        switch(_dcp->getMode())  {
        case GL_TRIANGLES:
            a = (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( i*3 ) * _step ) );
            b = (T*)(( (unsigned char *)( _buffer->getData() ) + _offset + _dcp->getIndex( i*3+1 ) * _step ));
            c = (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( i*3 +2) * _step ) );
            return;
            break;
        case GL_TRIANGLE_FAN:
            a = (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( 0 ) * _step ) );
            b = (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( i+1 ) * _step ) );
            c = (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( i+2 ) * _step ) );
            return;
        case GL_TRIANGLE_STRIP:
            if(!(i%2)) {
                a = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i ) * _step ) );
                b = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i+1 ) * _step ) );
                c = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i+2 ) * _step ) );

                return;
            }
            else {
                a = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i ) * _step ) );
                c = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i+1 ) * _step ) );
                b = (T*)(
                    ( (unsigned char *)( _buffer->getData() ) + _offset
                    + _dcp->getIndex( i+2 ) * _step ) );
            }

            break; 
            return;
        }
        return;
    }
    protected:
    jagBase::BufferPtr _buffer;
    VertexAttribPtr _vap;
    DrawCommandPtr _dcp;
    
    size_t _step;
    size_t _offset;
};


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#endif
