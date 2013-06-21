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
#include <Poco/String.h>

#include <jagDraw/Shader.h>

#include <fstream>
#include <string>


using namespace jagDraw;
using namespace jagDisk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class ShaderRW
\brief Loads shader source code from file.
*/
class ShaderRW : public ReaderWriter
{
public:
    ShaderRW()
      : ReaderWriter( "shader" ),
        _type( 0 )
    {}
    virtual ~ShaderRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
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
        // Get total file size. But because we're reading a text file,
        // the size of the actual data we read might be smaller. For
        // example, \cr\lf (two bytes) might be converted to \n (one byte).
        iStr.seekg( 0, std::ios::end );
        const std::istream::pos_type size( (unsigned long)( iStr.tellg() ) );
        iStr.seekg( 0, std::ios::beg );

        // totalSize reserves space for trailing '\0'.
        const unsigned int totalSize( ( (unsigned long) size ) + 1 );
        char* buff( new char[ totalSize ] );
        // Don't know where the trailing '\0' goes, so fill the whole
        // block with '0'.
        std::memset( buff, '\0', totalSize );

        iStr.read( buff, size );

        const std::string shaderSource( buff );
        delete[] buff;

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

/**@}*/


// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new ShaderRW(),   // Create an instance of ShaderRW.
    ShaderRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write shaders to disk."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ShaderRW )
POCO_END_MANIFEST
