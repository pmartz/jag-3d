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

#include <jagDraw/Image.h>

#include <fstream>


using namespace jagDraw;
using namespace jagDisk;


class ImageRW : public ReaderWriter
{
public:
    ImageRW()
      : ReaderWriter( "image" )
    {}
    virtual ~ImageRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
        return( ( extension == "gif" ) ||
            ( extension == "jpg" ) ||
            ( extension == "jpeg" ) ||
            ( extension == "tif" ) ||
            ( extension == "tiff" ) ||
            ( extension == "bmp" ) ||
            ( extension == "png" ) ||
            ( extension == "rgb" ) ||
            ( extension == "rgba" )
            );
    }

    virtual void* read( const std::string& fileName ) const
    {
        std::ifstream iStr( fileName.c_str() );
        if( !iStr )
        {
            // TBD record error
            return( NULL );
        }

        return( read( iStr ) );
    }
    virtual void* read( std::istream& iStr ) const
    {
        return( NULL );
#if 0
        jagDraw::Shader* shader( new jagDraw::Shader( _type ) );
        shader->addSourceString( shaderSource );

        return( shader );
#endif
    }

    virtual bool write( const std::string& fileName, const void* data ) const
    {
        std::ofstream oStr( fileName.c_str() );
        if( !oStr )
        {
            // TBD record error
            return( NULL );
        }

        return( write( oStr, data ) );
    }
    virtual bool write( std::ostream& oStr, const void* data ) const
    {
        return( false );
    }

protected:
};

// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new ImageRW(),   // Create an instance of MyMask.
    ImageRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write images to disk using OSG dependency."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ImageRW )
POCO_END_MANIFEST
