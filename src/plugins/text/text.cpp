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

    virtual bool write( const std::string& fileName, const void* data, const Options* options ) const
    {
        std::ofstream oStr( fileName.c_str() );
        if( !oStr )
        {
            // TBD record error
            return( false );
        }

        const bool result( write( oStr, data, options ) );
        oStr.close();

        return( result );
    }
    virtual bool write( std::ostream& oStr, const void* data, const Options* /*options*/ ) const
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
