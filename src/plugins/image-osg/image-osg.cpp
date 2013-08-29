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

#include <jagBase/Buffer.h>
#include <jagBase/LogMacros.h>
#include <jagDraw/Image.h>
#include <jagDraw/PixelStore.h>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Image>
#include <osg/Version>

#include <boost/any.hpp>
#include <fstream>


using namespace jagDraw;
using namespace jagDisk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class OSGImageRW
\brief OSG-based image data loader.
*/
class OSGImageRW : public ReaderWriter
{
public:
    OSGImageRW()
      : ReaderWriter( "osg-image" )
    {}
    virtual ~OSGImageRW()
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

    virtual ReadStatus read( const std::string& fileName, const Options* /*options*/ ) const
    {
        JAG3D_INFO(
            std::string( "Using OSG v" ) + std::string( osgGetVersion() ) );

        osg::ref_ptr< osg::Image > osgImage( osgDB::readImageFile( fileName ) );
        if( osgImage == NULL )
        {
            JAG3D_ERROR( std::string( "Can't load file " ) + fileName );
            return( ReadStatus() );
        }

        ImagePtr image( convertFromOsgImage( osgImage.get() ) );
        return( ReadStatus( boost::any( image ) ) );
    }
    virtual ReadStatus read( std::istream& iStr, const Options* /*options*/ ) const
    {
        return( ReadStatus() );
    }

    virtual bool write( const std::string& fileName, const void* data, const Options* /*options*/ ) const
    {
        JAG3D_INFO(
            std::string( "Using OSG v" ) + std::string( osgGetVersion() ) );

        osg::ref_ptr< osg::Image > osgImage( convertToOsgImage( (Image*)data ) );
        return( osgDB::writeImageFile( *osgImage, fileName ) );
    }
    virtual bool write( std::ostream& oStr, const void* data, const Options* /*options*/ ) const
    {
        return( false );
    }

protected:
    ImagePtr convertFromOsgImage( osg::Image* osgImage ) const
    {
        if( osgImage == NULL ) return( NULL );

        const unsigned int size( osgImage->getTotalSizeInBytes() );
        osgImage->setAllocationMode( osg::Image::NO_DELETE );

        // Check for deprecated internal formats and change to valid ones.
        GLenum intFormat;
        switch( osgImage->getInternalTextureFormat() )
        {
        case 1:
        case GL_ALPHA:
        case GL_LUMINANCE:
            intFormat = GL_RED;
            break;
        case 2:
        case GL_LUMINANCE_ALPHA:
            intFormat = GL_RG;
            break;
        case 3:
            intFormat = GL_RGB;
            break;
        case 4:
            intFormat = GL_RGBA;
            break;
        default:
            intFormat = osgImage->getInternalTextureFormat();
            break;
        }

        // Also check for deprecated formats.
        GLenum format;
        switch( osgImage->getPixelFormat() )
        {
        case GL_ALPHA:
        case GL_LUMINANCE:
            format = GL_RED;
            break;
        case GL_LUMINANCE_ALPHA:
            format = GL_RG;
            break;
        default:
            format = osgImage->getPixelFormat();
            break;
        }

        ImagePtr newImage( new Image() );
        newImage->set( 0, intFormat,
            osgImage->s(), osgImage->t(), osgImage->r(), 0,
            format, osgImage->getDataType(),
            const_cast< unsigned char* >(
                (const unsigned char*)osgImage->getDataPointer() ) );

        PixelStorePtr pixelStore( new PixelStore() );
        pixelStore->_alignment = osgImage->getPacking();
        newImage->setPixelStore( pixelStore );

        return( newImage );
    }

    osg::Image* convertToOsgImage( Image* jagImage ) const
    {
        return( NULL );
    }
};

/**@}*/


// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new OSGImageRW(),   // Create an instance of ImageRW.
    OSGImageRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write images to disk using OSG dependency."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( OSGImageRW )
POCO_END_MANIFEST
