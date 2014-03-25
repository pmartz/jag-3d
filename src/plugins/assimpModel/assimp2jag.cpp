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

#include "assimp2jag.h"

#include <jag/disk/ReaderWriter.h>
#include <jag/sg/Node.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/base/LogMacros.h>

#include <assimp/scene.h>

#include <sstream>


Assimp2Jag::Assimp2Jag( const aiScene* aiScene, const jag::disk::Options* options )
  : _logName( "jag.disk.rw.ai.ai2jag" ),
    _aiScene( *aiScene ),
    _vertexAttribName( "vertex" ),
    _normalAttribName( "normal" ),
    _texCoordAttribName( "texcoord" )
{
    JAG3D_TRACE_STATIC( _logName, "Assimp2Jag::Assimp2Jag" );
}
Assimp2Jag::~Assimp2Jag()
{
}

jag::sg::NodePtr Assimp2Jag::getJagScene()
{
    JAG3D_TRACE_STATIC( _logName, "Assimp2Jag::getJagScene" );

    if( _jagScene == NULL )
    {
        initData();
        _jagScene = traverse( _aiScene.mRootNode );
    }
    return( _jagScene );
}

void Assimp2Jag::initData()
{
    JAG3D_TRACE_STATIC( _logName, "Assimp2Jag::initData" );

    // Convert all aiScene meshes to JAG vertex array and Drawable objects.
    // The _vao and _draw member arrays will later be indexed by the aiNode's
    // mesh index.
    _vao.resize( _aiScene.mNumMeshes );
    _draw.resize( _aiScene.mNumMeshes );
    for( unsigned int idx=0; idx < _aiScene.mNumMeshes; idx++ )
    {
        JAG3D_TRACE_STATIC( _logName, "\tmesh" );

        const struct aiMesh* currentMesh( _aiScene.mMeshes[ idx ] );

        jag::draw::VertexArrayObjectPtr& vao( _vao[ idx ] );
        vao.reset( new jag::draw::VertexArrayObject() );

        jag::draw::DrawablePtr& draw( _draw[ idx ] );
        draw.reset( new jag::draw::Drawable() );

        // Vertices
        {
            ArrayInfo info( get3fData( currentMesh->mVertices, currentMesh->mNumVertices ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
            vao->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Vertex );

            jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
                _vertexAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
            vao->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Vertex );
        }

        // Normals
        if( currentMesh->mNormals != NULL )
        {
            ArrayInfo info( get3fData( currentMesh->mNormals, currentMesh->mNumVertices ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
            vao->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Vertex );

            jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
                _normalAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
            vao->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Normal );
        }

        // Tangents
        // Bitangents
        // Colors
        // Texcoords

        // Faces
        ArrayInfo info( getUIntFaceData( currentMesh ) );
        jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

        jag::draw::DrawElementsPtr drawcom( new jag::draw::DrawElements(
            GL_TRIANGLES, info._numElements, GL_UNSIGNED_INT, NULL, bop ) );
        draw->addDrawCommand( drawcom );

        // Bones
        // Name
        // Animations
    }
}

jag::sg::NodePtr Assimp2Jag::traverse( const aiNode* aiNode )
{
    JAG3D_TRACE_STATIC( _logName, "Assimp2Jag::traverse" );

    jag::sg::NodePtr parent( new jag::sg::Node() );

    for( unsigned int idx=0; idx < aiNode->mNumMeshes; idx++ )
    {
        const unsigned int meshIdx( aiNode->mMeshes[ idx ] );

        jag::sg::NodePtr node( new jag::sg::Node() );
        parent->addChild( node );
        jag::draw::CommandMapPtr& commands( node->getOrCreateCommandMap() );

        commands->insert( _vao[ meshIdx ] );
        node->addDrawable( _draw[ meshIdx ] );

        node->setTransform( asGMTLMatrix( aiNode->mTransformation ) );
    }

    for( unsigned int idx=0; idx < aiNode->mNumChildren; idx++ )
        parent->addChild( traverse( aiNode->mChildren[ idx ] ) );

    return( parent );
}


Assimp2Jag::ArrayInfo Assimp2Jag::get3fData( const aiVector3D* data, const unsigned int size ) const
{
    ArrayInfo info;

    info._type = GL_FLOAT;
    info._numElements = size;
    info._componentsPerElement = 3;

    jag::base::Point3fVec out;
    out.resize( size );
    unsigned int idx;
    for( idx=0; idx<size; idx++ )
    {
        const aiVector3D& v( data[ idx ] );
        gmtl::Point3f& p( out[ idx ] );
        p[ 0 ] = v[ 0 ];
        p[ 1 ] = v[ 1 ];
        p[ 2 ] = v[ 2 ];
    }

    jag::base::BufferPtr bp( new jag::base::Buffer( size * sizeof( gmtl::Point3f ), (void*)&( out[0] ) ) );
    info._buffer = bp;
    return( info );
}
Assimp2Jag::ArrayInfo Assimp2Jag::getUIntFaceData( const struct aiMesh* mesh ) const
{
    ArrayInfo info;

    info._type = GL_UNSIGNED_INT;
    info._numElements = mesh->mNumFaces * 3;
    info._componentsPerElement = 1;

    jag::draw::GLuintVec out;
    out.resize( info._numElements );
    unsigned int count( 0 );

    for( unsigned int faceIdx=0; faceIdx < mesh->mNumFaces; ++faceIdx )
    {
        const struct aiFace& face = mesh->mFaces[ faceIdx ];
        if( face.mNumIndices != 3 )
            JAG3D_ERROR_STATIC( _logName, "Unexpected number of face indices." );
                
        for( unsigned int idx=0; idx < face.mNumIndices; ++idx )
            out[ count++ ] = face.mIndices[ idx ];
    }
    if( count != info._numElements )
        JAG3D_ERROR_STATIC( _logName, "Unexpected total index count." );

    jag::base::BufferPtr bp( new jag::base::Buffer( info._numElements * sizeof( GLuint ), (void*)&out[0] ) );
    info._buffer = bp;
    return( info );
}
gmtl::Matrix44d Assimp2Jag::asGMTLMatrix( aiMatrix4x4 m )
{
    m.Transpose();
    gmtl::Matrix44d retVal;
    retVal.set( (double)m.a1, (double)m.a2, (double)m.a3, (double)m.a4,
        (double)m.b1, (double)m.b2, (double)m.b3, (double)m.b4,
        (double)m.c1, (double)m.c2, (double)m.c3, (double)m.c4,
        (double)m.d1, (double)m.d2, (double)m.d3, (double)m.d4 );
    return( retVal );
}
