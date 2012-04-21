#ifndef WIN32
#include <dlfcn.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <Chaskii/DB/PluginLoader.h>
#include <Chaskii/DB/ImageLoader.h>

#ifdef WIN32
	#include <io.h>
    #define F_OK 4
#else
	#include <unistd.h>
#endif

inline bool fileExists( const std::string &path )
{
    return access( path.c_str(), F_OK ) == 0;
}


namespace iiDB {

static class Preloader
{
    public:
        Preloader()
        {
            char *env = 0;
            if( (env = getenv( "CHASKII_DB_PRELOAD_IMAGE_LOADER" ) ) != NULL )
            {
#ifdef WIN32
			    LoadLibrary( env );
#else
                if( (dlopen( env, RTLD_LAZY )) == NULL )
                    fprintf(stderr,"dlerror() - %s\n", dlerror() );
#endif
            }
        }
} __ilri;


ImageLoaderRegistry::ImageLoaderRegistry()
{
}

ImageLoaderRegistry::~ImageLoaderRegistry()
{}

ImageLoaderRegistry *ImageLoaderRegistry::instance()
{
    static ImageLoaderRegistry *theImageLoaderRegistry = new ImageLoaderRegistry;
    return theImageLoaderRegistry;
}

void ImageLoaderRegistry::registerImageLoader( const std::string &extension, ImageLoader_ptr loader )
{
    registry[extension] = loader;
}

ImageLoader_ptr ImageLoaderRegistry::findImageLoaderForExtension( const std::string &ext )
{
    std::map<std::string, ImageLoader_ptr >::iterator p = registry.find( ext );
    if( p != registry.end() )
        return p->second;
    else
    {
        if( PluginLoader::instance()->loadPlugin( ext ) == true )
        p = registry.find( ext );
        if( p == registry.end() )
            return ImageLoader_ptr();
        else
            return p->second;

    }
    return ImageLoader_ptr();
}

ImageLoader_ptr ImageLoaderRegistry::findImageLoaderForFile( const std::string &filename )
{
    if( filename.empty() )
        return ImageLoader_ptr();
    int pos = filename.find_last_of('.');
    if( pos < 0 )
        return ImageLoader_ptr();

    std::string ext = filename.substr( pos+1 );
    return findImageLoaderForExtension( ext );
}


Image_ptr ReadImage( const std::string & filename )
{
    ImageLoader_ptr loader = ImageLoaderRegistry::instance()->findImageLoaderForFile( filename );
    if( loader == NULL )
    {
        fprintf(stderr, "[ReadImage()] - Can't find a loader for file \"%s\"\n", filename.c_str() );
        return Image_ptr();
    }

    Image_ptr image( loader->load( filename ) );
    image->name = filename;
    return image;
}

Image_ptr ReadImage( const std::string &ext, const std::istream &istream )
{
    ImageLoader_ptr loader = ImageLoaderRegistry::instance()->findImageLoaderForExtension( ext );
    if( loader == NULL )
    {
        fprintf(stderr, "[ReadImage()] Can't find a loader for extension \"%s\"\n", ext.c_str() );
        return Image_ptr();
    }

    return loader->load( istream );
}

Image_ptr ReadImage( const std::string &ext, char *buffer, size_t insize )
{
    ImageLoader_ptr loader = ImageLoaderRegistry::instance()->findImageLoaderForExtension( ext );
    if( loader == NULL )
    {
        fprintf(stderr, "[ReadImage()]Can't find a loader for extension \"%s\"\n", ext.c_str() );
        return Image_ptr();
    }

    return loader->load( buffer, insize );
}

}

