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
#ifndef CHASKII_DB_IMAGE_DEF
#define CHASKII_DB_IMAGE_DEF 1

#include <string>
#include <Chaskii/Export.h>
#include <Chaskii/OpenGL.h>
#include <Chaskii/Mem/ptr.h>

namespace iiDB {

class II_EXPORT Image {
    public:
        unsigned int width;
        unsigned int height;
        unsigned int depth;
        unsigned int numComponents;
        unsigned int size;
        unsigned int mipmaps;
        GLenum gltarget;
        GLenum glformat;
        GLenum gltype;
        bool compressed;
        std::string name;

        iiMem::ptr<GLbyte>::shared_array_ptr data;

        Image();
        Image( unsigned int width, unsigned int height, unsigned int depth, unsigned int numComponents, GLbyte *data );

        virtual ~Image() ;

        void setWidth( unsigned int w ) { width = w; }
        unsigned int getWidth() const { return width; }

        void setHeight( unsigned int h ) { height = h; }
        unsigned int getHeight() const { return height; }

        void setDepth( unsigned int d ) { depth = d; }
        unsigned int getDepth() const { return depth; }

        void setNumComponents( unsigned int nc ) { numComponents = nc; }
        unsigned int getNumComponents() const { return numComponents; }

        void setOpenGLFormat(GLenum fmt) { glformat = fmt; }
        GLenum getOpenGLFormat() const { return glformat; }

        void setOpenGLTarget(GLenum target) { gltarget = target; }
        GLenum getOpenGLTarget() const { return gltarget; }

        void setOpenGLType(GLenum type) { gltype = type; }
        GLenum getOpenGLType() const { return gltype; }

        const std::string &getName() const { return name; }

        GLbyte *getData() const { return data.get(); }

        bool isLoaded();
        bool isCompressed() { return compressed; }
        bool flipY();

};

typedef iiMem::ptr<Image>::shared_ptr Image_ptr;

}

#endif

