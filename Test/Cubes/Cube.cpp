#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <Chaskii/Mem/Buffer.h>
#include <Chaskii/Draw/BufferObject.h>
#include <Chaskii/Draw/VertexAttribute.h>
#include <Chaskii/Draw/VertexIterator.h>
#include "Cube.h"

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

Cube::Cube()
{
    //asDrawArrays();
   //asMultiDrawArrays( iiMath::matrix4f() );
   // asDrawElements();
}

Cube::Cube( float x, float y, float z, bool t ):
    m_is_transp(t)
{
    //asDrawArrays();
   asMultiDrawArrays(x, y, z);
   // asDrawElements();
}

void Cube::asDrawArrays()
{
#if 0
    unsigned char e[] = {
        0, 1, 2,
        2, 1, 3,
        2, 3, 6,
        6, 3, 7,
        6, 7, 4,
        4, 7, 5,
        4, 5, 0,
        0, 5, 1,
        1, 5, 3,
        3, 5, 7,
        4, 0, 6,
        6, 0, 2
    }; 
    size_t len = sizeof(e)/sizeof(unsigned char);

    std::vector<float> vv;
    int ii[] = { 0, 1, 2, 2, 1, 3 }; 
    for( unsigned int i = 0; i < len; i++ )
    {
        int index = e[i] * 3;
        vv.push_back( v[index + 0] );
        vv.push_back( v[index + 1] );
        vv.push_back( v[index + 2] );

        int ti = ii[(i % 6)];
        vv.push_back( t[ti * 2 + 0] );
        vv.push_back( t[ti * 2 + 1] );

        int ni = (i/6) % 6;
        vv.push_back( n[ni * 3 + 0] );
        vv.push_back( n[ni * 3 + 1] );
        vv.push_back( n[ni * 3 + 2] );
    }

    iiMem::Buffer_ptr vbuffer(new iiMem::Buffer( sizeof(float) * vv.size(), &vv.front() ));
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::ArrayBuffer( vbuffer, iiDraw::BufferObject::StaticDraw ) ));

    GLsizei stride = sizeof(GLfloat[8]);
    GLsizei offset = 0;
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 0, 3, GL_FLOAT, GL_FALSE, stride, offset )) );

    offset += sizeof(GLfloat[3]);
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 1, 2, GL_FLOAT, GL_FALSE, stride, offset )) );

    offset += sizeof(GLfloat[2]);
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 2, 3, GL_FLOAT, GL_FALSE, stride, offset )) );

    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::DrawArrays( GL_TRIANGLES, 0, vv.size() )));
#endif
}

void Cube::asMultiDrawArrays( float x, float y, float z )
{
    unsigned char e[] = {
        0, 1, 2, 3,
        2, 3, 6, 7,
        6, 7, 4, 5,
        4, 5, 0, 1,
        1, 5, 3, 7,
        4, 0, 6, 2
    }; 
    size_t len = sizeof(e)/sizeof(unsigned char);

    m_bound.set( iiMath::vec3( x, y, z ), sqrt( 2.0 ));

    std::vector<float> vv;
    for( unsigned int i = 0; i < len; i++ )
    {
        int index = e[i] * 3;

        // Vertex
        vv.push_back( v[index + 0] + x );
        vv.push_back( v[index + 1] + y );
        vv.push_back( v[index + 2] + z );

        // Texcoord
        int ti = (i % 4);
        vv.push_back( t[ti * 2 + 0] );
        vv.push_back( t[ti * 2 + 1] );

        // Normal
        int ni = (i/4);
        vv.push_back( n[ni * 3 + 0] );
        vv.push_back( n[ni * 3 + 1] );
        vv.push_back( n[ni * 3 + 2] );

        // Color
        vv.push_back( 1.0 );
        vv.push_back( 1.0 );
        vv.push_back( 1.0 );
        if( m_is_transp )
            vv.push_back( 0.15 );
        else
            vv.push_back( 1.0 );

    }

    iiMem::Buffer_ptr vbuffer(new iiMem::Buffer( sizeof(float) * vv.size(), &vv.front() ));
    addAttribute( sortable_ptr<iiDraw::DrawableAttribute>( new iiDraw::ArrayBuffer( vbuffer, iiDraw::BufferObject::StaticDraw ) ));

    iiDraw::VertexAttributeList * valist = new iiDraw::VertexAttributeList;
    iiDraw::VertexIteratorList * vilist = new iiDraw::VertexIteratorList;

    GLsizei stride = sizeof(GLfloat[12]);
    GLsizei offset = 0;
    valist->push_back( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 0, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[3]);
    valist->push_back( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 1, 2, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[2]);
    valist->push_back ( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 2, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[3]);
    valist->push_back ( iiDraw::VertexAttribute_ptr( new iiDraw::VertexAttribute( 3, 4, GL_FLOAT, GL_FALSE, stride, offset ) ));

    addAttribute( sortable_ptr<iiDraw::DrawableAttribute>( valist ) );

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
    addAttribute( sortable_ptr<iiDraw::DrawableAttribute>( vilist ) );
}

void Cube::asDrawElements()
{
#if 0
    std::vector<float>vv;
    std::vector<unsigned int> e;
    const int d = 32;

    for( int i = 0; i < d; i++ )
    {
        float t = float(i)/(float(d)-1.0f);
        float y = t;
        for( int j = 0; j < d; j++ )
        {
            float s = float(j)/(float(d)-1.0f);
            float x = s; 
            float z = 0.0;

            vv.push_back( x );
            vv.push_back( y );
            vv.push_back( z );

            vv.push_back( s );
            vv.push_back( t );

            vv.push_back( 0.0 );
            vv.push_back( 0.0 );
            vv.push_back( 1.0 );

            if( i < (d-1) )
            {
                e.push_back( (i+1) * d + j );
                e.push_back( (i+0) * d + j );
            }
        }
    }

    iiMem::Buffer_ptr vbuffer(new iiMem::Buffer( sizeof(float) * vv.size(), &vv.front() ));
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::ArrayBuffer( vbuffer, iiDraw::BufferObject::StaticDraw )) ); 

    size_t offset = 0;
    size_t stride = sizeof(float[8]);
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 0, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[3]);
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 1, 2, GL_FLOAT, GL_FALSE, stride, offset ) ));

    offset += sizeof(float[2]);
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::VertexAttribute( 2, 3, GL_FLOAT, GL_FALSE, stride, offset ) ));

    iiMem::Buffer_ptr ebuffer( new iiMem::Buffer( sizeof(unsigned int) * e.size(), &e.front() ));
    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::ElementArrayBuffer( ebuffer, iiDraw::BufferObject::StaticDraw )) );

    iiDraw::GLindexList indexList;
    iiDraw::GLsizeiList countList;

    offset = 0;
    for( int i = 0; i < d-1; i++ )
    {
        countList.push_back( d * 2 );
        indexList.push_back( (const GLvoid *)offset );
        offset += sizeof(GLuint) * d * 2;
    }

    addAttribute( iiDraw::DrawableAttribute_ptr( new iiDraw::MultiDrawElements( GL_TRIANGLE_STRIP, countList, GL_UNSIGNED_INT, indexList, d - 1 )));

#endif
}

