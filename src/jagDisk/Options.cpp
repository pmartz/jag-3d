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

#include <jagDisk/Options.h>
#include <jagBase/LogMacros.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/Environment.h>

#include <boost/foreach.hpp>


namespace jagDisk {


Options::Options( const int initFlags )
{
    if( ( initFlags & USE_CURRENT_DIRECTORY ) != 0 )
    {
        addPath( Poco::Path::current() );
    }
    if( ( initFlags & USE_JAG3D_DATA_PATH_ENV_VAR ) != 0 )
    {
        std::string paths;
        try {
            paths = Poco::Environment::get( "JAG3D_DATA_PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths );
    }
}
Options::Options( const Options& rhs )
  : _paths( rhs._paths ),
  _anyMap( rhs._anyMap )
{
}
Options::~Options()
{
}


void Options::addPath( const std::string& path )
{
    _paths.push_back( path );
}
void Options::addPaths( const std::string& paths )
{
    if( paths.empty() )
        return;

    const char sep( Poco::Path::pathSeparator() );
    std::string::size_type start( 0 ), end( paths.find( sep ) );
    while( start != std::string::npos )
    {
        if( start != end )
            _paths.push_back( paths.substr( start, end-start ) );
        start = ( end == std::string::npos ) ? end : end+1;
        end = paths.find( sep, start );
    }
}
void Options::clearPaths()
{
    _paths.clear();
}
const Poco::Path::StringVec& Options::getPaths() const
{
    return( _paths );
}

Poco::Path Options::findFile( const std::string& fileName ) const
{
    Poco::File file( fileName );
    if( file.exists() )
        return( Poco::Path( fileName ) );

    Poco::Path pathName;
    bool found( Poco::Path::find( _paths.begin(), _paths.end(), fileName, pathName ) );
    if( found )
        return( pathName );

    {
        const std::string logStr( "jag.disk.opt" );
        JAG3D_WARNING_STATIC( logStr, "findFile() could not find " + fileName );
        JAG3D_WARNING_STATIC( logStr, "using the following search locations:" );
        BOOST_FOREACH( std::string name, _paths )
        {
            JAG3D_WARNING_STATIC( logStr, "\t" + name );
        }
    }

    return( pathName );
}


// jagDisk
}
