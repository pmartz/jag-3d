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

#include <jag/draw/PlatformOpenGL.h>
#include "osg2jag.h"

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Array>
#include <osg/Vec3>
#include <osg/StateSet>
#include <osg/Material>

#include <jag/disk/Options.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/CommandMap.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/UniformBlock.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/draw/VertexArrayObject.h>
#include <jag/draw/types.h>
#include <jag/base/LogMacros.h>
#include <gmtl/gmtl.h>

#include <sstream>
#include <osg/io_utils>

static const std::string loggerName( "jag.plugin.model.osg2jag" );

using namespace jag::draw;


#define VEC4_TO_GMTL_PT4F(_v) \
    gmtl::Point4f( (float)(_v[0]), (float)(_v[1]), (float)(_v[2]), (float)(_v[3]) )


Osg2Jag::Osg2Jag( const jag::disk::Options* options )
  : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
    _jagScene( jagSG::NodePtr( (jagSG::Node*)NULL ) ),
    _current( jagSG::NodePtr( (jagSG::Node*)NULL ) ),
    _vertexAttribName( "vertex" ),
    _normalAttribName( "normal" ),
    _texCoordAttribName( "texcoord" )
{
    if( options != NULL )
    {
        try {
            if( options->hasOption( "vertexAttribName" ) )
                _vertexAttribName = boost::any_cast< std::string >( options->getOption( "vertexAttribName" ) );
            if( options->hasOption( "normalAttribName" ) )
                _normalAttribName = boost::any_cast< std::string >( options->getOption( "normalAttribName" ) );
            if( options->hasOption( "texCoordAttribName" ) )
                _texCoordAttribName = boost::any_cast< std::string >( options->getOption( "texCoordAttribName" ) );
        }
        catch( boost::bad_any_cast bac )
        {
            JAG3D_ERROR_STATIC( loggerName, "Unparsable jagDiskLLOptions: " + std::string( bac.what() ) );
        }
    }
}
Osg2Jag::~Osg2Jag()
{
}

void Osg2Jag::apply( osg::Node& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Node& )" );

    if( !preTraverse( &osgNode, osgNode.getStateSet() ) )
        return;
    traverse( osgNode );
    postTraverse();
}
void Osg2Jag::apply( osg::MatrixTransform& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Transform& )" );

    if( !preTraverse( &osgNode, osgNode.getStateSet(), asGmtlMatrix( osgNode.getMatrix() ) ) )
        return;
    traverse( osgNode );
    postTraverse();
}
void Osg2Jag::apply( osg::Geode& osgNode )
{
    JAG3D_TRACE_STATIC( loggerName, "apply( Geode& )" );

    if( !preTraverse( &osgNode, osgNode.getStateSet() ) )
        return;

    // storeInParent indicates that apply(Geometry) should not
    // create a new jagSG::Node, but rather store its jag::draw::CommandMap
    // and jag::draw::Drawable in the jagSG::Node corresponding to this
    // osg::Geode.
    bool storeInParent(
        ( osgNode.getNumDrawables() == 1 ) &&
        ( osgNode.getDrawable( 0 )->getNumParents() == 1 ) &&
        ( osgNode.getStateSet() == NULL )
        );
    unsigned int idx;
    for( idx=0; idx<osgNode.getNumDrawables(); idx++ )
    {
        if( osgNode.getDrawable( idx )->asGeometry() != NULL )
            apply( osgNode.getDrawable( idx )->asGeometry(), storeInParent );
    }

    traverse( osgNode );
    postTraverse();
}

bool Osg2Jag::preTraverse( osg::Object* osgObject, osg::StateSet* stateSet, const gmtl::Matrix44d& m )
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

    apply( stateSet );

    return( true );
}
void Osg2Jag::postTraverse()
{
    _nodeStack.pop_back();
    _current = ( _nodeStack.empty() ?
        jagSG::NodePtr( (jagSG::Node*)NULL ) : _nodeStack.back() );
}


