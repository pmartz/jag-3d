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

#include "osg2jag.h"

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Array>
#include <osg/Vec3>

#include <jagDraw/Node.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/types.h>
#include <jagBase/LogMacros.h>
#include <gmtl/gmtl.h>

#include <sstream>


static const std::string loggerName( "jag.plugin.model.osg2jag" );

using namespace jagDraw;


Osg2Jag::Osg2Jag()
  : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
    _jagScene( jagSG::NodePtr( (jagSG::Node*)NULL ) ),
    _current( jagSG::NodePtr( (jagSG::Node*)NULL ) )
{
}
Osg2Jag::~Osg2Jag()
{
}

void Osg2Jag::apply( osg::Node& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Node& )" );

    if( !preTraverse( &osgNode ) )
        return;
    traverse( osgNode );
    postTraverse();
}
void Osg2Jag::apply( osg::MatrixTransform& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Transform& )" );

    if( !preTraverse( &osgNode, asGmtlMatrix( osgNode.getMatrix() ) ) )
        return;
    traverse( osgNode );
    postTraverse();
}
void Osg2Jag::apply( osg::Geode& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Geode& )" );

    if( !preTraverse( &osgNode ) )
        return;

    unsigned int idx;
    for( idx=0; idx<osgNode.getNumDrawables(); idx++ )
    {
        if( osgNode.getDrawable( idx )->asGeometry() != NULL )
            apply( osgNode.getDrawable( idx )->asGeometry() );
    }

    traverse( osgNode );
    postTraverse();
}

bool Osg2Jag::preTraverse( osg::Object* osgObject, const gmtl::Matrix44d& m )
{
    if( _jagScene == NULL )
    {
        _jagScene = jagSG::NodePtr( new jagSG::Node() );
        _current = _jagScene;
    }
    else
    {
        OSGObjectMap::iterator it( _objInstances.find( osgObject ) );
        if( it != _objInstances.end() )
        {
            JAG3D_TRACE_STATIC( loggerName, "\tShared instance." );
            _current->addChild( it->second );
            return( false );
        }
        else
        {
            jagSG::NodePtr np( new jagSG::Node() );
            _current->addChild( np );
            _current = np;
            _objInstances[ osgObject ] = np;
        }
    }
    _nodeStack.push_back( _current );

    if( !( osgObject->getName().empty() ) )
        _current->setUserDataName( osgObject->getName() );
    _current->setTransform( m );

    return( true );
}
void Osg2Jag::postTraverse()
{
    _nodeStack.pop_back();
    _current = ( _nodeStack.empty() ?
        jagSG::NodePtr( (jagSG::Node*)NULL ) : _nodeStack.back() );
}


