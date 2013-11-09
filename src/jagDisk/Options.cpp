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
