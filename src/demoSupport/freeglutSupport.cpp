/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <demoSupport/DemoInterface.h>
#include <jag/draw/ContextSupport.h>
#include <jag/base/Profile.h>
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
unsigned int _nWin;

static jagMx::MxGamePadPtr gamePad( jagMx::MxGamePadPtr( (jagMx::MxGamePad*)NULL ) );


void init()
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = static_cast< GLuint >( glutGetWindow() );
    jag::draw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();

    di->init();
}

void display()
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = static_cast< GLuint >( glutGetWindow() );
    jag::draw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    cs->setActiveContext( contextID );
    di->frame();

    {
        JAG3D_PROFILE( "swap" );
        glutSwapBuffers();
    }

#ifdef JAG3D_ENABLE_PROFILING
    jag::base::ProfileManager::instance()->dumpAll();
    jag::base::ProfileManager::instance()->reset();
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

    if( di->mouseMotionRedrawsAllWindows() )
        for( unsigned int idx=0; idx<_nWin; ++idx )
            glutPostWindowRedisplay( idx+1 );
    else
        glutPostRedisplay();
}


int main( int argc, char* argv[] )
{
    DemoInterface::WinSizeType winsize;
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

    double version( di->defaultOpenGLVersion() );
    if( vm.count( "version" ) > 0 )
        version = vm[ "version" ].as< double >();
    double versionMajor;
    modf( version, &versionMajor );
    float versionMinor = (float)( version * 10. - versionMajor * 10. );

    int nwin( di->defaultNumWindows() );
    if( vm.count( "nwin" ) > 0 )
        nwin = vm[ "nwin" ].as< int >();
    _nWin = nwin;

    if( winsize.size() != 2 )
        winsize = di->defaultWinSize();

    unsigned int multisampleFlag( GLUT_MULTISAMPLE );
    if( ( vm.count( "no-ms" ) > 0 ) ||
        !( di->defaultMultisample() ) )
    {
        multisampleFlag = 0;
    }


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

    if( !( di->startup( jag::draw::ContextSupport::instance()->getNumRegisteredContexts() ) ) )
        return( 1 );

    glutMainLoop();

    return( 0 );
}