void Osg2Jag::apply( osg::Geometry* geom )
{
    JAG3D_TRACE_STATIC( loggerName, "Geometry" );

    if( geom->getVertexArray() == NULL )
    {
        JAG3D_WARNING_STATIC( loggerName, "Geometry has no vertex array. Skipping." );
        return;
    }

    if( !preTraverse( geom ) )
        return;

    DrawablePtr draw( DrawablePtr( new Drawable ) );
    jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );

    jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );

    const unsigned int numVertices( geom->getVertexArray()->getNumElements() );
    {
        ArrayInfo info( asJagArray( geom->getVertexArray() ) );
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
            JAG3D_NOTICE_STATIC( loggerName, "Only BIND_PER_VERTEX is currently supported." );
        }

        ArrayInfo info( asJagArray( geom->getNormalArray() ) );
        jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
        vaop->addVertexArrayCommand( bop, jagDraw::VertexArrayObject::Normal );

        jagDraw::VertexAttribPtr attrib( new jagDraw::VertexAttrib(
            "normal", info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( attrib, jagDraw::VertexArrayObject::Normal );
    }



    for( unsigned int idx = 0; idx < geom->getNumTexCoordArrays(); ++idx )
    {
        if( geom->getTexCoordArray( idx ) != NULL )
        {
            ArrayInfo info( asJagArray( geom->getTexCoordArray( idx ) ) );
            jagDraw::BufferObjectPtr bop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
            vaop->addVertexArrayCommand( bop, jagDraw::VertexArrayObject::TexCoord );
            std::ostringstream ostr;
            ostr << "texcoord" << idx;
            std::cout << info._componentsPerElement << " texcoords" << std::endl;
            jagDraw::VertexAttribPtr attrib( new jagDraw::VertexAttrib(
                ostr.str(), info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
            vaop->addVertexArrayCommand( attrib, jagDraw::VertexArrayObject::TexCoord );
        }
    }

    commands->insert( vaop );

    unsigned int idx;
    for( idx=0; idx<geom->getNumPrimitiveSets(); idx++ )
    {
        const osg::PrimitiveSet* ps( geom->getPrimitiveSet( idx ) );
        switch( ps->getType() )
        {
        case osg::PrimitiveSet::DrawArraysPrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawArrays" );

            const osg::DrawArrays* da( static_cast< const osg::DrawArrays* >( ps ) );
            jagDraw::DrawArraysPtr drawcom( new jagDraw::DrawArrays(
                da->getMode(), da->getFirst(), da->getCount() ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawArrayLengthsPrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawArrayLengths" );

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
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUByte" );

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
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUShort" );

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
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUInt" );

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
            JAG3D_ERROR_STATIC( loggerName, "Osg2Jag::apply(Geometry&): Unsupported osg::PrimitiveSet::Type." );
            break;
        }
        }
    }

    _current->setCommandMap( commands );
    _current->addDrawable( draw );

    postTraverse();
}

jagSG::NodePtr Osg2Jag::getJagScene()
{
    return( _jagScene );
}


Osg2Jag::ArrayInfo Osg2Jag::asJagArray( const osg::Array* arrayIn )
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
            const osg::Vec3& v( (*array)[ idx ] );
            gmtl::Point3f& p( out[ idx ] );
            p[ 0 ] = v[ 0 ];
            p[ 1 ] = v[ 1 ];
            p[ 2 ] = v[ 2 ];
        }

        jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( gmtl::Point3f ), (void*)&( out[0] ) ) );
        info._buffer = bp;
        break;
    }
    case osg::Array::Vec2ArrayType:
    {
        const osg::Vec2Array* array( static_cast< const osg::Vec2Array* >( arrayIn ) );
        const unsigned int size( array->size() );

        info._type = GL_FLOAT;
        info._numElements = size;
        info._componentsPerElement = 2;

        jagBase::Point2fVec out;
        out.resize( size );
        unsigned int idx;
        for( idx=0; idx<size; idx++ )
        {
            const osg::Vec2& v( (*array)[ idx ] );
            gmtl::Point2f& p( out[ idx ] );
            p[ 0 ] = v[ 0 ];
            p[ 1 ] = v[ 1 ];
        }

        jagBase::BufferPtr bp( new jagBase::Buffer( size * sizeof( gmtl::Point2f ), (void*)&( out[0] ) ) );
        info._buffer = bp;
        break;
    }
    default:
    {
        JAG3D_ERROR_STATIC( loggerName, "Osg2Jag::asJagArray(): Unsupported osg::Array::Type." );
        break;
    }
    }

    std::ostringstream ostr;
    ostr << "Processed array of size " << info._numElements;
    JAG3D_INFO_STATIC( loggerName, std::string(ostr.str()) );

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

gmtl::Matrix44d Osg2Jag::asGmtlMatrix( const osg::Matrixd& m )
{
    gmtl::Matrix44d result;
    result.set( m.ptr() );
    return( result );
}
gmtl::Matrix44d Osg2Jag::asGmtlMatrix( const osg::Matrixf& m )
{
    osg::Matrixd md( m );
    return( asGmtlMatrix( md ) );
}
