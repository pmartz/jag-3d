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
#include <jagBase/Profile.h>

#include <demoSupport/platformFreeglut.h>

#include <Poco/Foundation.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <iostream>


using namespace std;
namespace bpo = boost::program_options;


DemoInterface* di( NULL );


void init()
{
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    const jagDraw::platformContextID pCtxId = static_cast< GLuint >( glutGetWindow() );
    jagDraw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();

    di->init();
}

void display()
{
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    const jagDraw::platformContextID pCtxId = static_cast< GLuint >( glutGetWindow() );
    jagDraw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    cs->setActiveContext( contextID );
    di->frame();
    glutSwapBuffers();

#ifdef JAG3D_ENABLE_PROFILING
    jagBase::ProfileDump dumper;
    dumper.visit( jagBase::ProfileManager::instance()->getRoot() );
    jagBase::ProfileManager::instance()->getRoot()->reset();
#endif

    if( di->getContinuousRedraw() )
        glutPostRedisplay();
}

void reshape (int w, int h)
{
    glViewport( 0, 0, w, h );
    di->reshape( w, h );
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


int main( int argc, char* argv[] )
{
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "version", bpo::value< double >(), "OpenGL context version. Default: 4.0." );
    desc.add_options()
        ( "nwin", bpo::value< int >(), "Number of windows. Default: 1." );

    // Create test/demo-specific DemoInterface, and allow it to
    // add test/demo-specific options.
    di = DemoInterface::create( desc );

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

#if( POCO_OS == POCO_OS_MAC_OS_X )
    // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
    double version( 3.2 );
#else
    double version( 4.0 );
#endif
    if( vm.count( "version" ) > 0 )
        version = vm[ "version" ].as< double >();
    double versionMajor;
    modf( version, &versionMajor );
    float versionMinor = (float)( version * 10. - versionMajor * 10. );

    int nwin( 1 );
    if( vm.count( "nwin" ) > 0 )
        nwin = vm[ "nwin" ].as< int >();


    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE );
    if( version >= 3.0 )
    {
        glutInitContextVersion( int( versionMajor ), int( versionMinor ) );
        if( version >= 3.1 )
            glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
        if( version >= 3.2 )
            glutInitContextProfile( GLUT_CORE_PROFILE );
    }

    glutInitWindowSize( 300, 300 );
    glutSetOption( GLUT_RENDERING_CONTEXT, GLUT_CREATE_NEW_CONTEXT );
    while( nwin-- )
    {
        glutCreateWindow( argv[ 0 ] );
        init();

        glutDisplayFunc( display ); 
        glutReshapeFunc( reshape );
        glutKeyboardFunc( keyboard );
    }

    if( !( di->startup( jagDraw::ContextSupport::instance()->getNumRegisteredContexts() ) ) )
        return( 1 );

    glutMainLoop();

    return( 0 );
}
