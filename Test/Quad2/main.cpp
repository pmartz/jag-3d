#include <stdio.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/OpenGL.h>

#include <Chaskii/WindowSystem/WS.h>
#include <Chaskii/WindowSystem/OpenGLWindow.h>
#include <Chaskii/WindowSystem/EventResponder.h>
#include <Chaskii/WindowSystem/Keychar.h>

#include "Chaskii/Draw/ShaderProgram.h"
#include "Chaskii/Draw/VertexAttribute.h"
#include "Chaskii/Draw/VertexIterator.h"
#include "Chaskii/Draw/Drawable.h"

class BooleanToggle: public iiWS::EventResponder
{
    public:
        BooleanToggle( iiWS::KeyCharacter key, bool &flag ):
            iiWS::EventResponder(true),                 // Calling the super-class constructor with auto_register
                                                        // set to true, automatically registers the event responder
            m_key(key),
            m_flag(flag)
        {}

        virtual ~BooleanToggle() { puts("BooleanToggle DESTRUCTOR"); }

        virtual bool keyPress( iiWS::KeyCharacter k, bool isRepeat ) 
        {
            if( k == m_key )
                m_flag = !m_flag;

            return false;
        }
    protected:
        iiWS::KeyCharacter m_key;
        bool &m_flag;
};

typedef ptr<BooleanToggle>::shared_ptr BooleanToggle_ptr;


GLuint makeVBO()
{
    float buffer[16];
    buffer[0] = -1.0f;    // x0
    buffer[1] = -1.0f;    // y0
    buffer[2] = 0.0f;     // s0
    buffer[3] = 0.0f;     // t0

    buffer[4] =  1.0;     // x1
    buffer[5] = -1.0f;    // y1
    buffer[6] = 1.0f;     // s1
    buffer[7] = 0.0f;     // s1

    buffer[ 8] = -1.0f;   // x2
    buffer[ 9] =  1.0;    // y2
    buffer[10] = 0.0f;    // s2
    buffer[11] = 1.0f;    // s2

    buffer[12] = 1.0;      // x2
    buffer[13] = 1.0;      // y2
    buffer[14] = 1.0f;     // s2
    buffer[15] = 1.0f;     // s2

    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float[16]), (GLvoid *)buffer, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return vbo;
}


iiDraw::ShaderProgram *initShader()
{
    iiDraw::ShaderProgram *shader = new iiDraw::ShaderProgram;
    iiDraw::Shader *vshader = new iiDraw::Shader(GL_VERTEX_SHADER );
    vshader->addSourceFile( "px_vtx.glsl" );
    shader->attachShader( vshader );

    iiDraw::Shader *fshader = new iiDraw::Shader(GL_FRAGMENT_SHADER );
    fshader->addSourceFile( "px_frg.glsl" );
    shader->attachShader( fshader );

    return shader;
}

GLuint initTexture( )
{
    unsigned char *data = new unsigned char[256*256*3];
    unsigned char *ptr = data;
    bool flag = false;
    for( int i = 0; i < 256; i++ )
    {
        if( !(i%16) ) flag = !flag;
        for( int j = 0; j < 256; j++ )
        {
            if( !(j%16)) flag = !flag;
            if( flag )
            {
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
            }
            else
            {
                *(ptr++) = 0xFF;
                *(ptr++) = 0x00;
                *(ptr++) = 0x00;
            }
        }
    }

    GLuint tex;
    glGenTextures( 1, &tex );
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB,  GL_UNSIGNED_BYTE, data );
    glBindTexture(GL_TEXTURE_2D, 0); 

    return  tex;
}

void makeOrtho( float l, float r, float b, float t, float *p )
{
    float n = -1.0;
    float f = 1.0;

    p[0] = 2.0f/(r-l);
    p[1] = 0.0f;
    p[2] = 0.0f;
    p[3] = 0.0f;

    p[4] = 0.0f;
    p[5] = 2.0f/(t-b);
    p[6] = 0.0f;
    p[7] = 0.0f;

    p[ 8] = 0.0f;
    p[ 9] = 0.0f;
    p[10] = -2.0f/(f - n);
    p[11] = 0.0f;

    p[12] = -(r + l)/(r - l);
    p[13] = -(t + b)/(t - b);

    p[14] = -(n + f)/(f - n);
    p[15] = 1.0f;
}


int main( int argc, char **argv )
{
    iiWS::OpenGLWindow *win = new iiWS::OpenGLWindow;
    win->setPositionAndSize( 0, 0, 1024, 600 );
    win->realize();

    iiWS::WS::instance()->setBlockOnEvents( true );

    bool done = false;

    BooleanToggle_ptr bt( new BooleanToggle( iiWS::KeyChar_Escape, done ) );

    glClearColor( 0.15, 0.23, 0.15, 1.0 );

    iiDraw::ShaderProgram_ptr shader( initShader() );
    GLuint vbo = makeVBO();

    GLuint tex = initTexture();

    shader->apply();

    GLuint vloc = shader->getAttribLocation("vtxpos");
    GLuint tloc = shader->getAttribLocation("vtxtex");
    GLuint texunit0 = shader->getUniformLocation("texunit0");

    GLint omatLoc = shader->getUniformLocation( "omat" );

    float omat[16];
    makeOrtho( -2.0, 2.0, -2.0, 2.0, omat );
    glUniformMatrix4fv( omatLoc, 1, GL_FALSE, omat );

    glUniform1i( texunit0, 0 );

    glBindTexture( GL_TEXTURE_2D, tex );

    static const unsigned int stride = sizeof(float[4]);
    static const unsigned int offset = sizeof(float[2]);

    iiDraw::Drawable dbl;
    dbl.va.push_back( iiDraw::VertexAttribute_ptr(new iiDraw::VertexAttribute( vloc, 2, GL_FLOAT, GL_FALSE, stride, 0)) );
    dbl.va.push_back( iiDraw::VertexAttribute_ptr(new iiDraw::VertexAttribute( tloc, 2, GL_FLOAT, GL_FALSE, stride, offset)) );
    dbl.vi.push_back( iiDraw::VertexIterator_ptr( new iiDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 )));

    while( !done )
    {
        iiWS::WS::instance()->frame();

        win->makeCurrent();
        glClear( GL_COLOR_BUFFER_BIT );

        shader->apply();

        glBindBuffer( GL_ARRAY_BUFFER, vbo );

        dbl.apply();

        win->swapBuffers();
    }

    return 0;
}
