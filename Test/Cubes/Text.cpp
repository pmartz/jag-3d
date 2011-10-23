#include <Chaskii/OpenGL.h>
#include <iconv.h>
#include "Text.h"

class FTLibrary
{
    public:
        static FTLibrary *instance()
        {
            static FTLibrary *theLibrary = new FTLibrary;
            return theLibrary;
        }

        const FT_Library &get() { return _library; }


    private:
        FT_Library _library;
        FTLibrary()
        {
            FT_Init_FreeType( &_library );
        }
};

Text::Text()
{
    //_font = std::string("fonts/arial.ttf" );
    _pixelSize = 24;
    _handle = 0;
    _face = NULL;
    _utf8String.clear();
    _px = _py = 0;
}



Text::Text( const std::string &font,
    const iiMath::vec4 &color,
    int pixelSize ):
    _font(font),
    _pixelSize(pixelSize),
    _handle(0)
{
    _face = NULL;
    _utf8String.clear();
    _gi.clear();

    setColor( color );
    _px = _py = 0;
}


void Text::setPosition( int x, int y )
{
    _px = x;
    _py = y;
}

void Text::setString( const std::string &s )
{
    Utf8String ss;
    for( std::string::const_iterator p = s.begin(); p != s.end(); p++ )
        ss.push_back( *p ); 
//    ss.push_back( '\0' );

    setString(ss);
}

void Text::setColor( const iiMath::vec4 &color )
{
    setColor( color[0], color[1], color[2], color[3] );
}

void Text::setColor( float r, float g, float b, float a )
{
    _color[0] = (unsigned char)(255.0f * r);
    _color[1] = (unsigned char)(255.0f * g);
    _color[2] = (unsigned char)(255.0f * b);
    _color[3] = (unsigned char)(255.0f * a);
}

void Text::setPixelSize( size_t pixelSize) 
{
    _pixelSize = pixelSize;
    if( _face != NULL )
        FT_Set_Pixel_Sizes( _face, _pixelSize, _pixelSize );
}

unsigned int Text::getHorizontalAdvance() // in pixels
{
    if( _face == NULL )
        _init();

    FT_Glyph_Metrics &metrics = _face->glyph->metrics; 
    return (metrics.horiAdvance/64);
}

void Text::addChar( char c )
{
    _utf8String.push_back( c );
}

void Text::removeChar()
{
    if( _utf8String.size() > 0 )
        _utf8String.erase( _utf8String.end() - 1 );
}


const Utf8String &Text::getString() const 
{ 
    return _utf8String; 
}

void Text::_init()
{
    if( _face != NULL )
        return;

    const FT_Library  &_library = FTLibrary::instance()->get();

    int error = 0;
    if( _font.empty() )
    {
        error = FT_New_Memory_Face( 
                _library, 
                (FT_Byte *)_default_font,
                _default_font_size, 0, &_face );
        if( error != 0 )
        {
            fprintf(stderr, "Warning... unable to initialize default font\n" );
            _face = NULL;
        }
    }
    else
    {
        error = FT_New_Face( _library, _font.c_str(), 0, &_face );
        if( error != 0 )
        {
            fprintf(stderr, "Warning... unable to initialize font %s... attempting to use default font\n", _font.c_str() );
            error = FT_New_Memory_Face( 
                        _library, 
                        (FT_Byte *)_default_font,
                        _default_font_size, 0, &_face );
            if( error != 0 )
            {
                fprintf(stderr, "Warning... unable to initialize default font\n" );
                _face = NULL;
            }
        }
    }
    FT_Set_Pixel_Sizes( _face, _pixelSize, _pixelSize );
}

void Text::setFont( const std::string &font )
{
    _font = font;
}

void Text::setString( const Utf8String &utf8String )
{
    if( _face == NULL )
        _init();

    _utf8String = utf8String;
    unsigned short unibuf[2560];

    char *src = (char *)&utf8String.front();
    size_t inbytes_left = utf8String.size();

    char *dest = (char *)&unibuf[0];
    size_t outbytes_left = 2560;

    iconv_t ic = iconv_open( "UNICODELITTLE", "UTF-8" );
#ifdef WIN32
    iconv(ic, (const char**)(&src), &inbytes_left, &dest, &outbytes_left );
#else
    iconv(ic, &src, &inbytes_left, &dest, &outbytes_left );
#endif
    iconv_close(ic);

    int len = (2560 - outbytes_left)/sizeof(unsigned short);
    _gi.clear();
    for(  int i= 0; i < len; i++ )
    {
        unsigned int key =   FT_Get_Char_Index( _face, unibuf[i] );
        _gi.push_back( key );
    }

    /*
    if( _handle != 0 )
    {
        glDeleteLists( _handle, 1 );
        _handle = 0;
    }
    */
}

