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

#ifndef __JAGDISK_PLUGIN_MANAGER_H__
#define __JAGDISK_PLUGIN_MANAGER_H__ 1


#include <jagDisk/Export.h>
#include <jagDisk/ReaderWriter.h>
#include <jagBase/LogBase.h>
#include <jagBase/types.h>

#include <Poco/Path.h>
#include <string>
#include <set>


namespace jagDisk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \class ReaderWriterInfo PluginManager.h <jagDisk/PluginManager.h>
\brief A collection of information for classes exported from a plugin.
\details Plugins create an instance of ReaderWriterInfo for each class (ReaderWriterInfo-derived class)
exported by the plugin. The constructor automatically registers all information with the PluginManager
singleton, using PluginManager::addReaderWriter().
\see REGISTER_READERWRITER
*/
struct JAGDISK_EXPORT ReaderWriterInfo
{
    ReaderWriterInfo( ReaderWriterPtr instance, const std::string& className,
            const std::string& baseClassName, const std::string& description );

    std::string _pluginName;
    std::string _className, _baseClassName, _description;
    ReaderWriterPtr _rwInstance;

    friend bool operator<( const ReaderWriterInfo& lhs, const ReaderWriterInfo& rhs );
};
typedef std::vector< ReaderWriterInfo > ReaderWriterInfoVec;

bool operator<( const ReaderWriterInfo& lhs, const ReaderWriterInfo& rhs );


/** \def REGISTER_READERWRITER
\brief Convenience CPP macro for plugin class registration.
\details Plugins use this macro once for every exporter class they contain.
The macro declares a static instance of type ReaderWriterInfo. Static instances
are initialized immediately following plugin (shared libraries) load, which
causes each exported class to immediately register itself with the PluginManager.
See PluginManager::addReaderWriter(), which is invoked by the ReaderWriterInfo
constructor. */
#define REGISTER_READERWRITER(_instance,_className,_baseClassName,_description) \
static ReaderWriterInfo staticRWRegistration_##_className( \
    ReaderWriterPtr( _instance ), #_className, _baseClassName, _description )



/** \class PluginManager PluginManager.h <jagDisk/PluginManager.h>
\brief A singleton for managing plugins, plugin search paths, and plugin contents.
\details The PluginManager provides an interface for finding plugins, loading them,
and creating instances of classes contained in plugin.

PluginManager is a singleton, so the first access to PluginManager::instance()
creates the single instance. instance() takes parameter to indicate default plugin
search paths. By default, the current directory, and all directories listed in the
environment variable JAG3D_PLUGIN_PATH, are added to the list of search paths.
Additional paths can be added using addPath() and addPaths().
\envvar JAG3D_PLUGIN_PATH

The PluginManager doesn't search for plugins (shared libraries) directly. Instead,
it searches for plugin description files with the extension *.ini. These files use
the Microsoft INI file format and contain a generic plugin name and descrition text.
For an example plugin description file, see data/plugin-example.ini. Applications
load a plugin using loadPlugin() and loadPlugins(). The name parameter is the generic
plugin name from the .ini file. Both the .ini file and the shared library must share the
same base name and exist in the same directory. This system allows multiple plugins to
use the same generic plugin name.

Plugins use the REGISTER_OPERATION macro to register a class (derived from ReaderWriter)
with the PluginManager. Use of REGISTER_OPERATION results in a call to addReaderWriter()
when the plugin is loaded.

To create an instance of a plugin class object, applications call createOperation().
Currently, there is no way to query the list of classes contained in a plugin, but this
capability can be added as future work. */
class JAGDISK_EXPORT PluginManager : protected jagBase::LogBase
{
public:
    typedef enum {
        NO_PATHS                    = 0,
        USE_CURRENT_DIRECTORY       = ( 1 << 0x0 ),
        USE_JAG3D_PLUGIN_PATH_ENV_VAR = ( 1 << 0x1 ),
        USE_SYSTEM_PATH             = ( 1 << 0x2 ),
        USE_LD_LIBRARY_PATH         = ( 1 << 0x3 )
    } InitFlags;

