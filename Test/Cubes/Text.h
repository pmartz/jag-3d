#ifndef LOCAL_TEXT_DEF
#define LOCAL_TEXT_DEF 1

#include <string>
#include <vector>
#include <Chaskii/Math/vec4.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef std::vector<unsigned char> Utf8String;

class Text
{
    public:
        Text();
        Text( const std::string &font, const iiMath::vec4 &color, int pixelSize);

        void setFont( const std::string &font );

        void setString( const std::string &s );

        void setColor( const iiMath::vec4 &color );

        void setColor( float r, float g, float b, float a=1.0f );

        void setPixelSize( size_t pixelSize) ;
        void setString( const Utf8String &s );
        void setPosition( int x, int y );
        virtual void render();

        int getWidthInPixels( const std::string &s);
        int getWidthInPixels();
        void getPixelDimensions( unsigned int &width, unsigned int &height );

        void addChar( char c );
        void removeChar();

        unsigned int getHorizontalAdvance(); // in pixels
        const Utf8String &getString() const; 

    private:
        FT_Face _face;
        Utf8String _utf8String;
        std::vector<unsigned int> _gi;
        std::string _font;
        unsigned char _color[4];
        int _pixelSize;
        int _px, _py;
        static int  _default_font_size;
        static unsigned char _default_font[];


        unsigned int _handle;
        void _init();

};


#endif
