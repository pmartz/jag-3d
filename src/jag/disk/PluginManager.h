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

#ifndef __JAG_DISK_PLUGIN_MANAGER_H__
#define __JAG_DISK_PLUGIN_MANAGER_H__ 1


#include <jag/disk/Export.h>
#include <jag/disk/ReaderWriter.h>
#include <jag/base/types.h>

#include <Poco/Path.h>
#include <string>
#include <set>
#include <vector>
#include <list>


namespace jag {
namespace disk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/


/** \class ReaderWriterInfo PluginManager.h <jag/disk/PluginManager.h>
\brief A collection of information for classes exported from a plugin.
\details Plugins create an instance of ReaderWriterInfo for each class (ReaderWriterInfo-derived class)
exported by the plugin. The constructor automatically registers all information with the PluginManager
singleton, using PluginManager::addReaderWriter().
\see JAG3D_REGISTER_READERWRITER
*/
struct JAGDISK_EXPORT ReaderWriterInfo
{
    ReaderWriterInfo( ReaderWriterPtr instance, const std::string& pluginName,
            const std::string& className,
            const std::string& baseClassName, const std::string& description );

    std::string _pluginName;
    std::string _className, _baseClassName, _description;
    ReaderWriterPtr _rwInstance;

    friend bool operator<( const ReaderWriterInfo& lhs, const ReaderWriterInfo& rhs );
};
typedef std::vector< ReaderWriterInfo > ReaderWriterInfoVec;
typedef std::list< ReaderWriterInfo* > ReaderWriterInfoList;

bool operator<( const ReaderWriterInfo& lhs, const ReaderWriterInfo& rhs );


/** \def REGISTER_READERWRITER
\brief Convenience CPP macro for plugin class registration.
\details Plugins use this macro once for every exported class they contain.
The macro declares a static instance of type ReaderWriterInfo. Static instances
are initialized immediately following plugin (shared libraries) load, which
causes each exported class to immediately register itself with the PluginManager.
See PluginManager::addReaderWriter(), which is invoked by the ReaderWriterInfo
constructor.

For static linking, this macro defines a function jagp_<plugin>, such as jagp_shader
or jagp_text. Client code can statically link with a plugin library using the
JAG3D_REFERENCE_PLUGIN macro, which calls this function. */
#define JAG3D_REGISTER_READERWRITER(_plugin,_instance,_className,_baseClassName,_description) \
extern "C" void jagp_##_plugin( void ) {} \
static ReaderWriterInfo staticRWRegistration_##_className( \
    ReaderWriterPtr( _instance ), #_plugin, #_className, _baseClassName, _description )



/** \class PluginManager PluginManager.h <jag/disk/PluginManager.h>
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
capability can be added as future work.

During a read or write operation (jag/disk/ReadWrite.cpp), the PluginManager provides
a list of loaded ReaderWriters, and each is attempted in turn to fulfill the read or
write request. By default, the list of ReaderWriters is in an indeterminate order.
Applications can control the order by setting a gloval plugin preference list with an
environment variable, JAG3D_PLUGIN_PREFERENCE, which is a comma-separated list of plugin
names (minus the \c jagp- prefix, such as "osgModel,assimpModel,osgImage,shader,text").
Alternately, the application can set the global plugin preference list with a call to
setDefaultPluginPreference(const jag::base::StringVec&).
\envvar JAG3D_PLUGIN_PREFERENCE
*/
class JAGDISK_EXPORT PluginManager
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


    /** \struct PluginInfo PluginManager.h <jag/disk/PluginManager.h>
    \brief Information about a dynamic linked plugin, collected from a .jagpi file.
    \details loadConfigFiles() creates a PluginInfo struct for each
    .jagpi file that it finds. They are stored in the _pluginInfo vector.
    If a plugin in this vector can support a read or write operation,
    PluginManager loads that plugin as needed, and its contained
    ReaderWriter-derived class instances are stored in the _rwInfo vec.
    */
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
    /** \brief Load names plugins. Plugins must already be on _pluginInfo vec. */
    bool loadPlugins( const jag::base::StringVec& plugins );
    /** \brief Load a single plugin. */
    bool loadPlugin( PluginInfo* pi );

