/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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
