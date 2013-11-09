/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <demoSupport/DemoInterface.h>
#include <jagDraw/ContextSupport.h>
#include <jagBase/Profile.h>
#include <jagMx/MxCore.h>
#include <jagMx/MxUtils.h>
#include <jagMx/MxGamePad.h>
#ifdef DIRECTINPUT_ENABLED
#  include <jagMx/MxGamePadDX.h>
#endif

#include <demoSupport/platformFreeglut.h>

#include <Poco/Foundation.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <iostream>
#include <vector>


using namespace std;
namespace bpo = boost::program_options;


DemoInterface* di( NULL );

int _lastX, _lastY;
float _lastNX, _lastNY;
typedef std::vector< int > IntVec;
IntVec _width, _height;
static bool _leftDrag( false ), _middleDrag( false ), _rightDrag( false );

static jagMx::MxGamePadPtr gamePad( jagMx::MxGamePadPtr( (jagMx::MxGamePad*)NULL ) );


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

    {
        JAG3D_PROFILE( "swap" );
        glutSwapBuffers();
    }

#ifdef JAG3D_ENABLE_PROFILING
    jagBase::ProfileManager::instance()->dumpAll();
    jagBase::ProfileManager::instance()->reset();
#endif

    if( di->getContinuousRedraw() )
        glutPostRedisplay();
}

void reshape( int w, int h )
{
    const int window( glutGetWindow() - 1 );
    _width[ window ] = w;
    _height[ window ] = h;

    glViewport( 0, 0, w, h );
    di->getCollectionVisitor().setViewport( 0, 0, w, h );
    di->reshape( w, h );
}

void keyboard( unsigned char key, int x, int y )
{
    switch (key)
    {
    case 27:
    case 'q':
        di->shutdown();
        delete di;
        exit( 0 );
        break;
    default:
        if( di->keyCommand( (int) key ) )
            glutPostRedisplay();
        break;
    }
}

void timer( int value )
{
    // Currently we can only use window 0.
    jagMx::MxCorePtr mxCore( di->getMxCore( 0 ) );
    if( mxCore == NULL )
        return;

    gamePad->setMxCore( mxCore );

    bool redraw( false );
#ifdef DIRECTINPUT_ENABLED
    jagMx::MxGamePadDX* gp( dynamic_cast< jagMx::MxGamePadDX* >( gamePad.get() ) );
    if( gp != NULL )
    {
        // TBD really should only need to do this once.
        if( di != NULL )
            gp->setStickRate( di->getMoveRate() );
        redraw = gp->poll( 1./60. ); // TBD use a timer.
    }
#endif

    if( redraw )
    {
        glutPostRedisplay();
        glutTimerFunc( 16, timer, 0 );
    }
}

void normXY( float& normX, float& normY, const int x, const int y, const int width, const int height )
{
    // Given a width x height window, convert pixel coords x and y
    // to normalized coords in the range -1 to 1. Invert Y so that
    // -1 is at the window bottom.
    const float halfW( (float)width * .5f );
    const float halfH( (float)height * .5f );

    normX = ( (float)x - halfW ) / halfW;
    normY = -( (float)y - halfH ) / halfH;
}

