#include <stdio.h>
#include <fcntl.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/OpenGL.h>
#include <Chaskii/Draw/Drawable.h>
#include <Chaskii/Draw/ShaderProgram.h>

#include <Chaskii/WindowSystem/WS.h>
#include <Chaskii/WindowSystem/OpenGLWindow.h>
#include <Chaskii/WindowSystem/Keychar.h>

#include <Chaskii/Math/matrix.h>
#include <Chaskii/Draw/Image.h>
#include <Chaskii/Draw/Texture.h>
#include <Chaskii/DB/ImageLoader.h>

#include "Trackball.h"
#include "BooleanToggle.h"
#include "Patch.h"


/*
enum VertexAttributeBindings 
{
    VertexCoordinate,
    VertexNormal,
    TextureCoordinate0,
    TextureCoordinate1,
    TextureCoordinate2,
    TextureCoordinate3,
    TextureCoordinate4,
    TextureCoordinate5,
    TextureCoordinate6,
    TextureCoordinate7,
};
*/


#if 0
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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB,  GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, 0); 

    return  tex;
}
#endif

GLuint initHeightMap( )
{
    unsigned char *data = new unsigned char[400*400];
    int fd = open( "hm.raw", O_RDONLY );
    if( read( fd, data, 400*400) < 0 )
        perror("ead");
    close(fd);

    GLuint tex;
    glGenTextures( 1, &tex );
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, 400, 400, 0, GL_LUMINANCE,  GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, 0); 

    return  tex;
}

GLuint initNormalMap( )
{
    unsigned char *data = new unsigned char[400*400*3];
    int fd = open( "normals.raw", O_RDONLY );
    if( read( fd, data, 400*400*3) )
        perror("read");
    close(fd);

    GLuint tex;
    glGenTextures( 1, &tex );
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 400, 400, 0, GL_RGB,  GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, 0); 

    return  tex;
}



template <unsigned N>
void print_matrix( const gmtl::Matrix<float,N,N> &m )
{
    for( unsigned int i = 0; i < N; i++ )
    {
        printf("\t");
        for( unsigned int j = 0; j < N; j++ )
            printf(" %8.4f", m(i,j));
        printf("\n" );
    }
}

class StateControl: public iiWS::EventResponder
{
    public:
        StateControl():
            iiWS::EventResponder( true ),
            wireframe(false),
            cullface(true)
        {
        }


        virtual bool keyPress( iiWS::KeyCharacter key, bool isRepeat ) 
        {
            switch( key )
            {
                case iiWS::KeyChar_b:
                    cullface = !cullface;
                    if( cullface )
                        glEnable( GL_CULL_FACE );
                    else
                        glDisable( GL_CULL_FACE );
                    break;

                case iiWS::KeyChar_w:
                    wireframe = !wireframe;
                    if( wireframe )
                        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                    else
                        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    break;

                default:
                    break;

            }
            return false;
        }
    private:
        bool wireframe;
        bool cullface;
};

iiDraw::DrawableAttribute_ptr initRawTexture( const char *filename, GLenum unit, unsigned int width, unsigned int height, GLenum format )
{
    iiDB::Image_ptr image( new iiDB::Image );

    size_t size = width * height * 3;
    image->data = new GLbyte[size];

    image->width = width;
    image->height = height;
    image->depth = 0;
    image->numComponents = 3;
    image->glformat = GL_RGB;
    image->gltarget = GL_TEXTURE_2D;
    image->gltype = GL_UNSIGNED_BYTE;

    int fd = open( filename, O_RDONLY );
    read( fd, image->getData(), size );
    close(fd);

    return iiDraw::DrawableAttribute_ptr( new iiDraw::Texture( iiDB::Image_ptr(image), unit ) );
}

iiDraw::DrawableAttribute_ptr initTexture( const char *filename, GLenum unit )
{
    iiDB::Image_ptr image( iiDB::ReadImage( filename ) );
    return iiDraw::DrawableAttribute_ptr( new iiDraw::Texture( iiDB::Image_ptr(image), unit ) );
}

iiDraw::DrawableAttribute_ptr initTexture( GLenum unit )
{
    iiDB::Image_ptr image( new iiDB::Image );

    image->data = new GLbyte[256*256*3];
     GLbyte *ptr = image->data.get();
    bool flag = false;
    for( int i = 0; i < 256; i++ )
    {
        if( !(i%16) ) flag = !flag;
        for( int j = 0; j < 256; j++ )
        {
            if( !(j%16)) flag = !flag;
            if( flag )
            {
                *(ptr++) = 0x00;
                *(ptr++) = 0x00;
                *(ptr++) = 0xFF;
            }
            else
            {
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
            }
        }
    }

    image->width = 256;
    image->height = 256;
    image->depth = 0;
    image->numComponents = 3;
    image->glformat = GL_RGB;
    image->gltarget = GL_TEXTURE_2D;
    image->gltype = GL_UNSIGNED_BYTE;

    image->name = std::string("checkers");

    return iiDraw::DrawableAttribute_ptr( new iiDraw::Texture( iiDB::Image_ptr(image), unit ) );
}

