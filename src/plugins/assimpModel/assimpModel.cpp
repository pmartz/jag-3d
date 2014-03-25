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

#include <jag/base/Config.h>
#include <jag/disk/PluginManager.h>
#include <jag/disk/ReaderWriter.h>
#include <jag/sg/Node.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/base/LogMacros.h>

#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include <assimp/ai_assert.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/version.h>

#include <sstream>


using namespace jag::disk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class AssimpModelRW
\brief Open Asset Import (ASSIMP)-based model loader.

\logname jag.disk.rw.assimp
*/
class AssimpModelRW : public jag::disk::ReaderWriter
{
    std::string _logName;

public:
    AssimpModelRW()
      : _logName( "jag.disk.rw.assimp" ),
        _vertexAttribName( "vertex" ),
        _normalAttribName( "normal" ),
        _texCoordAttribName( "texcoord" )
    {
        Assimp::DefaultLogger::create( "", Assimp::Logger::NORMAL, aiDefaultLogStream_STDOUT );
    }
    virtual ~AssimpModelRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
        if( allLower == "dae" ) return( true );
        if( allLower == "blend" ) return( true );
        if( allLower == "3ds" ) return( true );
        if( allLower == "ase" ) return( true );
        if( allLower == "obj" ) return( true );
        if( allLower == "xgl" ) return( true );
        if( allLower == "ply" ) return( true );
        if( allLower == "dxf" ) return( true );
        if( allLower == "lwo" ) return( true );
        if( allLower == "lxo" ) return( true );
        if( allLower == "stl" ) return( true );
        if( allLower == "x" ) return( true );
        if( allLower == "ac" ) return( true );
        if( allLower == "ms3d" ) return( true );
        if( allLower == "scn" ) return( true );
        if( allLower == "xml" ) return( true );
        if( allLower == "irrmesh" ) return( true );
        if( allLower == "mdl" ) return( true );
        if( allLower == "md3" ) return( true );
        if( allLower == "md5" ) return( true );
        if( allLower == "smd" ) return( true );
        if( allLower == "m3" ) return( true );
        if( allLower == "3d" ) return( true );
        if( allLower == "q3d" ) return( true );
        if( allLower == "off" ) return( true );
        if( allLower == "ter" ) return( true );
        return( false );
    }


    virtual ReadStatus read( const std::string& fileName, const Options* options ) const
    {
        {
            std::ostringstream ostr;
            ostr << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." <<
                aiGetVersionRevision() << ", 0x" << std::hex << aiGetCompileFlags();
            JAG3D_INFO_STATIC( _logName, std::string( "Using ASSIMP v" ) +
                std::string( ostr.str() ) );
        }


        Assimp::Importer importer;
        const aiScene* aiScene( importer.ReadFile( fileName.c_str(),
            aiProcess_Triangulate ) );
        if( aiScene == NULL )
        {
            JAG3D_INFO_STATIC( _logName, "Unable to create aiScene. ASSIMP error:" );
            JAG3D_INFO_STATIC( _logName, std::string( "\t" ) + importer.GetErrorString() );
            return( ReadStatus() );
        }
        
        jag::sg::NodePtr result( traverse( aiScene, aiScene->mRootNode ) );
        boost::any tempAny( result );
        return( ReadStatus( tempAny ) );
    }
    
protected:
    std::string _vertexAttribName, _normalAttribName, _texCoordAttribName;

    struct ArrayInfo {
        jag::base::BufferPtr _buffer;
        GLenum _type;
        unsigned int _numElements;
        unsigned int _componentsPerElement;
    };

    ArrayInfo get3fData( const aiVector3D* data, const unsigned int size ) const
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
    ArrayInfo getUIntFaceData( const struct aiMesh* mesh ) const
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
    static gmtl::Matrix44d asGMTLMatrix( aiMatrix4x4 m )
    {
        m.Transpose();
        gmtl::Matrix44d retVal;
        retVal.set( (double)m.a1, (double)m.a2, (double)m.a3, (double)m.a4,
            (double)m.b1, (double)m.b2, (double)m.b3, (double)m.b4,
            (double)m.c1, (double)m.c2, (double)m.c3, (double)m.c4,
            (double)m.d1, (double)m.d2, (double)m.d3, (double)m.d4 );
        return( retVal );
    }


    jag::sg::NodePtr traverse( const aiScene* aiScene, const struct aiNode* aiNode ) const
    {
        jag::draw::DrawablePtr draw( new jag::draw::Drawable() );
        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );

        for( unsigned int meshIdx=0; meshIdx < aiNode->mNumMeshes; ++meshIdx )
        {
            const struct aiMesh* currentMesh( aiScene->mMeshes[ aiNode->mMeshes[ meshIdx ] ] );

            {
                ArrayInfo info( get3fData( currentMesh->mVertices, currentMesh->mNumVertices ) );
                jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
                vaop->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Vertex );

                jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
                    _vertexAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
                vaop->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Vertex );
            }

            if( currentMesh->mNormals != NULL )
            {
                ArrayInfo info( get3fData( currentMesh->mNormals, currentMesh->mNumVertices ) );
                jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, info._buffer ) );
                vaop->addVertexArrayCommand( bop, jag::draw::VertexArrayObject::Vertex );

                jag::draw::VertexAttribPtr attrib( new jag::draw::VertexAttrib(
                    _normalAttribName, info._componentsPerElement, info._type, GL_FALSE, 0, 0 ) );
                vaop->addVertexArrayCommand( attrib, jag::draw::VertexArrayObject::Vertex );
            }

            ArrayInfo info( getUIntFaceData( currentMesh ) );
            jag::draw::BufferObjectPtr bop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, info._buffer ) );

            jag::draw::DrawElementsPtr drawcom( new jag::draw::DrawElements(
                GL_TRIANGLES, info._numElements, GL_UNSIGNED_INT, NULL, bop ) );
            draw->addDrawCommand( drawcom );
        }

        jag::sg::NodePtr node( new jag::sg::Node() );
        node->addDrawable( draw );

        node->setTransform( asGMTLMatrix( aiNode->mTransformation ) );

        jag::draw::CommandMapPtr& commands( node->getOrCreateCommandMap() );
        commands->insert( vaop );


        // Traverse the assimp node tree.
        for( unsigned int childIdx=0; childIdx < aiNode->mNumChildren; ++childIdx )
        {
            jag::sg::NodePtr child( traverse( aiScene, aiNode->mChildren[ childIdx ] ) );
            if( child != NULL )
                node->addChild( child );
        }


        return( node );
    }
};

/**@}*/


// Register the assimpModelRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
JAG3D_REGISTER_READERWRITER(
    assimpModel,           // Plugin library name.
    new AssimpModelRW(),   // Create an instance of ModelRW.
    AssimpModelRW,         // Class name -- NOT a string.
    "ReaderWriter",     // Base class name as a string.
    "Read and write models to disk using ASSIMP dependency."  // Description text.
);


#ifndef JAG3D_STATIC

// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( AssimpModelRW )
POCO_END_MANIFEST

#endif
