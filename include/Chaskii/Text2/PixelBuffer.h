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
#ifndef PIXEL_BUFFER_DEF
#define PIXEL_BUFFER_DEF 1

#include <string.h>
#include <Chaskii/Export.h>
#include <Chaskii/Mem/ref_ptr.h>

class II_EXPORT PixelBuffer
{
    public:
        enum Format {
            Luminance = 0x1909,
            LuminanceAlpha = 0x190A,
            RGB = 0x1907,
            RGBA = 0x1908
        };

        PixelBuffer();

        PixelBuffer( Format format );
        PixelBuffer( Format format, unsigned int width, unsigned int height );
        void resize( int width, int height );

        void fill( unsigned char r, 
                   unsigned char g,
                   unsigned char b,
                   unsigned char a );


        Format getFormat() const { return _format; }
        unsigned int getWidth() const { return _width; }
        unsigned int getHeight() const { return _height; }
        unsigned int getNumComponents() const { return _ncomp; }

        void clear();

        unsigned char *ptr() const;

        unsigned char *operator[](size_t index);


    private:
        Format _format;
        unsigned int _ncomp;
        unsigned int _width, _height;
        unsigned int _bufsize;
        iiMem::ref_array_ptr<unsigned char> _buff;

        void _setNumComponents()
        {
            _ncomp = _format == Luminance ? 1:
                     _format == LuminanceAlpha ? 2:
                     _format == RGB? 3 :
                     _format == RGBA? 4 : 4;
        }
};

#endif
