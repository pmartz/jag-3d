// Copyright

#include <stdlib.h>
#include <GL/glew.h>
#include <platformFreeglut.h>
#include <string>
#include <iostream>
#include <utilities/glutil.h>
#include <utilities/shader.h>

using namespace std;

// Globals

// Interleaved rgb color and xy vertex data for triangle.
const GLfloat varray[] = {
    -1.f, -1.f,
    1.f, 0.f, 0.f,
    1.f, -1.f,
    0.f, 1.f, 0.f,
    0.f, 1.f,
    0.f, 0.f, 1.f
};

// Buffer object ID for color and vertex data.
GLuint bufferID;




const GLvoid *bufferObjectPtr (GLsizei index)
{
    // TBD - do we need this or is this a holdover from overloading OGL1.5 DrawArrays when VBOs were added?
    return (const GLvoid *) (((char *) NULL) + index);
}


void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glFlush ();

    GL_CHECK_ERROR( "display" );
}


void reshape (int w, int h)
{
    glViewport( 0, 0, w, h );

    GL_CHECK_ERROR( "reshape" );
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
    // initialize GLEW before calling anything else.
    GL_CHECK_ERROR( "Pre-glewInit()" );
    glewInit();
    // Look for an error from GLEW.
    GL_CHECK_ERROR( "Post-glewInit()" );


    // Display information on the type of vontext we created.
    cout << string( "GL_VENDOR: " ) << string( (char*)(glGetString( GL_VENDOR )) ) << endl;;
    cout << string( "GL_RENDERER: " ) << string( (char*)(glGetString( GL_RENDERER )) ) << endl;
    cout << string( "GL_VERSION: " ) << string( (char*)(glGetString( GL_VERSION )) ) << endl;
    cout << string( "GL_SHADING_LANGUAGE_VERSION: " ) << string( (char*)(glGetString( GL_SHADING_LANGUAGE_VERSION )) ) << endl;

    GLint maxAttribs( 0 );
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxAttribs );
    cout << string( "GL_MAX_VERTEX_ATTRIBS: " ) << maxAttribs << endl;


    // OpenGL state init
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    //
    // Test 3.0 forward compatible spec compliance.
    //
    // Test 1: glMatrixMode is deprecated, this should generate an error.
    glMatrixMode( GL_MODELVIEW );
    GL_CHECK_ERROR( "Using deprecated glMatrixMode generates" );

#ifdef _DEBUG
    // Test 2: glShadeModel was not deprecated in the "final" 3.0 spec (August 2008), but
    // this was a mistake and a Nov 2008 spec revision caught the problem and deprecated it.
    // It should be gone in 3.1.
    {
        glShadeModel( GL_SMOOTH );
        GLenum err = GL_CHECK_ERROR( "Testing glShadeModel" );
        if( err == GL_NO_ERROR )
            cerr << "glShadeModel does not generate an error." << endl;
    }

    // Test 3: Calling glVertexAttribPointer with no buffer object bound
    // should generate GL_INVALID_OPERATION.
    {
        GLfloat v[] = { 0., 0., 0. };
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, v );
        GLenum err = GL_CHECK_ERROR( "Testing glVertexAttribPointer" );
        if( err != GL_INVALID_OPERATION )
            cerr << "Error " << std::hex << err << " should be GL_INVALID_OPERATION." << endl;
    }
#endif

    // Allocate buffer objects and store data.
    glGenBuffers( 1, &bufferID );
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
    glBufferData( GL_ARRAY_BUFFER, sizeof(varray), varray, GL_STATIC_DRAW );


    // Create shaders.
    GLuint vertexShader = shader::compile( GL_VERTEX_SHADER, "simple3x.vs" );
    GLuint fragmentShader = shader::compile( GL_FRAGMENT_SHADER, "simple3x.fs" );
    GLObjectVector objs;
    objs.push_back( vertexShader );
    objs.push_back( fragmentShader );
    GLuint program = program::link( objs );
    glUseProgram( program );

    GLint gl3color = glGetAttribLocation(program, "gl3color");
    glEnableVertexAttribArray( gl3color );

    GLint gl3vertex = glGetAttribLocation(program, "gl3vertex");
    glEnableVertexAttribArray( gl3vertex );

    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
    const int stride( sizeof( GLfloat ) * 5 );
    glVertexAttribPointer( gl3color, 3, GL_FLOAT, GL_FALSE,
        stride, bufferObjectPtr( sizeof( GLfloat ) * 2 ) );
    glVertexAttribPointer( gl3vertex, 2, GL_FLOAT, GL_FALSE,
        stride, bufferObjectPtr( 0 ) );

    GL_CHECK_ERROR( "init()" );
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
// begin THREE
    glutInitContextVersion( 3, 0 );
// end THREE
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
