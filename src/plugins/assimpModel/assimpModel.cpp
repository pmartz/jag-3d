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
#include <jag/sg/Node.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/base/LogMacros.h>

#include <Poco/ClassLibrary.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/version.h>

#include "assimp2jag.h"

#include <sstream>


using namespace jag::disk;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class AssimpModelRW
\brief Open Asset Import (ASSIMP)-based model loader.

\logname jag.disk.rw.assimp
*/
class AssimpModelRW : public jag::disk::ReaderWriter
{
    std::string _logName;

public:
    AssimpModelRW()
      : _logName( "jag.disk.rw.ai" )
    {
        Assimp::DefaultLogger::create( "", Assimp::Logger::NORMAL, aiDefaultLogStream_STDOUT );
    }
    virtual ~AssimpModelRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string allLower( Poco::toLower( extension ) );
        if( allLower == "dae" ) return( true );
        if( allLower == "blend" ) return( true );
        if( allLower == "3ds" ) return( true );
        if( allLower == "ase" ) return( true );
        if( allLower == "obj" ) return( true );
        if( allLower == "xgl" ) return( true );
        if( allLower == "ply" ) return( true );
        if( allLower == "dxf" ) return( true );
        if( allLower == "lwo" ) return( true );
        if( allLower == "lxo" ) return( true );
        if( allLower == "stl" ) return( true );
        if( allLower == "x" ) return( true );
        if( allLower == "ac" ) return( true );
        if( allLower == "ms3d" ) return( true );
        if( allLower == "scn" ) return( true );
        if( allLower == "xml" ) return( true );
        if( allLower == "irrmesh" ) return( true );
        if( allLower == "mdl" ) return( true );
        if( allLower == "md3" ) return( true );
        if( allLower == "md5" ) return( true );
        if( allLower == "smd" ) return( true );
        if( allLower == "m3" ) return( true );
        if( allLower == "3d" ) return( true );
        if( allLower == "q3d" ) return( true );
        if( allLower == "off" ) return( true );
        if( allLower == "ter" ) return( true );
        return( false );
    }


    virtual ReadStatus read( const std::string& fileName, const Options* options ) const
    {
        {
            std::ostringstream ostr;
            ostr << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." <<
                aiGetVersionRevision() << ", 0x" << std::hex << aiGetCompileFlags();
            JAG3D_INFO_STATIC( _logName, std::string( "Using ASSIMP v" ) +
                std::string( ostr.str() ) );
        }


        Assimp::Importer importer;
        const aiScene* aiScene( importer.ReadFile( fileName.c_str(),
            aiProcess_Triangulate ) );
        if( aiScene == NULL )
        {
            JAG3D_INFO_STATIC( _logName, "Unable to create aiScene. ASSIMP error:" );
            JAG3D_INFO_STATIC( _logName, std::string( "\t" ) + importer.GetErrorString() );
            return( ReadStatus() );
        }

        JAG3D_TRACE_STATIC( _logName, "Converting to JAG..." );
        Assimp2Jag assimp2Jag( aiScene, options );
        jag::sg::NodePtr result( assimp2Jag.getJagScene() );

        boost::any tempAny( result );
        return( ReadStatus( tempAny ) );
    }
};

/**@}*/


// Register the assimpModelRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
JAG3D_REGISTER_READERWRITER(
    assimpModel,           // Plugin library name.
    new AssimpModelRW(),   // Create an instance of ModelRW.
    AssimpModelRW,         // Class name -- NOT a string.
    "ReaderWriter",     // Base class name as a string.
    "Read and write models to disk using ASSIMP dependency."  // Description text.
);


#ifndef JAG3D_STATIC

// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( AssimpModelRW )
POCO_END_MANIFEST

#endif
