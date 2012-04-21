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
#ifndef CHASKII2TEXT_TYPEFACE_DEF
#define CHASKII2TEXT_TYPEFACE_DEF 1

#include <string>
#include <vector>

#include <Chaskii/Export.h>
#include <Chaskii/Mem/ref_ptr.h>
#include <Chaskii/Math/vec2.h>

namespace iiText2 {

class II_EXPORT Typeface 
{
    public:

        /**
          Default Constructor. Uses the default font and pixels size
          */

        Typeface();
        virtual ~Typeface();
        /** 
          Constructor. Specify the name of the font to be used and pixel size.
          Pixel size in this context refers to the size in pixels of the bitmap
          rendered into the Texture Pallette.  The bigger the size, the higher 
          the quality of the rendering, but the more use of resources*/
        Typeface( const std::string &fontname, unsigned int pixelSize=32 );

        void genScaledCoordsHoriz( double &penx, double &peny, unsigned int index, 
                                          std::vector<iiMath::vec2> &scaledCoords, 
                                          std::vector<iiMath::vec2> &texCoords );

        double getKerningDistanceHoriz( int g0, int g1 );
        unsigned int getGlyphIndex( unsigned int ascii );

        unsigned int getPixelSize();

        /**
          getTextStringWidth() returns the width of a string, taking into account
          kerning.  The value returned is a ratio of width to height.  

          Note that this is not a world space value, but could be used to determine 
          the width of a string in world space by applying the ratio to the known 
          height in world space of the text 
          */
          
        double getTextStringWidth( const char *text );

        void getYMinMax( double &ymin, double &ymax );


        void apply();

    private:
        struct PrivateData;
#ifdef WIN32 // Let windows leak.  It deserves it
            PrivateData *pd;
#else
            iiMem::ref_ptr<PrivateData> pd;
#endif

        void reset();

};

}

#endif
