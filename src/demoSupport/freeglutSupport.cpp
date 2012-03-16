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

#include <demoSupport/DemoInterface.h>
#include <jagDraw/ContextSupport.h>

#include <demoSupport/platformFreeglut.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>


using namespace std;
namespace bpo = boost::program_options;


DemoInterface* di( NULL );


void init()
{
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );

    int glutContext = glutGet( GLUT_RENDERING_CONTEXT );
    const jagDraw::platformContextID pCtxId = static_cast< GLuint >( glutContext );
    jagDraw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();

    di->init();
}

void display()
{
    di->frame();
    glutSwapBuffers();
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
        delete di;
        exit( 0 );
        break;
    }
}


int main (int argc, char** argv)
{
    bpo::options_description desc( "Options" );
    // Add freeglut test/demo options
    desc.add_options()
        ( "version", bpo::value< double >(), "OpenGL context version. Default: 3.1." );

    // Create test/demo-specific DemoInterface, and allow it to
    // add test/demo-specific options.
    di = DemoInterface::create( desc );
    di->startup();

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

    double version( 3.1 );
    if( vm.count( "version" ) > 0 )
        version = vm[ "version" ].as< double >();
    double versionMajor;
    modf( version, &versionMajor );
    float versionMinor = (float)( version * 10. - versionMajor * 10. );



    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    if( version >= 3.0 )
    {
        glutInitContextVersion( int( versionMajor ), int( versionMinor ) );
        if( version >= 3.2 )
            glutInitContextProfile( GLUT_CORE_PROFILE );
        else
            glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
    }

    glutInitWindowSize( 300, 300 ); 
    glutCreateWindow( argv[ 0 ] );

    init();

    glutDisplayFunc( display ); 
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();

    return( 0 );
}