void Text::getPixelDimensions( unsigned int &width, unsigned int &height )
{
    if( _face == NULL )
        _init();

    bool incrementLines = true;
    int nlines = 0;
    width = 0;
    unsigned int w = 0;
    for( unsigned int i = 0; i < _gi.size(); i++ )
    {
        if( incrementLines )
        {
            nlines++;
            w = 0;
            incrementLines = false;
        }

        if( _gi[i] == 0 )
        {
            incrementLines = true;
            continue;
        }

        FT_Load_Glyph( _face, _gi[i], 0 );
        FT_Glyph_Metrics &metrics = _face->glyph->metrics; 

        if( i > 0 ) 
        {    
            FT_Vector kd;
            FT_Get_Kerning( _face, _gi[i-1], _gi[i], FT_KERNING_DEFAULT, &kd );
            w += kd.x/64; 
        }

        //w += (metrics.width/64);
        w += (metrics.horiAdvance/64);
        if( w > width )
        {
            width = w;
        }
    }

    height = nlines * (_face->max_advance_height/64);
}

int Text::getWidthInPixels( const std::string &s )
{
    int w = 0;
    for( unsigned int i = 0; i < s.length(); i++ )
    {
        unsigned int key =   FT_Get_Char_Index( _face, s[i] );
        FT_Load_Glyph( _face, key, 0 );
        FT_Glyph_Metrics &metrics = _face->glyph->metrics; 
        w += (metrics.width/64);
    }
    return w;
}

int Text::getWidthInPixels( )
{
    if( _face == NULL )
        return 0;

    int width = 0;
    for( unsigned int i = 0; i < _gi.size(); i++ )
    {
        FT_Load_Glyph( _face, _gi[i], 0 );
        FT_Glyph_Metrics &metrics = _face->glyph->metrics; 
        /*
        if( i > 0 ) 
        {    
            FT_Vector kd;
            FT_Get_Kerning( _face, _gi[i-1], _gi[i], FT_KERNING_DEFAULT, &kd );
            width += kd.x/64; 
        }
        */
        
        width += (metrics.horiAdvance/64);
        //width += (metrics.width/64);
    }
    return width;
}


void Text::render()
{
    if( _face == NULL )
        _init();

    /*
    if( glIsList(_handle ) == 0 )
        _handle = 0;
        */

    //if( _handle == 0 )
    {
        //_handle = glGenLists( 1 );
        //glNewList( _handle, GL_COMPILE );
        int _x = _px;
        int _y = _py;

        int spacing = 5;

        for( unsigned int i = 0; i < _gi.size(); i++ )
        {
            FT_Load_Glyph( _face, _gi[i], 0 );
            FT_Render_Glyph( _face->glyph, FT_RENDER_MODE_NORMAL );
            FT_Glyph_Metrics &metrics = _face->glyph->metrics; 

            size_t bitmap_size = (metrics.width/64) * (metrics.height/64) * 4;
            unsigned char *bitmap = new unsigned char[bitmap_size];
            

            if( _gi[i] == 0 )
            {
                _x = _px;
                _y -= ((metrics.vertAdvance/64) + spacing);
                continue;
            }

            memset( bitmap, 0, bitmap_size );

            for( int row = 0; row < _face->glyph->bitmap.rows ; row++ )
            {
                for( int col = 0; col < _face->glyph->bitmap.width; col++ )
                {
                    int index = (_face->glyph->bitmap.rows - 1 - row) * _face->glyph->bitmap.width + col;
        
                    int dindex = (row * _face->glyph->bitmap.width + col) * 4;
                    unsigned char *ptr = &bitmap[dindex];
        
                    ptr[0] = _color[0];
                    ptr[1] = _color[1];
                    ptr[2] = _color[2];
                    ptr[3] = _face->glyph->bitmap.buffer[index];
                }
            }
        
            if( i > 0 ) 
            {    
                FT_Vector kd;
                FT_Get_Kerning( _face, _gi[i-1], _gi[i], FT_KERNING_DEFAULT, &kd );
                _x += kd.x/64; 
            }

            glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
            glRasterPos2i( _x + metrics.horiBearingX/64, _y - (metrics.height - metrics.horiBearingY)/64  );
            glDrawPixels( metrics.width/64, metrics.height/64, GL_RGBA, GL_UNSIGNED_BYTE, bitmap ); 
            glPopAttrib();

            _x += (metrics.horiAdvance/64);

            delete [] bitmap;
        }
        
        //glEndList();
    }

    //glCallList(_handle);
}