    /** \brief Create the PluginManager singleton instance.
    \param initFlags specifies default plugin search paths.
    */
    static PluginManager* instance( const int initFlags=( USE_CURRENT_DIRECTORY | USE_SYSTEM_PATH | USE_JAG3D_PLUGIN_PATH_ENV_VAR ) );
    virtual ~PluginManager();

    /** \brief Add a single plugin search path.
    \details Adds the specified path to the list of paths.
    \param loadConfigs If true, implicitly call loadConfigFiles() to search all
    paths for plugin config files. This is inefficient when calling addPath()
    multiple times. If false, loadConfigFiles() is not called; the calling code
    is responsible for calling this function to search for plugin config files.
    */
    void addPath( const std::string& path, const bool loadConfigs=true );
    /** \brief Like addPath, but loads multiple paths.
    \details \c paths is a colon (on Unix) or semi-colon (on Windows) separated
    list of paths to add. */
    void addPaths( const std::string& paths, const bool loadConfigs=true );
    /** \brief Clear the list of plugin search paths. */
    void clearPaths();
    /** \brief Find plugin config files in all plugin search paths.
    \details In general, applications don't need to call this function explicitly.
    It's called automatically with the second parameter to addPath() and addPaths()
    is true. Also called automatically by the PluginManager constructor when the singleton
    instance is first created. However, applications that make multiple calls to addPath()
    should pass false as the second parameter, then call loadConfigFiles() after the final
    addPath() for efficiency reasons. */
    void loadConfigFiles();


    struct PluginInfo
    {
        PluginInfo( const std::string& name=std::string( "" ) )
          : _loaded( false ),
            _name( name )
        {}

        mutable bool _loaded;

        Poco::Path _path;         /**< Full path and name to plugin shared library. */
        std::string _name;        /**< Taken from plugin .jagpi file */
        std::string _description; /**< Taken from plugin .jagpi file */

        typedef std::set< std::string > StringSet;
        StringSet _extensions; /**< Taken from plugin .jagpi file "Extensions" string */

        ReaderWriterVec _readerWriters;

        friend bool operator<( const PluginInfo& lhs, const PluginInfo& rhs );
    };
    typedef std::vector< PluginInfo > PluginInfoVec;
    typedef std::vector< PluginInfo* > PluginInfoPtrVec;

    /** \brief Load all specified plugins. */
    bool loadPlugins( PluginInfoPtrVec& plugins );
    /** \brief Load a single plugin. */
    bool loadPlugin( PluginInfo* pi );

    /** \brief Return a list of all plugins that support the specified \c extension. */
    PluginInfoPtrVec getPluginsForExtension( const std::string& extension );


    const ReaderWriterInfoVec& getLoadedReaderWriters() const;


    /** \brief Adds an ReaderWriterInfo record to the list of registered operations.
    \details The operation is associated with the actively loading plugin (getActivelyLoadingPlugin()).
    This function is primarily used by the ReaderWriterInfo constructor, and indirectly
    by the REFISTER_OPERATION convenience macro for registration of plugin Operations. */
    void addReaderWriter( const ReaderWriterInfo& rwInfo );

    /** \brief Returns the plugin name of the most recently loaded plugin.
    \details This function is not inteded for direct application use, but is public
    for easy access by the ReaderWriterInfo class.

    The actively loading plugin name is set by the loadPlugin() and loadPlugins()
    functions immediately prior to actually loading the plugin. When the load occurs, 
    plugin static initializers invoke addReaderWriter(), which calls getActivelyLoadingPlugin()
    and associates all newly registered plugin operations with the actively loading plugin. */
    PluginInfo* getActivelyLoadingPlugin() { return( _activelyLoadingPlugin ); }


protected:
    PluginManager( const int initFlags );

    PluginInfo* _activelyLoadingPlugin;

    Poco::Path::StringVec _paths;
    PluginInfoVec _pluginInfo;

    ReaderWriterInfoVec _rwInfo;
};

bool operator<( const PluginManager::PluginInfo& lhs, const PluginManager::PluginInfo& rhs );

/**@}*/


// jagDisk
}


// __JAGDISK_PLUGIN_MANAGER_H__
#endif