void Osg2Jag::apply( osg::Geometry* geom, const bool storeInParent )
{
    JAG3D_TRACE_STATIC( loggerName, "Geometry" );

    if( geom->getVertexArray() == NULL )
    {
        JAG3D_WARNING_STATIC( loggerName, "Geometry has no vertex array. Skipping." );
        return;
    }

    if( !storeInParent )
    {
        if( !preTraverse( geom, geom->getStateSet() ) )
            return;
    }
    else
        apply( geom->getStateSet() );

    DrawablePtr draw( DrawablePtr( new Drawable ) );
    jag::draw::CommandMapPtr& commands( _current->getOrCreateCommandMap() );

    jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );

    const unsigned int numVertices( geom->getVertexArray()->getNumElements() );
    {
        ArrayInfo info( asJagArray( geom->getVertexArray() ) );
        jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
        vaop->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Vertex );

        jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
            _vertexAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Vertex );
    }
    if( ( geom->getNormalArray() != NULL ) &&
        ( geom->getNormalBinding() != osg::Geometry::BIND_OFF ) )
    {
        if( geom->getNormalBinding() != osg::Geometry::BIND_PER_VERTEX )
        {
            JAG3D_NOTICE_STATIC( loggerName, "Only BIND_PER_VERTEX is currently supported." );
        }

        ArrayInfo info( asJagArray( geom->getNormalArray() ) );
        jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
        vaop->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Normal );

        jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
            _normalAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Normal );
    }



    for( unsigned int idx = 0; idx < geom->getNumTexCoordArrays(); ++idx )
    {
        if( geom->getTexCoordArray( idx ) != NULL )
        {
            ArrayInfo info( asJagArray( geom->getTexCoordArray( idx ) ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
            vaop->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::TexCoord );
            std::ostringstream ostr;
            ostr << _texCoordAttribName << idx;
            jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
                ostr.str(), info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
            vaop->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::TexCoord );
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
            jag::draw::DrawArraysPtr drawcom( new jag::draw::DrawArrays(
                da->getMode(), da->getFirst(), da->getCount() ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawArrayLengthsPrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawArrayLengths" );

            const osg::DrawArrayLengths* dal( static_cast< const osg::DrawArrayLengths* >( ps ) );
            const unsigned int size( dal->size() );

            jag::draw::GLintVec first( size );
            jag::draw::GLsizeiVec count( size );
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

            jag::draw::MultiDrawArraysPtr drawcom( new jag::draw::MultiDrawArrays(
                dal->getMode(), first, count, size ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUBytePrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUByte" );

            const osg::DrawElementsUByte* deub( static_cast< const osg::DrawElementsUByte* >( ps ) );
            ArrayInfo info( asJagArray( deub ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jag::draw::DrawElementsPtr drawcom( new jag::draw::DrawElements(
                deub->getMode(), info._numElements, GL_UNSIGNED_BYTE, NULL, bop ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUShortPrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUShort" );

            const osg::DrawElementsUShort* deus( static_cast< const osg::DrawElementsUShort* >( ps ) );
            ArrayInfo info( asJagArray( deus ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jag::draw::DrawElementsPtr drawcom( new jag::draw::DrawElements(
                deus->getMode(), info._numElements, GL_UNSIGNED_SHORT, NULL, bop ) );
            draw->addDrawCommand( drawcom );
            break;
        }
        case osg::PrimitiveSet::DrawElementsUIntPrimitiveType:
        {
            JAG3D_TRACE_STATIC( loggerName, "DrawElementsUInt" );

            const osg::DrawElementsUInt* deui( static_cast< const osg::DrawElementsUInt* >( ps ) );
            ArrayInfo info( asJagArray( deui ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jag::draw::DrawElementsPtr drawcom( new jag::draw::DrawElements(
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

    _current->addDrawable( draw );

    if( !storeInParent )
        postTraverse();
}


jag::draw::UniformBlockPtr Osg2Jag::findMaterial( osg::Material* m )
{
    // Find by address
    OSGMaterialMap::iterator it( _matInstances.find( m ) );
    if( it != _matInstances.end() )
    {
        return( it->second );
    }

    // Some models do not share materials, but we can work around
    // that by looking up materials by their osg::Object name.
    BOOST_FOREACH( OSGMaterialMap::value_type dataPair, _matInstances )
    {
        if( dataPair.first->getName() == m->getName() )
        {
            return( dataPair.second );
        }
    }

    return( jag::draw::UniformBlockPtr( (jag::draw::UniformBlock*)NULL ) );
}
void Osg2Jag::addMaterial( osg::Material* m, jag::draw::UniformBlockPtr ub )
{
    _matInstances[ m ] = ub;
}

void Osg2Jag::apply( osg::StateSet* stateSet )
{
    if( stateSet == NULL )
        return;

    OSGStateSetMap::iterator it( _ssInstances.find( stateSet ) );
    if( it != _ssInstances.end() )
    {
        if( !( it->second->empty() ) )
        {
            // There are problems sharing command maps.
            //_current->setCommandMap( it->second );
            _current->setCommandMap( jag::draw::CommandMapPtr(
                new jag::draw::CommandMap( *(it->second) ) ) );
        }
        return;
    }

    jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new jag::draw::CommandMap ) );
    _ssInstances[ stateSet ] = commands;


    jag::draw::UniformBlockSetPtr ubsp( jag::draw::UniformBlockSetPtr(
        new jag::draw::UniformBlockSet() ) );

    // Materials
    osg::StateAttribute* sa( stateSet->getAttribute( osg::StateAttribute::MATERIAL ) );
    if( sa != NULL )
    {
        osg::Material* m( static_cast< osg::Material* >( sa ) );

        UniformBlockPtr frontMaterials( findMaterial( m ) );
        if( frontMaterials == NULL )
        {
            frontMaterials = jag::draw::UniformBlockPtr(
                new jag::draw::UniformBlock( "LightingMaterialFront" ) );
            frontMaterials->addUniform( jag::draw::UniformPtr(
                new jag::draw::Uniform( "ambient", VEC4_TO_GMTL_PT4F(m->getAmbient(osg::Material::FRONT)) ) ) );
            frontMaterials->addUniform( jag::draw::UniformPtr(
                new jag::draw::Uniform( "diffuse", VEC4_TO_GMTL_PT4F(m->getDiffuse(osg::Material::FRONT)) ) ) );
            frontMaterials->addUniform( jag::draw::UniformPtr(
                new jag::draw::Uniform( "specular", VEC4_TO_GMTL_PT4F(m->getSpecular(osg::Material::FRONT)) ) ) );
            frontMaterials->addUniform( jag::draw::UniformPtr(
                new jag::draw::Uniform( "shininess", m->getShininess( osg::Material::FRONT ) ) ) );
            addMaterial( m, frontMaterials );
        }
        ubsp->insert( frontMaterials );
    }

    // Texture2D
    {
    }

    if( !( ubsp->empty() ) )
        commands->insert( ubsp );

    if( !( commands->empty() ) )
        _current->setCommandMap( commands );
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

        jag::base::Point3fVec out;
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

        jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( gmtl::Point3f ), (void*)&( out[0] ) ) );
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

        jag::base::Point2fVec out;
        out.resize( size );
        unsigned int idx;
        for( idx=0; idx<size; idx++ )
        {
            const osg::Vec2& v( (*array)[ idx ] );
            gmtl::Point2f& p( out[ idx ] );
            p[ 0 ] = v[ 0 ];
            p[ 1 ] = v[ 1 ];
        }

        jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( gmtl::Point2f ), (void*)&( out[0] ) ) );
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

    jag::draw::GLubyteVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( GLubyte ), (void*)&out[0] ) );
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

    jag::draw::GLushortVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( GLushort ), (void*)&out[0] ) );
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

    jag::draw::GLuintVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
        out[ idx ] = (*arrayIn)[ idx ];

    jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( GLuint ), (void*)&out[0] ) );
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
