/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

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



/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

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


/*@}*/


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#endif
