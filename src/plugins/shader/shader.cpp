/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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

        return( ReadStatus( boost::any( shader ) ) );
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
