// Copyright

#include <jagBase/PlatformOpenGL.h>
#include <jagBase/Version.h>
#include <jagDraw/BufferObject.h>
#include <platformFreeglut.h>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;


// GLobal
jagDraw::BufferObjectPtr bop;


void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT );
    bop->apply();
    glFlush ();
}


void reshape (int w, int h)
{
    glViewport( 0, 0, w, h );
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
        case 'q':
            exit( 0 );
        break;
    }
}

void init()
{
    cout << jagBase::getVersionString() << endl;

#ifdef GLEW_MX
    glewContextInit(glewGetContext());
#ifdef _WIN32
    wglewContextInit(wglewGetContext());
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    glxewContextInit(glxewGetContext());
#endif

#else
    glewInit();
#endif


    // Display information on the type of vontext we created.
    cout << string( "GL_VENDOR: " ) << string( (char*)(glGetString( GL_VENDOR )) ) << endl;;
    cout << string( "GL_RENDERER: " ) << string( (char*)(glGetString( GL_RENDERER )) ) << endl;
    cout << string( "GL_VERSION: " ) << string( (char*)(glGetString( GL_VERSION )) ) << endl;
    cout << string( "GL_SHADING_LANGUAGE_VERSION: " ) << string( (char*)(glGetString( GL_SHADING_LANGUAGE_VERSION )) ) << endl;

    glClearColor( 0.5f, 0.5f, 0.5f, 0.f );

    float verts[] = {
        -1., -1., 0.,
        1., -1., 0.,
        -1., 1., 0.,
        1., 1., 0. };
    jagBase::BufferPtr bp( new jagBase::Buffer( sizeof( verts ), (void*)verts ) );
    bop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( jagDraw::BufferObject::ArrayBuffer, bp, jagDraw::BufferObject::StaticDraw ) );
}




int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitContextVersion( 3, 0 );
    glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

    glutInitWindowSize( 300, 300 ); 
    glutCreateWindow( "Hello OpenGL 3.x" );

    init();

    glutDisplayFunc( display ); 
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();

    return( 0 );
}
