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
#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include <jag/draw/Shader.h>

#include <fstream>
#include <string>


using namespace jag::disk;
using namespace jag::draw;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class ShaderRW
\brief Loads shader source code from file.
*/
class ShaderRW : public ReaderWriter
{
    std::string _logName;

public:
    ShaderRW()
      : _logName( "jag.disk.rw.shader" ),
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

    virtual ReadStatus read( const std::string& fileName, const Options* options ) const
    {
        std::ifstream iStr( fileName.c_str() );
        if( !iStr )
        {
            // TBD record error
            return( ReadStatus() );
        }

        Poco::Path pathName( fileName );
        const std::string extension( pathName.getExtension() );
        if( isVert( extension ) )
            _type = GL_VERTEX_SHADER;
        else if( isGeom( extension ) )
            _type = GL_GEOMETRY_SHADER;
        else if( isFrag( extension ) )
            _type = GL_FRAGMENT_SHADER;

        ReadStatus readStatus( read( iStr, options ) );
        _type = 0;
        iStr.close();

        return( readStatus );
    }
    virtual ReadStatus read( std::istream& iStr, const Options* /*options*/ ) const
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

        ShaderPtr shader( ShaderPtr( new Shader( _type ) ) );
        shader->addSourceString( shaderSource );

        boost::any tempAny( shader );
        return( ReadStatus( tempAny ) );
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
JAG3D_REGISTER_READERWRITER(
    shader,           // Plugin library name.
    new ShaderRW(),   // Create an instance of ShaderRW.
    ShaderRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write shaders to disk."  // Description text.
);


#ifndef JAG3D_STATIC

// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ShaderRW )
POCO_END_MANIFEST

#endif
