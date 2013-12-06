/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
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

#include <jagDraw/PlatformOpenGL.h>
#include "osg2jag.h"

#include <osg/Geode>
#include <osg/Geometry>

#include <jagDraw/DrawNode.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/types.h>
#include <jagBase/LogMacros.h>
#include <gmtl/gmtl.h>

#include <sstream>


Osg2Jag::Osg2Jag()
  : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN )
{
}
Osg2Jag::~Osg2Jag()
{
}

void Osg2Jag::apply( osg::Node& node )
{
    JAG3D_TRACE_STATIC( "jag.demo.jagload", "Node" );
    traverse( node );
}
void Osg2Jag::apply( osg::Geode& node )
{
    JAG3D_TRACE_STATIC( "jag.demo.jagload", "Geode" );

    unsigned int idx;
    for( idx=0; idx<node.getNumDrawables(); idx++ )
    {
        if( node.getDrawable( idx )->asGeometry() != NULL )
            apply( node.getDrawable( idx )->asGeometry() );
    }

    traverse( node );
}

void Osg2Jag::apply( osg::Geometry* geom )
{
    JAG3D_TRACE_STATIC( "jag.demo.jagload", "Geometry" );

    if( geom->getVertexArray() == NULL )
    {
        JAG3D_WARNING_STATIC( "jag.demo.jagload", "Geometry has no vertex array. Skipping." );
        return;
    }

    jagDraw::DrawablePtr draw( jagDraw::DrawablePtr( new jagDraw::Drawable ) );
    jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );

    jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );

    const unsigned int numVertices( geom->getVertexArray()->getNumElements() );
    {
        osg::Matrix m = osg::computeLocalToWorld( getNodePath() );
        ArrayInfo info( asJagArray( geom->getVertexArray(), m ) );
        jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
        vaop->addVertexArrayCommand( bop, jagDraw::VertexArrayObject::Vertex );

        jagDraw::VertexAttribPtr attrib( new jagDraw::VertexAttrib(
            "vertex", info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( attrib, jagDraw::VertexArrayObject::Vertex );
    }
    if( ( geom->getNormalArray() != NULL ) &&
        ( geom->getNormalBinding() != osg::Geometry::BIND_OFF ) )
    {
        if( geom->getNormalBinding() != osg::Geometry::BIND_PER_VERTEX )
        {
            JAG3D_NOTICE_STATIC( "jag.demo.jagload", "Only BIND_PER_VERTEX is currently supported." );
        }

        osg::Matrix m = osg::computeLocalToWorld( getNodePath() );
        m.setTrans(0.,0.,0.);
        ArrayInfo info( asJagArray( geom->getNormalArray(), m ) );
        jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
        vaop->addVertexArrayCommand( bop, jagDraw::VertexArrayObject::Normal );

        jagDraw::VertexAttribPtr attrib( new jagDraw::VertexAttrib(
            "normal", info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( attrib, jagDraw::VertexArrayObject::Normal );
    }
    // TBD tex coords

    commands->insert( vaop );

    unsigned int idx;
    for( idx=0; idx<geom->getNumPrimitiveSets(); idx++ )
    {
        const osg::PrimitiveSet* ps( geom->getPrimitiveSet( idx ) );
        switch( ps->getType() )
        {
        case osg::PrimitiveSet::DrawArraysPrimitiveType:
        {
            JAG3D_TRACE_STATIC( "jag.demo.jagload", "DrawArrays" );

            const osg::DrawArrays* da( static_cast< const osg::DrawArrays* >( ps ) );
            jagDraw::DrawArraysPtr drawcom( new jagDraw::DrawArrays(
                da->getMode(), da->getFirst(), da->getCount() ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawArrayLengthsPrimitiveType:
        {
            JAG3D_TRACE_STATIC( "jag.demo.jagload", "DrawArrayLengths" );

            const osg::DrawArrayLengths* dal( static_cast< const osg::DrawArrayLengths* >( ps ) );
            const unsigned int size( dal->size() );

            jagDraw::GLintVec first( size );
            jagDraw::GLsizeiVec count( size );
            GLint* fp( &first[ 0 ] );
            GLsizei* cp( &count[ 0 ] );

            unsigned int idx;
            for( idx=0; idx<size; idx++ )
            {
                if( idx==0 )
                    fp[ idx ] = dal->getFirst();
                else
                    fp[ idx ] = fp[ idx-1 ] + (int)( cp[ idx-1 ] );
                cp[ idx ] = (*dal)[ idx ];
            }

            jagDraw::MultiDrawArraysPtr drawcom( new jagDraw::MultiDrawArrays(
                dal->getMode(), first, count, size ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUBytePrimitiveType:
        {
            JAG3D_TRACE_STATIC( "jag.demo.jagload", "DrawElementsUByte" );

            const osg::DrawElementsUByte* deub( static_cast< const osg::DrawElementsUByte* >( ps ) );
            ArrayInfo info( asJagArray( deub ) );
            jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jagDraw::DrawElementsPtr drawcom( new jagDraw::DrawElements(
                deub->getMode(), info._numElements, GL_UNSIGNED_BYTE, NULL, bop ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUShortPrimitiveType:
        {
            JAG3D_TRACE_STATIC( "jag.demo.jagload", "DrawElementsUShort" );

            const osg::DrawElementsUShort* deus( static_cast< const osg::DrawElementsUShort* >( ps ) );
            ArrayInfo info( asJagArray( deus ) );
            jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jagDraw::DrawElementsPtr drawcom( new jagDraw::DrawElements(
                deus->getMode(), info._numElements, GL_UNSIGNED_SHORT, NULL, bop ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUIntPrimitiveType:
        {
            JAG3D_TRACE_STATIC( "jag.demo.jagload", "DrawElementsUInt" );

            const osg::DrawElementsUInt* deui( static_cast< const osg::DrawElementsUInt* >( ps ) );
            ArrayInfo info( asJagArray( deui ) );
            jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jagDraw::DrawElementsPtr drawcom( new jagDraw::DrawElements(
                deui->getMode(), info._numElements, GL_UNSIGNED_INT, NULL, bop ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        default:
        {
            JAG3D_ERROR_STATIC( "jag.demo.jagload", "Osg2Jag::apply(Geometry&): Unsupported osg::PrimitiveSet::Type." );
            break;
        }
        }
    }

    jagDraw::DrawNodePtr drawNode( new jagDraw::DrawNode( commands ) );
    drawNode->addDrawable( draw );
    _jagDrawNodes.push_back( drawNode );
}

jagDraw::DrawNodeContainer& Osg2Jag::getDrawNodeContainer()
{
    return( _jagDrawNodes );
}


Osg2Jag::ArrayInfo Osg2Jag::asJagArray( const osg::Array* arrayIn, const osg::Matrix& m=osg::Matrix::identity() )
{
    ArrayInfo info;

    switch( arrayIn->getType() )
    {
    case osg::Array::Vec3ArrayType:
    {
        const osg::Vec3Array* array( static_cast< const osg::Vec3Array* >( arrayIn ) );
        const unsigned int size( array->size() );

        info._type = GL_FLOAT;
        info._numElements = size;
        info._componentsPerElement = 3;

        jagBase::Point3fVec out;
        out.resize( size );
        unsigned int idx;
        for( idx=0; idx<size; idx++ )
        {
            const osg::Vec3 v( (*array)[ idx ] * m );
            gmtl::Point3f& p( out[ idx ] );
            p[ 0 ] = v[ 0 ];
            p[ 1 ] = v[ 1 ];
            p[ 2 ] = v[ 2 ];
        }

        jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( gmtl::Point3f ), (void*)&( out[0] ) ) );
        info._buffer = bp;
        break;
    }
    default:
    {
        JAG3D_ERROR_STATIC( "jag.demo.jagload", "Osg2Jag::asJagArray(): Unsupported osg::Array::Type." );
        break;
    }
    }

    std::ostringstream ostr;
    ostr << "Processed array of size " << info._numElements;
    JAG3D_INFO_STATIC( "jag.demo.jagload", std::string(ostr.str()) );

    return( info );
}

Osg2Jag::ArrayInfo Osg2Jag::asJagArray( const osg::VectorGLubyte* arrayIn )
{
    const unsigned int size( arrayIn->size() );

    ArrayInfo info;
    info._type = GL_UNSIGNED_BYTE;
    info._numElements = size;
    info._componentsPerElement = 1;

    jagDraw::GLubyteVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( GLubyte ), (void*)&out[0] ) );
    info._buffer = bp;
    return( info );
}

Osg2Jag::ArrayInfo Osg2Jag::asJagArray( const osg::VectorGLushort* arrayIn )
{
    const unsigned int size( arrayIn->size() );

    ArrayInfo info;
    info._type = GL_UNSIGNED_SHORT;
    info._numElements = size;
    info._componentsPerElement = 1;

    jagDraw::GLushortVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( GLushort ), (void*)&out[0] ) );
    info._buffer = bp;
    return( info );
}

Osg2Jag::ArrayInfo Osg2Jag::asJagArray( const osg::VectorGLuint* arrayIn )
{
    const unsigned int size( arrayIn->size() );

    ArrayInfo info;
    info._type = GL_UNSIGNED_INT;
    info._numElements = size;
    info._componentsPerElement = 1;

    jagDraw::GLuintVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( GLuint ), (void*)&out[0] ) );
    info._buffer = bp;
    return( info );
}
