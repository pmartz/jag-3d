#ifndef CHASKII_IMAGE_LOADER_JPG_DEF
#define CHASKII_IMAGE_LOADER_JPG_DEF

#include <Chaskii/DB/Image.h>
#include <Chaskii/DB/ImageLoader.h>

namespace iiDB {

class ImageLoaderJpg: public ImageLoader
{
    public:
        ImageLoaderJpg();
        ~ImageLoaderJpg();

        virtual Image_ptr load( const std::string &filename ) ;
        virtual Image_ptr load( const std::istream &fin ) ;
        virtual Image_ptr load( const char *inbuffer, size_t insize ) ;

};

}

#endif