void mouse( int button, int op, int x, int y )
{
    const int window( glutGetWindow() - 1 );
    const int width( _width[ window ] );
    const int height( _height[ window ] );

    normXY( _lastNX, _lastNY, x, y, width, height );
    _lastX = x;
    _lastY = y;

    if( button == GLUT_LEFT_BUTTON )
        _leftDrag = ( op == GLUT_DOWN );
    if( button == GLUT_MIDDLE_BUTTON )
        _middleDrag = ( op == GLUT_DOWN );
    if( button == GLUT_RIGHT_BUTTON )
        _rightDrag = ( op == GLUT_DOWN );
}
void motion( int x, int y )
{
    if( !_leftDrag && !_middleDrag && !_rightDrag )
        return;

    const int window( glutGetWindow() - 1 );
    jagMx::MxCorePtr mxCore( di->getMxCore( window ) );
    if( mxCore == NULL )
        return;

    const int width( _width[ window ] );
    const int height( _height[ window ] );

    float nx, ny;
    normXY( nx, ny, x, y, width, height );
    const float deltaX( nx - _lastNX );
    const float deltaY( ny - _lastNY );

    if( _middleDrag || ( _leftDrag && _rightDrag ) )
    {
        if( ( deltaX == 0.f ) && ( deltaY == 0.f ) )
            // Then we're really not dragging...
            return;

        gmtl::Planed panPlane( -( mxCore->getDir() ), 0. );
        gmtl::Vec3d panDelta( jagMx::pan( mxCore.get(), panPlane, deltaX, deltaY ) );
        mxCore->moveLiteral( -panDelta );
    }
    else if( _rightDrag )
    {
        if( deltaY == 0.f )
            // Then we're really not dragging...
            return;

        mxCore->moveOrbit( deltaY );
    }
    else if( _leftDrag )
    {
        if( ( deltaX == 0.f ) && ( deltaY == 0.f ) )
            // Then we're really not dragging...
            return;

        double angle;
        gmtl::Vec3d axis;
        jagMx::computeTrackball( angle, axis,
            gmtl::Vec2d( _lastNX, _lastNY ), gmtl::Vec2d( deltaX, deltaY ),
            mxCore->getOrientationMatrix() );

        mxCore->rotateOrbit( angle, axis );
    }

    _lastNX = nx;
    _lastNY = ny;

    glutPostRedisplay();
}


int main( int argc, char* argv[] )
{
    std::vector< int > winsize;
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "help,h", "Help text" )
        ( "version,v", bpo::value< double >(), "OpenGL context version. Default: 4.0." )
        ( "nwin", bpo::value< int >(), "Number of windows. Default: 1." )
        ( "winsize,w", bpo::value< std::vector< int > >( &winsize )->multitoken(), "Window width and height. Default: 300 300." )
        ( "no-ms", "Disable multisampling" )
        ;

    // Create test/demo-specific DemoInterface, and allow it to
    // add test/demo-specific options.
    di = DemoInterface::create( desc );

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

    if( !( di->parseOptions( vm ) ) ||
        ( vm.count( "help" ) > 0 ) )
    {
        std::cout << desc << std::endl;
        return( 1 );
    }

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

    if( winsize.size() != 2 )
    {
        winsize.clear();
        winsize.push_back( 300 ); winsize.push_back( 300 );
    }

    unsigned int multisampleFlag( GLUT_MULTISAMPLE );
    if( vm.count( "no-ms" ) > 0 )
        multisampleFlag = 0;


#ifdef DIRECTINPUT_ENABLED
    gamePad = jagMx::MxGamePadDXPtr( new jagMx::MxGamePadDX() );
#endif


    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | multisampleFlag );
    if( version >= 3.0 )
    {
        glutInitContextVersion( int( versionMajor ), int( versionMinor ) );
        if( version >= 3.1 )
            glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
        if( version >= 3.2 )
            glutInitContextProfile( GLUT_CORE_PROFILE );
    }

    glutInitWindowSize( winsize[ 0 ], winsize[ 1 ] );
    glutSetOption( GLUT_RENDERING_CONTEXT, GLUT_CREATE_NEW_CONTEXT );
    _width.resize( nwin );
    _height.resize( nwin );
    while( nwin-- )
    {
        glutCreateWindow( argv[ 0 ] );
        init();

        glutDisplayFunc( display ); 
        glutReshapeFunc( reshape );
        glutKeyboardFunc( keyboard );
        if( gamePad != NULL )
            glutTimerFunc( 16, timer, 0 );
        glutMouseFunc( mouse );
        glutMotionFunc( motion );

        _width[ nwin ] = winsize[ 0 ];
        _height[ nwin ] = winsize[ 1 ];
    }

    if( !( di->startup( jagDraw::ContextSupport::instance()->getNumRegisteredContexts() ) ) )
        return( 1 );

    glutMainLoop();

    return( 0 );
}
