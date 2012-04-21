#include <Chaskii/DB/Image.h>
#include <memory.h>

namespace iiDB {

Image::Image():
    width(0),
    height(0),
    depth(0),
    numComponents(0),
    data(0L)
{
    compressed = false;
    glformat = GL_RGB;
    gltype = GL_UNSIGNED_BYTE;
    gltarget = GL_TEXTURE_2D;
}

Image::Image( unsigned int w,
              unsigned int h,
              unsigned int d,
              unsigned int nc,
              GLbyte *dt ):
    width(w),
    height(h),
    depth(d),
    numComponents(nc)
{
    unsigned int data_size = width * height * depth * numComponents;
    data = new GLbyte[data_size];
    if(dt != NULL)
    {
    	memcpy( data.get(), dt, data_size );
    }
    else
    {
    	memset( data.get(), 0, data_size );
    }
    compressed = false;
    glformat =  numComponents == 1 ? GL_LUMINANCE :
                numComponents == 2 ? GL_LUMINANCE_ALPHA :
                numComponents == 3 ? GL_RGB :
                numComponents == 4 ? GL_RGBA : GL_NONE;
        
    gltype = GL_UNSIGNED_BYTE;
    gltarget = GL_TEXTURE_2D;
}

Image::~Image()
{
}

bool Image::flipY()
{
    if( data == NULL )
        return false;

    unsigned int row_size = width * numComponents;
    unsigned int img_size = height * row_size;
    GLbyte *tbuf = new GLbyte [img_size];
    GLbyte *sptr = &data.get()[img_size - row_size];
    GLbyte *dptr = tbuf;

    for( unsigned int i = 0; i < height; i++ )
    {
        memcpy( dptr, sptr, row_size );
        dptr += row_size;
        sptr -= row_size;
    }

    data = tbuf;

    return true;
}


}


