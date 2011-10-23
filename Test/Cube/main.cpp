#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <Chaskii/Mem/Buffer.h>
#include <Chaskii/Draw/BufferObject.h>
#include <Chaskii/Draw/VertexAttribute.h>
#include <Chaskii/Draw/VertexIterator.h>
#include <Chaskii/Draw/Drawable.h>

static float v[] = {
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
};


static float n[] = {
    0.0,  0.0,  1.0,
    0.0,  1.0,  0.0,
    0.0,  0.0, -1.0,
    0.0, -1.0,  0.0,
    1.0,  0.0,  0.0,
   -1.0,  0.0,  0.0,
};

static float t[] = {
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    1.0, 1.0
};

unsigned char e[] = {
    0, 1, 2, 3,
    2, 3, 6, 7,
    6, 7, 4, 5,
    4, 5, 0, 1,
    1, 5, 3, 7,
    4, 0, 6, 2
}; 

int main( int argc, char **argv )
{
    size_t len = sizeof(e)/sizeof(unsigned char);

    std::vector<float> vv;
    for( unsigned int i = 0; i < len; i++ )
    {
        int index = e[i] * 3;

        vv.push_back( v[index + 0] );
        vv.push_back( v[index + 1] );
        vv.push_back( v[index + 2] );

        int ti = (i % 4);
        vv.push_back( t[ti * 2 + 0] );
        vv.push_back( t[ti * 2 + 1] );

        int ni = (i/4);
        vv.push_back( n[ni * 3 + 0] );
        vv.push_back( n[ni * 3 + 1] );
        vv.push_back( n[ni * 3 + 2] );
    }

    iiDraw::VertexAttributeList_ptr valist(new iiDraw::VertexAttributeList);
    iiDraw::VertexIteratorList_ptr vilist(new iiDraw::VertexIteratorList);

    iiDraw::Drawable drawable;

    iiMem::Buffer_ptr vbuffer(new iiMem::Buffer( sizeof(float) * vv.size(), &vv.front() ));
    drawable.addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::ArrayBuffer( vbuffer, iiDraw::BufferObject::StaticDraw ) ));

    GLsizei stride = sizeof(GLfloat[8]);
    GLsizei offset = 0;
    valist->push_back( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 0, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[3]);
    valist->push_back( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 1, 2, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[2]);
    valist->push_back ( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 2, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    std::vector<GLint> firstList;
    std::vector<GLsizei> countList;
    GLint first = 0;
    for( int i = 0; i < 6; i++ )
    {
        firstList.push_back( first );
        countList.push_back( 4 );
        first += 4;
    }
    vilist->push_back( iiDraw::VertexIterator_ptr( new iiDraw::MultiDrawArrays( GL_TRIANGLE_STRIP, firstList, countList, firstList.size() )));
    drawable.addAttribute( valist );
    drawable.addAttribute( vilist );
}


