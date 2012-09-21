/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2012 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jagDisk/PluginManager.h>
#include <jagDisk/ReaderWriter.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>

#include <jagDraw/Shader.h>

#include <fstream>


using namespace jagDraw;
using namespace jagDisk;


class ShaderRW : public ReaderWriter
{
public:
    ShaderRW()
      : ReaderWriter( "shaderrw" ),
        _type( 0 )
    {}
    virtual ~ShaderRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( extension );
        return( isVert( allLower ) ||
            isGeom( allLower ) ||
            isFrag( allLower ) );
    }

    virtual void* read( const std::string& fileName ) const
    {
        std::ifstream iStr( fileName.c_str() );
        if( !iStr )
        {
            // TBD record error
            return( NULL );
        }

        Poco::Path pathName( fileName );
        const std::string extension( pathName.getExtension() );
        if( isVert( extension ) )
            _type = GL_VERTEX_SHADER;
        else if( isGeom( extension ) )
            _type = GL_GEOMETRY_SHADER;
        else if( isFrag( extension ) )
            _type = GL_FRAGMENT_SHADER;

        void* data( read( iStr ) );
        _type = 0;
        return( data );
    }
    virtual void* read( std::istream& iStr ) const
    {
        std::string shaderSource;
        shaderSource.reserve( 4096 );

        char buf[ 1024 ];
        while( iStr.good() )
        {
            iStr.getline( buf, 128 );
            shaderSource += std::string( buf ) + "\n";
        }
        shaderSource += std::string( "\0" );

        jagDraw::Shader* shader( new jagDraw::Shader( _type ) );
        shader->addSourceString( shaderSource );

        return( shader );
    }

protected:
    static bool isVert( const std::string& extension )
    {
        return( ( extension == std::string( "vs" ) ) ||
            ( extension == std::string( "vert" ) ) );
    }
    static bool isGeom( const std::string& extension )
    {
        return( ( extension == std::string( "gs" ) ) ||
            ( extension == std::string( "geom" ) ) );
    }
    static bool isFrag( const std::string& extension )
    {
        return( ( extension == std::string( "fs" ) ) ||
            ( extension == std::string( "frag" ) ) );
    }

    mutable GLenum _type;
};

// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new ShaderRW(),   // Create an instance of MyMask.
    ShaderRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write shaders to disk."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ShaderRW )
POCO_END_MANIFEST
