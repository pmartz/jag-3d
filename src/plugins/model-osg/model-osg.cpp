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
#include <jagSG/Node.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include "osg2jag.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Node>

#include <fstream>


using namespace jagDisk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class ModelRW
\brief OSG-based model data loader.
*/
class ModelRW : public ReaderWriter
{
public:
    ModelRW()
      : ReaderWriter( "model" )
    {}
    virtual ~ModelRW()
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

    virtual void* read( const std::string& fileName ) const
    {
        osg::ref_ptr< osg::Node > osgNode( osgDB::readNodeFile( fileName ) );
        if( !( osgNode.valid() ) )
            return( NULL );

        Osg2Jag osg2Jag;
        osgNode->accept( osg2Jag );
        return( osg2Jag.getJagScene() );
    }
    virtual void* read( std::istream& iStr ) const
    {
        return( NULL );
    }

    virtual bool write( const std::string& fileName, const void* data ) const
    {
        osg::ref_ptr< osg::Node > osgNode( convertToOsgNode( (jagSG::Node*)data ) );
        return( osgDB::writeNodeFile( *osgNode, fileName ) );
    }
    virtual bool write( std::ostream& oStr, const void* data ) const
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
    new ModelRW(),   // Create an instance of ModelRW.
    ModelRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Read and write models to disk using OSG dependency."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ModelRW )
POCO_END_MANIFEST
