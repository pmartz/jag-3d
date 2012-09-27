#include <Chaskii/OpenGL.h>
#include <iconv.h>

#include <Chaskii/Text2/PixelBuffer.h>
#include <Chaskii/Text2/Text2D.h>
#include <Chaskii/Text2/FTLibrary.h>

Text2D::Text2D()
{
    _pixelSize = 24;
    _face = NULL;
    _utf8String.clear();
}


void Text2D::setString( const std::string &s )
{
    Utf8String ss;
    for( std::string::const_iterator p = s.begin(); p != s.end(); p++ )
        ss.push_back( *p ); 
    ss.push_back( '\0' );

    setString(ss);
}

void Text2D::setColor( float r, float g, float b, float a )
{
    _color[0] = (unsigned char)(255.0f * r);
    _color[1] = (unsigned char)(255.0f * g);
    _color[2] = (unsigned char)(255.0f * b);
    _color[3] = (unsigned char)(255.0f * a);
}

void Text2D::setPixelSize( size_t pixelSize) 
{
    _pixelSize = pixelSize;
    if( _face != NULL )
        FT_Set_Pixel_Sizes( _face, _pixelSize, _pixelSize );
}

size_t Text2D::getPixelSize()
{
    return _pixelSize;
}

unsigned int Text2D::getHorizontalAdvance() // in pixels
{
    if( _face == NULL )
        _init();

    FT_Glyph_Metrics &metrics = _face->glyph->metrics; 
    return (metrics.horiAdvance/64);
}

const Utf8String &Text2D::getString() const 
{ 
    return _utf8String; 
}

void Text2D::_init()
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

void Text2D::setFont( const std::string &font )
{
    _font = font;
}

void Text2D::setString( const Utf8String &utf8String )
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
}

void Text2D::getPixelExtents( int &x0, int &y0, int &x1, int &y1 )
{
    if( _face == NULL )
        _init();

    bool incrementLines = true;
    int nlines = 0;
    x0 = 0;
    x1 = 0;
    y0 = 0;
    y1 = 0;
    int w = 0;
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
        if( w > x1 )
            x1 = w;

        int b = -(metrics.height - metrics.horiBearingY)/64;
        int t = b + (metrics.height/64);

        if( b < y0 ) y0 = b;
        if( t > y1 ) y1 = t;
    }
}

void Text2D::getPixelDimensions( unsigned int &width, unsigned int &height )
{
    if( _face == NULL )
        _init();

    int x0, y0, x1, y1;
    getPixelExtents( x0, y0, x1, y1 );
    width = x1 - x0;
    height = y1 - y0;
}

int Text2D::getWidthInPixels( const std::string &s )
{
    if( _face == NULL )
        _init();

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

int Text2D::getWidthInPixels( )
{
    if( _face == NULL )
        return 0;

    int width = 0;
    for( unsigned int i = 0; i < _gi.size(); i++ )
    {
        FT_Load_Glyph( _face, _gi[i], 0 );
        FT_Glyph_Metrics &metrics = _face->glyph->metrics; 

        if( i > 0 ) 
        {     
            FT_Vector kd;
            FT_Get_Kerning( _face, _gi[i-1], _gi[i], FT_KERNING_DEFAULT, &kd );
            width += kd.x/64; 
        }
    
        width += (metrics.horiAdvance/64);
    }
    return width;
}


void Text2D::renderToPixelBuffer( int px, int py, PixelBuffer &pbuff )
{
    if( _face == NULL )
        _init();

    int _x = px;
    int _y = py;

    PixelBuffer::Format format = pbuff.getFormat();
    int ncomp = pbuff.getNumComponents();
    unsigned int pbuff_width = pbuff.getWidth();
    unsigned int pbuff_height = pbuff.getHeight();


    for( unsigned int i = 0; i < _gi.size()-1; i++ )
    {
        FT_Load_Glyph( _face, _gi[i], 0 );
        FT_Render_Glyph( _face->glyph, FT_RENDER_MODE_NORMAL );
        FT_Glyph_Metrics &metrics = _face->glyph->metrics; 

        int xx = _x + metrics.horiBearingX/64;
        int yy = _y - (metrics.height - metrics.horiBearingY)/64;

        for( int row = 0; row < _face->glyph->bitmap.rows ; row++ )
        {
            for( int col = 0; col < _face->glyph->bitmap.width; col++ )
            {
                unsigned int r = yy + row;
                unsigned int c = xx + col;
                if( r >= pbuff_height || c >= pbuff_width )
                    continue;

                int dindex = (r * pbuff_width + c) * ncomp;
                unsigned char *ptr = &(pbuff.ptr())[dindex];

                int sindex = (_face->glyph->bitmap.rows - 1 - row) * _face->glyph->bitmap.width + col;

                unsigned char rd, gn, bl;//, al;

                switch( format )
                {
                    case PixelBuffer::Luminance:
                        *(ptr) = _face->glyph->bitmap.buffer[sindex];
                        break;

                    case PixelBuffer::LuminanceAlpha:
                        *(ptr++) = _face->glyph->bitmap.buffer[sindex];
                        *(ptr++) = _face->glyph->bitmap.buffer[sindex];
                        break;

                    case PixelBuffer::RGB:
                        rd = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[0]);
                        gn = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[1]);
                        bl = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[2]);
                        if( rd > 0 || gn > 0 || bl > 0 )
                        {
                            ptr[0] = rd;
                            ptr[1] = gn;
                            ptr[2] = bl;
                        }
                        ptr += 3;
                        break;

                    case PixelBuffer::RGBA:
                        *(ptr++) = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[0]);
                        *(ptr++) = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[1]);
                        *(ptr++) = (unsigned char)(((float)_face->glyph->bitmap.buffer[sindex]/255.0) * (float)_color[2]);
                        *(ptr++) = _face->glyph->bitmap.buffer[sindex];
                        break;
                }
            }
        }
    
        if( i > 0 ) 
        {    
            FT_Vector kd;
            FT_Get_Kerning( _face, _gi[i-1], _gi[i], FT_KERNING_DEFAULT, &kd );
            _x += kd.x/64; 
        }
        _x += (metrics.horiAdvance/64);

    }
}


