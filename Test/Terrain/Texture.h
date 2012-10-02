#ifndef CHASKII_DRAW_TEXTURE_DEF
#define CHASKII_DRAW_TEXTURE_DEF

#include <Chaskii/Draw/DrawableAttribute.h>
#include <Chaskii/DB/Image.h>
#include <Chaskii/Mem/ptr.h>

namespace iiDraw {


class Texture: public DrawableAttribute
{
    public:

        Texture( iiDB::Image_ptr image, GLenum texunit=GL_TEXTURE0 ):
           DrawableAttribute( DrawableAttribute::Texture_t ),
           m_image(image),
           m_id(0),
           m_texunit(texunit) 
        {}

        virtual void apply()
        {
            if( m_id == 0 )
                init();

            glActiveTexture( m_texunit );
            glBindTexture( GL_TEXTURE_2D, m_id );
        }

    private:
        iiDB::Image_ptr m_image;
        GLuint m_id;
        GLenum m_texunit;

        void init()
        {
            if( m_id != 0 )
                return;

            glGenTextures( 1, &m_id );
            glBindTexture(GL_TEXTURE_2D, m_id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D( GL_TEXTURE_2D, 0, 
                    m_image->glformat, 
                    m_image->width, 
                    m_image->height, 
                    0, 
                    m_image->glformat,  
                    m_image->gltype, 
                    m_image->data.get() );
            glGenerateMipmap( GL_TEXTURE_2D );
            glBindTexture(GL_TEXTURE_2D, 0); 
        }

};

typedef iiMem::ptr<Texture>::shared_ptr Texture_ptr;

}

#endif
