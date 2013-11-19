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
#include <jagSG/Node.h>
#include <jagBase/LogMacros.h>

#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include "osg2jag.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Node>
#include <osg/Version>

#include <fstream>


using namespace jagDisk;


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