    /** \brief Return a list of all plugins that support the specified \c extension. */
    PluginInfoPtrVec getPluginsForExtension( const std::string& extension );


    /** \brief Return a list of ReaderWriters sorted by the default plugin preference.
    \details Returns a list of loaded ReaderWriters sorted in priority order from
    the default plugin preferences. This function loads a plugin if it appears in the
    plugin preferences list and isn't already loaded. Initially, the default plugin
    preference list is empty, and ReaderWriters are in an indeterminate order. Set
    the default plugin preference list with setDefaultPluginPreference(). */
    const ReaderWriterInfoVec& getLoadedReaderWriters() const;


    /** \brief Adds an ReaderWriterInfo record to the list of registered operations.
    \details The operation is associated with the actively loading plugin (getActivelyLoadingPlugin()).
    This function is primarily used by the ReaderWriterInfo constructor, and indirectly
    by the REFISTER_OPERATION convenience macro for registration of plugin Operations. */
    void addReaderWriter( const ReaderWriterInfo& rwInfo );
    /** \brief Adds an ReaderWriterInfo record to the list of registered operations.
    \details Same as addReaderWriter, but does not invoke internalInit() and therefore
    avoids issues with indetermnate order of static initialization. */
    void addStaticReaderWriter( const ReaderWriterInfo& rwInfo );

    /* \brief Set the default plugin preference list.
    \details By default, the plugin preference list is empty, and read/write operations
    attempt to use ReaderWriters in an indeterminate order. This function changes that
    default behavior so that ReaderWriters are attempted in the order specified by
    \c plugins. This is particularly useful when more than one plugin supports the
    same extension.
    */
    void setDefaultPluginPreference( const jag::base::StringVec& plugins );

    /** \brief Sort the list of ReaderWriters by the list of preferred plugins.
    */
    static void sortReaderWriters( ReaderWriterInfoVec& rwInfo, const jag::base::StringVec& plugins );

    /** \brief Returns the plugin name of the most recently loaded plugin.
    \details This function is not inteded for direct application use, but is public
    for easy access by the ReaderWriterInfo class.

    The actively loading plugin name is set by the loadPlugin() and loadPlugins()
    functions immediately prior to actually loading the plugin. When the load occurs, 
    plugin static initializers invoke addReaderWriter(), which calls getActivelyLoadingPlugin()
    and associates all newly registered plugin operations with the actively loading plugin. */
    PluginInfo* getActivelyLoadingPlugin();


protected:
    PluginManager( const int initFlags );
    void internalInit();

    enum {
        Uninitialized = 0,
        Initializing = 1,
        Initialized = 2
    };
    int _initState;

    int _initFlags;

    PluginInfo* _activelyLoadingPlugin;

    Poco::Path::StringVec _paths;

    // For dynamic loading, info collected from all the .jagpi files
    // that were found during PluginManager::loadConfigFiles().
    // Not used by static-linked plugins.
    PluginInfoVec _pluginInfo;

    // Default plugin preference. ReaderWriters from plugins in this vector
    // are tried in priority order to fulfill read and write operations.
    jag::base::StringVec _pluginPreference;

    // For dynamic loading, list of ReaderWriter-derived class instances that
    // have been dynamically loaded. For static-linked plugins, the list of
    // static-linked ReaderWriters.
    ReaderWriterInfoVec _rwInfo;

    // PluginManager can't use jag::base::LogBase due to indeterminate
    // order of static initialization issue, so a separate _logName is
    // required for use with the JAG3D_<priority>_STATIC log macros.
    std::string _logName;
};

bool operator<( const PluginManager::PluginInfo& lhs, const PluginManager::PluginInfo& rhs );

/**@}*/


// namespace jag::disk::
}
}


// __JAG_DISK_PLUGIN_MANAGER_H__
#endif
