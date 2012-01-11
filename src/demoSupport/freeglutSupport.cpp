/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

#include <DemoInterface.h>
#include <jagDraw/PlatformOpenGL.h>
#include <platformFreeglut.h>

#include <jagDraw/Error.h>
#include <jagBase/Version.h>

#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;


DemoInterface* di( NULL );

void display()
{
    di->frame();
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
            di->shutdown();
            exit( 0 );
        break;
    }
}

void init()
{
    cout << jagBase::getVersionString() << endl;

#ifdef __glew_h__
    // TBD Probably need to move this type of stuff into an init function in jagDraw.
#ifdef GLEW_MX
    glewContextInit( glewGetContext() );
#ifdef _WIN32
    wglewContextInit(wglewGetContext());
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    glxewContextInit(glxewGetContext());
#endif

#else
    glewInit();
#endif
#endif

    JAG_ERROR_CHECK( "simple3x init()" );


    di = DemoInterface::create();
    di->init();
}



int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitContextVersion( 3, 3 );
    //glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

    glutInitWindowSize( 300, 300 ); 
    glutCreateWindow( "Hello OpenGL 3.x" );

    init();

    glutDisplayFunc( display ); 
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();

    delete di;

    return( 0 );
}
