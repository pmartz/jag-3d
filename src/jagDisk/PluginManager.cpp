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
#include <jagBase/LogMacros.h>

#include <Poco/Glob.h>
#include <Poco/File.h>
#include <Poco/SharedLibrary.h>
#include <Poco/ClassLoader.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Environment.h>
#include <Poco/AutoPtr.h>
#include <boost/foreach.hpp>

#include <sstream>
#include <iostream>
#include <set>


using Poco::Util::IniFileConfiguration;


namespace jagDisk {


ReaderWriterInfo::ReaderWriterInfo( ReaderWriterPtr instance, const std::string& className,
            const std::string& baseClassName, const std::string& description )
  : _className( className ),
    _baseClassName( baseClassName ),
    _description( description ),
    _rwInstance( instance )
{
    PluginManager::PluginInfo* pi( PluginManager::instance()->getActivelyLoadingPlugin() );
    _pluginName = pi->_name;
    pi->_readerWriters.push_back( instance );

    PluginManager::instance()->addReaderWriter( *this );
}

bool operator<( const ReaderWriterInfo& lhs, const ReaderWriterInfo& rhs )
{
    if( lhs._pluginName < rhs._pluginName )
        return( true );
    else if( lhs._pluginName > rhs._pluginName )
        return( false );

    if( lhs._className < rhs._className )
        return( true );
    else if( lhs._className > rhs._className )
        return( false );

    if( lhs._baseClassName < rhs._baseClassName )
        return( true );
    else if( lhs._baseClassName > rhs._baseClassName )
        return( false );

    if( lhs._description < rhs._description )
        return( true );
    else
        return( false );
}




PluginManager* PluginManager::instance( const int initFlags )
{
    static PluginManager* s_instance( new PluginManager( initFlags ) );
    return( s_instance );
}

PluginManager::PluginManager( const int initFlags )
  : LogBase( "jag.disk.plugmgr" ),
    _activelyLoadingPlugin( NULL )
{
    if( initFlags != 0 )
    {
        JAG3D_TRACE( "Constructor: Plugin search path includes: " );
    }

    if( ( initFlags & USE_CURRENT_DIRECTORY ) != 0 )
    {
        JAG3D_TRACE( "\tCurrent directory" );
        addPath( Poco::Path::current(), false );
    }
    if( ( initFlags & USE_JAG3D_PLUGIN_PATH_ENV_VAR ) != 0 )
    {
        JAG3D_TRACE( "\tJAG3D_PLUGIN_PATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "JAG3D_PLUGIN_PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }
    if( ( initFlags & USE_SYSTEM_PATH ) != 0 )
    {
        JAG3D_TRACE( "\tPATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }
    if( ( initFlags & USE_LD_LIBRARY_PATH ) != 0 )
    {
        JAG3D_TRACE( "\tLD_LIBRARY_PATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "LD_LIBRARY_PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }

    if( !_paths.empty() )
        loadConfigFiles();
}
PluginManager::~PluginManager()
{
}

void PluginManager::addPath( const std::string& path, const bool loadConfigs )
{
    _paths.push_back( path );
    if( loadConfigs )
        loadConfigFiles();
}
void PluginManager::addPaths( const std::string& paths, const bool loadConfigs )
{
    if( paths.empty() )
        return;

    const char sep( Poco::Path::pathSeparator() );
    std::string::size_type start( 0 ), end( paths.find( sep ) );
    while( start != std::string::npos )
    {
        if( start != end )
        {
            _paths.push_back( paths.substr( start, end-start ) );
        }
        start = ( end == std::string::npos ) ? end : end+1;
        end = paths.find( sep, start );
    }

    if( loadConfigs )
        loadConfigFiles();
}
void PluginManager::clearPaths()
{
    _paths.clear();
}


bool PluginManager::loadPlugins( PluginInfoPtrVec& plugins )
{
    BOOST_FOREACH( PluginInfo* pi, plugins )
    {
        if( pi->_loaded )
            continue;

        if( !( loadPlugin( pi ) ) )
            // Abort.
            return( false );
    }

    return( true );
}

bool PluginManager::loadPlugin( PluginInfo* pi )
{
    _activelyLoadingPlugin = pi;

    const Poco::Path& lib( pi->_path );

    typedef Poco::ClassLoader< ReaderWriter > LibLoader;
    LibLoader loader;
    try {
        loader.loadLibrary( lib.toString() );
    } catch( Poco::LibraryLoadException lle ) {
        JAG3D_ERROR( "Can't load \"" + lib.toString() + "\":" );
        JAG3D_ERROR( "\tCaught Poco::LibraryLoadException." );
        JAG3D_ERROR( "\tMessage: " + lle.message() );
        return( false );
    } catch( ... ) {
        JAG3D_ERROR( "Can't load \"" + lib.toString() + "\": unknown exception." );
        return( false );
    }

    pi->_loaded = true;
    return( true );
}

PluginManager::PluginInfoPtrVec PluginManager::getPluginsForExtension( const std::string& extension )
{
    PluginInfoPtrVec plugins;

    BOOST_FOREACH( PluginInfo& pi, _pluginInfo )
    {
        if( ( pi._extensions.find( extension ) != pi._extensions.end() ) ||
            ( pi._extensions.find( "*" ) != pi._extensions.end() ) )
            // The PluginInfo either supports this extension explicitly,
            // or claims to support all extensions ("*").
            plugins.push_back( &pi );
    }

    return( plugins );
}


const ReaderWriterInfoVec& PluginManager::getLoadedReaderWriters() const
{
    return( _rwInfo );
}


void PluginManager::addReaderWriter( const ReaderWriterInfo& rwInfo )
{
    _rwInfo.push_back( rwInfo );
}


void PluginManager::loadConfigFiles()
{
    typedef std::set< std::string > StringSet;

    _pluginInfo.clear();

    BOOST_FOREACH( Poco::Path::StringVec::value_type s, _paths )
    {
        Poco::Path path, libPath;
        if( !( path.tryParse( s.c_str() ) ) )
            continue;
        path.makeDirectory();
        path.setFileName( "*.jagpi" );

        StringSet stringSet;
        Poco::Glob::glob( path, stringSet, Poco::Glob::GLOB_DOT_SPECIAL );
        BOOST_FOREACH( StringSet::value_type jagpiFileName, stringSet )
        {
            JAG3D_DEBUG( "Found plugin info file: " + jagpiFileName );

            std::string extensions;
            Poco::AutoPtr< IniFileConfiguration > conf( new IniFileConfiguration( jagpiFileName ) );
            PluginInfo pi;
            try {
                extensions = conf->getString( "Jag3DPlugin.Extensions" );
                pi._name = conf->getString( "Jag3DPlugin.Name" );
                pi._description = conf->getString( "Jag3DPlugin.Description" );
            }
            catch( ... )
            {
                JAG3D_WARNING( "\tNot one of our .jagpi files, or badly formed .jagpi file." );
                continue;
            }

            // Parse the extensions string into a vector of individual strings.
            while( !( extensions.empty() ) )
            {
                size_t pos( extensions.find( ";" ) );
                if( pos != std::string::npos )
                {
                    pi._extensions.insert( extensions.substr( 0, pos ) );
                    extensions = extensions.substr( pos+1 );
                }
                else
                {
                    pi._extensions.insert( extensions );
                    extensions.clear();
                }
            }

            // Create a platform-specific plugin name. Note that
            // Poco has SharedLibrary::suffix(), which seems useful,
            // except it returns ".dylib" on OSX instead of ".so".
            // For this reason, we must roll our own...
            pi._path = path;
            std::string debugSuffix( "" );
#ifdef _DEBUG
            debugSuffix = "d";
#endif
            std::string platformPluginName;
#if( POCO_OS == POCO_OS_WINDOWS_NT )
            platformPluginName = Poco::Path( jagpiFileName ).getBaseName() + debugSuffix + ".dll";
#else
            platformPluginName = "lib" + Poco::Path( jagpiFileName ).getBaseName() + debugSuffix + ".so";
#endif
            pi._path.setFileName( platformPluginName );
            if( !( Poco::File( pi._path ).exists() ) )
                continue;

            _pluginInfo.push_back( pi );

            JAG3D_DEBUG( "\tLocation: " + pi._path.toString() );
            JAG3D_DEBUG( "\tPlugin name: " + pi._name );
            JAG3D_DEBUG( "\tPlugin desc: " + pi._description );
        }
    }
}


bool operator<( const PluginManager::PluginInfo& lhs, const PluginManager::PluginInfo& rhs )
{
    if( lhs._name < rhs._name )
        return( true );
    else
        return( false );
}


// jagDisk
}
