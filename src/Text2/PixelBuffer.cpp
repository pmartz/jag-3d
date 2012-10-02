#include <Chaskii/Text2/PixelBuffer.h>


PixelBuffer::PixelBuffer( ):
    _format(RGBA),
    _width(0),
    _height(0)
{
    _setNumComponents();
}

PixelBuffer::PixelBuffer( Format format ):
    _format(format),
    _width(0),
    _height(0)
{
    _setNumComponents();
}

PixelBuffer::PixelBuffer( Format format, unsigned int width, unsigned int height ):
    _format(format)
{
    _setNumComponents();
    resize( width, height);
}

void PixelBuffer::resize( int width, int height )
{
    if( (width * height * _ncomp) != _bufsize )
    {
        _width = width;
        _height = height;
        _bufsize = _width * _height * _ncomp;
        _buff = new unsigned char[_bufsize];
        clear();
    }
}

void PixelBuffer::fill(
        unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a )
{
    unsigned char *ptr = _buff.get();
    for( unsigned int i = 0; i < _width * _height; i++ )
    {
        *(ptr++) = r;
        if( _ncomp > 1 )
            *(ptr++) = g;
        if( _ncomp > 2 )
            *(ptr++) = b;
        if( _ncomp > 3 )
            *(ptr++) = a;
    }
}


void PixelBuffer::clear()
{
    memset( _buff.get(), 0, sizeof(unsigned char) * _width * _height * _ncomp );
}

unsigned char *PixelBuffer::ptr() const { return _buff.get(); }

unsigned char *PixelBuffer::operator[](size_t index)
{
    return &(_buff.get())[index*_width];
}

