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

#include <jag/disk/PluginManager.h>
#include <jag/disk/ReaderWriter.h>
#include <jagSG/Node.h>
#include <jag/base/LogMacros.h>

#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include "osg2jag.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Node>
#include <osg/Version>

#include <fstream>


using namespace jag::disk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class OSGModelRW
\brief OSG-based model data loader.
*/
class OSGModelRW : public ReaderWriter
{
public:
    OSGModelRW()
      : ReaderWriter( "osg-model" )
    {}
    virtual ~OSGModelRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
        return( ( extension == "gif" ) ||
            ( extension == "osg" ) ||
            ( extension == "osgt" ) ||
            ( extension == "ive" ) ||
            ( extension == "osgb" ) ||
            ( extension == "flt" ) ||
            ( extension == "3ds" ) ||
            ( extension == "obj" )
            );
    }

    virtual ReadStatus read( const std::string& fileName, const Options* options ) const
    {
        JAG3D_INFO(
            std::string( "Using OSG v" ) + std::string( osgGetVersion() ) );

        osg::ref_ptr< osg::Node > osgNode( osgDB::readNodeFile( fileName ) );
        if( !( osgNode.valid() ) )
            return( ReadStatus() );

        Osg2Jag osg2Jag( options );
        osgNode->accept( osg2Jag );
        jagSG::NodePtr result( osg2Jag.getJagScene() );
        return( ReadStatus( boost::any( result ) ) );
    }
    virtual ReadStatus read( std::istream& iStr, const Options* /*options*/ ) const
    {
        return( ReadStatus() );
    }

    virtual bool write( const std::string& fileName, const void* data, const Options* /*options*/ ) const
    {
        osg::ref_ptr< osg::Node > osgNode( convertToOsgNode( (jagSG::Node*)data ) );
        return( osgDB::writeNodeFile( *osgNode, fileName ) );
    }
    virtual bool write( std::ostream& oStr, const void* data, const Options* /*options*/ ) const
    {
        return( false );
    }

protected:
    osg::Node* convertToOsgNode( jagSG::Node* jagNode ) const
    {
        return( NULL );
    }
};

/**@}*/


// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new OSGModelRW(),   // Create an instance of ModelRW.
    OSGModelRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write models to disk using OSG dependency."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( OSGModelRW )
POCO_END_MANIFEST
