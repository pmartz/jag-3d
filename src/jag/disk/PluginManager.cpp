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
#include <jag/base/LogMacros.h>

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


namespace jag {
namespace disk {


ReaderWriterInfo::ReaderWriterInfo( ReaderWriterPtr instance, const std::string& className,
            const std::string& baseClassName, const std::string& description )
  : _className( className ),
    _baseClassName( baseClassName ),
    _description( description ),
    _rwInstance( instance )
{
#ifdef JAG3D_STATIC

    // Use special method for adding static ReaderWriters so that
    // we don't invoke PluginManager::internalInit() and create issues
    // with indeterminate order of static initialization.
    PluginManager::instance()->addStaticReaderWriter( *this );

#else

    PluginManager::PluginInfo* pi( PluginManager::instance()->getActivelyLoadingPlugin() );
    _pluginName = pi->_name;
    pi->_readerWriters.push_back( instance );

    PluginManager::instance()->addReaderWriter( *this );

#endif
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
  : _initState( Uninitialized ),
    _initFlags( initFlags ),
    _activelyLoadingPlugin( NULL ),
    _logName( "jag.disk.plugmgr" )
{
}
PluginManager::~PluginManager()
{
}

void PluginManager::internalInit()
{
    if( _initState != Uninitialized )
        return;
    _initState = Initializing;

    if( _initFlags != 0 )
    {
        JAG3D_TRACE_STATIC( _logName, "Constructor: Plugin search path includes: " );
    }

    if( ( _initFlags & USE_CURRENT_DIRECTORY ) != 0 )
    {
        JAG3D_TRACE_STATIC( _logName, "\tCurrent directory" );
        addPath( Poco::Path::current(), false );
    }
    if( ( _initFlags & USE_JAG3D_PLUGIN_PATH_ENV_VAR ) != 0 )
    {
        JAG3D_TRACE_STATIC( _logName, "\tJAG3D_PLUGIN_PATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "JAG3D_PLUGIN_PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }
    if( ( _initFlags & USE_SYSTEM_PATH ) != 0 )
    {
        JAG3D_TRACE_STATIC( _logName, "\tPATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }
    if( ( _initFlags & USE_LD_LIBRARY_PATH ) != 0 )
    {
        JAG3D_TRACE_STATIC( _logName, "\tLD_LIBRARY_PATH env var" );
        std::string paths;
        try {
            paths = Poco::Environment::get( "LD_LIBRARY_PATH" );
        } catch (...) {}
        if( !paths.empty() )
            addPaths( paths, false );
    }

    if( !( _rwInfo.empty() ) )
    {
        JAG3D_INFO_STATIC( _logName, "internalInit(): Already loaded (static):" );
        BOOST_FOREACH( const ReaderWriterInfo& rw, _rwInfo )
        {
            JAG3D_INFO_STATIC( _logName, "\t" + rw._className + ", " + rw._description );
        }
    }

    if( !_paths.empty() )
        loadConfigFiles();

    _initState = Initialized;
}

void PluginManager::addPath( const std::string& path, const bool loadConfigs )
{
    internalInit();

    _paths.push_back( path );
    if( loadConfigs )
        loadConfigFiles();
}
void PluginManager::addPaths( const std::string& paths, const bool loadConfigs )
{
    if( paths.empty() )
        return;

    internalInit();

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
    internalInit();

    _paths.clear();
}


bool PluginManager::loadPlugins( PluginInfoPtrVec& plugins )
{
    internalInit();

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
    internalInit();

    _activelyLoadingPlugin = pi;

    const Poco::Path& lib( pi->_path );

    typedef Poco::ClassLoader< ReaderWriter > LibLoader;
    LibLoader loader;
    try {
        loader.loadLibrary( lib.toString() );
    } catch( Poco::LibraryLoadException lle ) {
        JAG3D_ERROR_STATIC( _logName, "Can't load \"" + lib.toString() + "\":" );
        JAG3D_ERROR_STATIC( _logName, "\tCaught Poco::LibraryLoadException." );
        JAG3D_ERROR_STATIC( _logName, "\tMessage: " + lle.message() );
        return( false );
    } catch( ... ) {
        JAG3D_ERROR_STATIC( _logName, "Can't load \"" + lib.toString() + "\": unknown exception." );
        return( false );
    }

    pi->_loaded = true;
    return( true );
}

PluginManager::PluginInfoPtrVec PluginManager::getPluginsForExtension( const std::string& extension )
{
    internalInit();

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
    PluginManager* nonConstThis( const_cast< PluginManager* >( this ) );
    nonConstThis->internalInit();

    return( _rwInfo );
}


void PluginManager::addReaderWriter( const ReaderWriterInfo& rwInfo )
{
    internalInit();

    _rwInfo.push_back( rwInfo );
}
void PluginManager::addStaticReaderWriter( const ReaderWriterInfo& rwInfo )
{
    _rwInfo.push_back( rwInfo );
}


void PluginManager::loadConfigFiles()
{
    internalInit();

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
            JAG3D_DEBUG_STATIC( _logName, "Found plugin info file: " + jagpiFileName );

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
                JAG3D_WARNING_STATIC( _logName, "\tNot one of our .jagpi files, or badly formed .jagpi file." );
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

            JAG3D_DEBUG_STATIC( _logName, "\tLocation: " + pi._path.toString() );
            JAG3D_DEBUG_STATIC( _logName, "\tPlugin name: " + pi._name );
            JAG3D_DEBUG_STATIC( _logName, "\tPlugin desc: " + pi._description );
        }
    }
}

PluginManager::PluginInfo* PluginManager::getActivelyLoadingPlugin()
{
    internalInit();
    return( _activelyLoadingPlugin );
}


bool operator<( const PluginManager::PluginInfo& lhs, const PluginManager::PluginInfo& rhs )
{
    if( lhs._name < rhs._name )
        return( true );
    else
        return( false );
}


// namespace jag::disk::
}
}
