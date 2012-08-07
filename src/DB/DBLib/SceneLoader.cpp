#ifndef WIN32
#include <dlfcn.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include <Chaskii/DB/PluginLoader.h>
#include <Chaskii/DB/SceneLoader.h>
#include <Chaskii/SceneGraph/Node.h>

#ifdef WIN32
	#include <io.h>
    #define F_OK 4
#else
	#include <unistd.h>
#endif

namespace iiDB {

static inline bool fileExists( const std::string &path )
{
    return access( path.c_str(), F_OK ) == 0;
}

SceneLoaderRegistry::SceneLoaderRegistry()
{
}

SceneLoaderRegistry::~SceneLoaderRegistry()
{}

SceneLoaderRegistry *SceneLoaderRegistry::instance()
{
    static SceneLoaderRegistry *theSceneLoaderRegistry = new SceneLoaderRegistry;
    return theSceneLoaderRegistry;
}

void SceneLoaderRegistry::registerSceneLoader( const std::string &extension, SceneLoader *loader )
{
    registry[extension] = loader;
}

SceneLoader *SceneLoaderRegistry::findSceneLoaderForExtension( const std::string &ext )
{
    std::map<std::string, iiMem::ref_ptr<SceneLoader> >::iterator p = registry.find( ext );

    if( p != registry.end() )
        return p->second.get();
    else
    {
        if( PluginLoader::instance()->loadPlugin( ext ) == true )
            p = registry.find( ext );

        if( p == registry.end() )
            return NULL;
        else
            return p->second.get();

    }
    return NULL;
}

SceneLoader *SceneLoaderRegistry::findSceneLoaderForFile( const std::string &filename )
{
    if( filename.empty() )
        return NULL;

    int pos = filename.find_last_of('.');
    if( pos < 0 )
        return NULL;

    std::string ext = filename.substr( pos+1 );
    return findSceneLoaderForExtension( ext );
}


iiSceneGraph::Node * LoadScene( const std::string & filename )
{
    SceneLoader *loader = SceneLoaderRegistry::instance()->findSceneLoaderForFile( filename );
    if( loader == NULL )
    {
        fprintf(stderr, "[LoadScene()] - Can't find a loader for file \"%s\"\n", filename.c_str() );
        return NULL;
    }

    return loader->load( filename );
}

iiSceneGraph::Node * LoadScene( const std::string &ext, const std::istream &istream )
{
    SceneLoader *loader = SceneLoaderRegistry::instance()->findSceneLoaderForExtension( ext );
    if( loader == NULL )
    {
        fprintf(stderr, "[LoadScene()] Can't find a loader for extension \"%s\"\n", ext.c_str() );
        return NULL;
    }

    return loader->load( istream );
}

iiSceneGraph::Node * LoadScene( const std::string &ext, char *buffer, size_t insize )
{
    SceneLoader *loader = SceneLoaderRegistry::instance()->findSceneLoaderForExtension( ext );
    if( loader == NULL )
    {
        fprintf(stderr, "[LoadScene()]Can't find a loader for extension \"%s\"\n", ext.c_str() );
        return NULL;
    }

    return loader->load( buffer, insize );
}

}