int main( int argc, char **argv )
{
    iiWS::OpenGLWindow *win = new iiWS::OpenGLWindow;
    win->setPositionAndSize( 0, 0, 1024, 576 );
    win->realize();

    iiWS::WS::instance()->setBlockOnEvents( false );

    iiDraw::DrawContext context( win->getContextID() );

    bool done = false;
    BooleanToggle_ptr bt( new BooleanToggle( iiWS::KeyChar_Escape, done ) );

    new StateControl;


    iiDraw::ShaderProgram_ptr norm_shader(new iiDraw::ShaderProgram );
    iiDraw::ShaderProgram::SourceList l;
    l.add( GL_VERTEX_SHADER, "norm_vtx.glsl" );
//    l.add( GL_GEOMETRY_SHADER, "norm_geom.glsl" );
    l.add( GL_FRAGMENT_SHADER, "norm_frg.glsl" );
    norm_shader->fromSourceFileList( l );

    norm_shader->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS );
    norm_shader->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINES );
    norm_shader->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 1024 );

    norm_shader->bindAttribLocation( 0, "ii_Vertex3" );
    norm_shader->bindAttribLocation( 1, "ii_TexCoord20" );


    iiDraw::ShaderProgram_ptr shader(new iiDraw::ShaderProgram );
    //shader->fromSourceFiles( "vtx.glsl", "frg.glsl" );
    //iiDraw::ShaderProgram::SourceList l;
    l.clear();
    l.add( GL_VERTEX_SHADER, "ii_light.glsl" );
    l.add( GL_VERTEX_SHADER, "vtx.glsl" );
    l.add( GL_FRAGMENT_SHADER, "frg.glsl" );
    shader->fromSourceFileList( l );

    shader->bindAttribLocation( 0, "ii_Vertex3" );
    shader->bindAttribLocation( 1, "ii_TexCoord20" );
    shader->bindAttribLocation( 2, "ii_Normal" );

    shader->apply();

    GLint MVPloc = shader->getUniformLocation( "ii_ModelViewProjectionMatrix" );
    GLint NormalMatloc = shader->getUniformLocation( "ii_NormalMatrix" );

    shader->setUniformValue( "texunit0", iiDraw::UniformValue( 0 ));
    shader->setUniformValue( "texunit1", iiDraw::UniformValue( 1 ));
    shader->setUniformValue( "texunit2", iiDraw::UniformValue( 2 ));

    shader->setUniformValue( "ii_FrontMaterial.ambient",     iiDraw::UniformValue( iiMath::vec4f( 0.2, 0.2, 0.2, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.diffuse",     iiDraw::UniformValue( iiMath::vec4f( 0.8, 0.8, 0.8, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.specular",    iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.shininess",   iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));

    shader->setUniformValue( "ii_LightSource[0].ambient",    iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].diffuse",    iiDraw::UniformValue( iiMath::vec4f( 1.0, 1.0, 1.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].specular",   iiDraw::UniformValue( iiMath::vec4f( 1.0, 1.0, 1.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].position",   iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 1.0, 1.0 )));

    Patch_ptr patch(new Patch);

    // P = Projection Matrix, V = View Matrix, M = Model Matrix
    //iiMath::matrix4f P( iiMath::Frustumf(  -.38, .38, -.24, .24, 1., 1000.0 ) );
    iiMath::matrix4f P( iiMath::Perspective<float>(  20.0, 16.0/9.0, 0.1, 1000.0  ));
    iiMath::matrix4f V; 
    iiMath::matrix4f M; 

    patch->addAttribute( initTexture( GL_TEXTURE0 ) );
    patch->addAttribute( initTexture( "heightMap.jpg", GL_TEXTURE1 ) );
    patch->addAttribute( initRawTexture( "normals.raw", GL_TEXTURE2, 400, 400, GL_RGB ) );


    Trackball * trackball = new Trackball;
    trackball->setOrientation( Trackball::Y_UP);
    //trackball->translate( -b.center()[0], -b.center()[1], -b.center()[2] );
    trackball->setDistance( 5.0 );
    trackball->setReference();
    trackball->mapButtonState( 5, Trackball::UpdatePan );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glFrontFace( GL_CCW );
    glCullFace( GL_BACK );
    glClearColor( 0.2, 0.25, 0.2, 1.0 );

    norm_shader->apply();


    float a = 0.0;
    int frame_count = 0;
    while( !done )
    {
        iiWS::WS::instance()->frame();
        ++frame_count;

        win->makeCurrent();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        gmtl::setRot( M, gmtl::EulerAngleXYZf( a, a, 0.0f));
        V = trackball->getMatrix();
        iiMath::matrix4f VM = V * M;
        {
            iiMath::matrix4f O;
            gmtl::invertFull( O, VM );
            iiMath::matrix3f b = gmtl::makeRot< iiMath::matrix3f >( gmtl::makeRot< gmtl::EulerAngleXYZf >( O ));
            gmtl::transpose( b );
            iiDraw::UniformValue( b ).apply( NormalMatloc );
            //glUniformMatrix3fv( NormalMatloc, 1, GL_FALSE, b.getData() );
        }

        iiMath::matrix4f PVM =  P * VM;

        iiDraw::UniformValue( PVM ).apply( MVPloc);
        //glUniformMatrix4fv( MVPloc, 1, GL_FALSE, PVM.getData() );

        //a += M_PI/1800.0;;

        patch->apply( context );


        win->swapBuffers();
    }

    return 0;
}
