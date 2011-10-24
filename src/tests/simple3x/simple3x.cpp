// Copyright

#include <platformFreeglut.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

void display()
{
    cout << "In display." << std::endl;
}


void reshape (int w, int h)
{
    cout << "In reshape." << std::endl;
}


int main (int argc, char** argv)
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitContextVersion( 3, 0 );
    glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

    glutInitWindowSize( 300, 300 ); 
    glutCreateWindow( "Hello OpenGL 3.x" );

    glutDisplayFunc( display ); 
    glutReshapeFunc( reshape );

    glutMainLoop();

    return( 0 );
}
