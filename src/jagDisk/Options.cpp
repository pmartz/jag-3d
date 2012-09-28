/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

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
  : _paths( rhs._paths )
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
