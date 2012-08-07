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
#ifndef LOCAL_TEXT_DEF
#define LOCAL_TEXT_DEF 1

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Chaskii/Export.h>
#include <Chaskii/Text2/PixelBuffer.h>

typedef std::vector<unsigned char> Utf8String;

class II_EXPORT Text2D
{
    public:
        Text2D();

        void setFont( const std::string &font );
        void setString( const std::string &s );
        void setColor( float r, float g, float b, float a=1.0f );
        void setPixelSize( size_t pixelSize) ;
        size_t getPixelSize();
        void setString( const Utf8String &s );

        virtual void renderToPixelBuffer( int x, int y, PixelBuffer &pbuff );

        int getWidthInPixels( const std::string &s);
        int getWidthInPixels();
        void getPixelDimensions( unsigned int &width, unsigned int &height );
        void getPixelExtents( int &x0, int &y0, int &x1, int &y1 );

        unsigned int getHorizontalAdvance(); // in pixels
        const Utf8String &getString() const; 

    private:
        FT_Face _face;
        Utf8String _utf8String;
        std::vector<unsigned int> _gi;
        std::string _font;
        unsigned char _color[4];
        int _pixelSize;
        static int  _default_font_size;
        static unsigned char _default_font[];

        void _init();

};


#endif
