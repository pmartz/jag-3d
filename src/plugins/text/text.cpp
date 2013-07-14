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

#include "TextDumpSG.h"
#include "TextDumpDG.h"
#include <jagSG/Node.h>
#include <jagDraw/DrawGraph.h>

#include <jagBase/LogMacros.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include <fstream>
#include <string>


using namespace jagDisk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class TextDump
\brief Dump information about scene graphs and draw graphs to text file.
*/
class TextDump : public ReaderWriter
{
public:
    TextDump()
      : ReaderWriter( "text" )
    {}
    virtual ~TextDump()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
        return( ( extension == "txt" ) ||
            ( extension == "text" )
            );
    }

    virtual void* read( const std::string& fileName ) const
    {
        return( NULL );
    }
    virtual void* read( std::istream& iStr ) const
    {
        return( NULL );
    }

    virtual bool write( const std::string& fileName, const void* data ) const
    {
        std::ofstream oStr( fileName.c_str() );
        if( !oStr )
        {
            // TBD record error
            return( false );
        }

        const bool result( write( oStr, data ) );
        oStr.close();

        return( result );
    }
    virtual bool write( std::ostream& oStr, const void* data ) const
    {
        const jagSG::Node* node( static_cast< const jagSG::Node* >( data ) );
        if( node != NULL )
        {
            TextDumpSG tdsg( oStr );
            const_cast< jagSG::Node* >( node )->accept( tdsg );
            return( true );
        }

        JAG3D_NOTICE( "Text dump for jagDraw::DrawGraph: not yet implemented." );
#if 0
        const jagDraw::DrawGraph* drawGraph( dynamic_cast< const jagDraw::DrawGraph* >( data ) );
        if( drawGraph != NULL )
        {
            TextDumpDG tddg( oStr );
            drawGraph->accept( tddg );
            return( true );
        }
#endif

        return( false );
    }

protected:
};

/**@}*/


// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new TextDump(),   // Create an instance of ImageRW.
    TextDump,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Dump information about scene graphs and draw graphs to text file."  // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( TextDump )
POCO_END_MANIFEST
