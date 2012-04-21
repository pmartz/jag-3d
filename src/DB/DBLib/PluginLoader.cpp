#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <stdio.h>
#endif

#include <Chaskii/DB/PluginLoader.h>

namespace iiDB {

PluginLoader::PluginLoader()
{
    _aliasMap["ive"] = "osg";
    _aliasMap["geo"] = "osg";
}

PluginLoader::~PluginLoader()
{
}


PluginLoader *PluginLoader::instance()
{
    static PluginLoader * thePluginLoader = new PluginLoader;
    return thePluginLoader;
}

bool PluginLoader::loadPlugin(const std::string &ext )
{
    std::string l_ext = ext;
    std::map<std::string,std::string>::iterator p = _aliasMap.find(ext);
    if( p != _aliasMap.end() )
        l_ext = p->second;


#ifdef WIN32
    std::string plugin_name = "libii_" + l_ext + ".dll"; 
    std::string path = "iiPlugins/" + plugin_name; 
    LoadLibrary( LPCSTR(plugin_name.c_str()) ); 

    return true; 
#else
  #ifdef __APPLE__
    std::string plugin_name = "libii_" + l_ext + ".so";

    // First look in App Bundle
    // Next look in $(HOME)/Library/Application Support/Chaskii/iiPlugins
    char *home = getenv( "HOME" ); 
    if( home != NULL ) 
    { 
        std::string path = std::string(home) + "/Library/Application Support/Chaskii/iiPlugins/" + plugin_name; 
        fprintf(stderr, "[%s:%d] checking %s\n", __FILE__, __LINE__, path.c_str()); 
        if( dlopen( path.c_str(), RTLD_LAZY ) != NULL ) 
        { 
            return true; 
        } 
    } 
    
    // Next look in /Library/Application Support/Chaskii/iiPlugins 
    std::string path = "/Library/Application Support/Chaskii/iiPlugins/" + plugin_name; 
    fprintf(stderr, "[%s:%d] checking %s\n", __FILE__, __LINE__, path.c_str()); 
    if( dlopen( path.c_str(), RTLD_LAZY ) != NULL ) 
    { 
        return true; 
    } 
    return false;

  #else

    std::string plugin_name = "libii_" + l_ext + ".so";
    void *handle = dlopen( plugin_name.c_str(), RTLD_LAZY );
    if(handle == NULL)
    { 
        char *errmsg = dlerror(); 
        fprintf(stderr, "dlerror reports: %s\n", errmsg); 
        return false; 
    } 
    return true;

  #endif
#endif
}



}
