/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_DB_IMAGE_LOADER_DEF
#define CHASKII_DB_IMAGE_LOADER_DEF 1

#include <iostream>
#include <map>
#include <string>

#include <Chaskii/Export.h>
#include <Chaskii/DB/Image.h>
#include <Chaskii/Mem/ptr.h>

namespace iiDB {

class  ImageLoader
{
    public:
        ImageLoader() {}
        virtual ~ImageLoader() {}
        virtual Image_ptr load( const std::string &filename ){ return Image_ptr(); }
        virtual Image_ptr load( const std::istream & ) { return Image_ptr(); }
        virtual Image_ptr load( const char *inbuffer, size_t insize ) {  return Image_ptr(); }
    private:
};

typedef iiMem::ptr<ImageLoader>::shared_ptr ImageLoader_ptr;

class II_EXPORT ImageLoaderRegistry
{
    public:
        static ImageLoaderRegistry *instance();

        void registerImageLoader( const std::string &extension, ImageLoader_ptr );
        ImageLoader_ptr findImageLoaderForExtension( const std::string &extension );
        ImageLoader_ptr findImageLoaderForFile( const std::string &filename );

        void addAlias( const std::string &extension, const std::string &alias );
        const std::string &findAlias( const std::string &extension );

    private:
        ImageLoaderRegistry();
        virtual ~ImageLoaderRegistry();

        std::map<std::string, ImageLoader_ptr > registry;
        std::map<std::string, std::string> aliases;
};

extern II_EXPORT Image_ptr ReadImage( const std::string &filename );
extern II_EXPORT Image_ptr ReadImage( const std::string &ext, const std::istream & );
extern II_EXPORT Image_ptr ReadImage( const std::string &ext, char *buffer, size_t insize );

}

#endif
