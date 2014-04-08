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
#include <jag/base/types.h>
#include <jag/base/Profile.h>
#include <jag/mx/MxCore.h>
#include <jag/mx/MxUtils.h>

#include <demoSupport/qtGlWidget.h>
#include <QApplication>
#include <QGLFormat>
#include <QCoreApplication>
#include <QKeyEvent>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/foreach.hpp>

#include <math.h>


using namespace std;
namespace bpo = boost::program_options;


DemoInterface* di( NULL );

int _lastX, _lastY;
float _lastNX, _lastNY;
jag::base::UIntVec _width, _height;
static bool _leftDrag( false ), _middleDrag( false ), _rightDrag( false );


GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{
}

void GLWidget::initializeGL()
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = reinterpret_cast< const jag::draw::platformContextID >( context() );
    jag::draw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();

    di->init();
}

void GLWidget::paintGL()
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = reinterpret_cast< const jag::draw::platformContextID >( context() );
    jag::draw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    cs->setActiveContext( contextID );
    di->frame();

#ifdef JAG3D_ENABLE_PROFILING
    jagBase::ProfileManager::instance()->dumpAll();
    jagBase::ProfileManager::instance()->reset();
#endif

    if( di->getContinuousRedraw() )
        update( rect() );
}

void GLWidget::resizeGL( int w, int h )
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = reinterpret_cast< const jag::draw::platformContextID >( context() );
    jag::draw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    cs->setActiveContext( contextID );

    _width[ contextID ] = w;
    _height[ contextID ] = h;

    glViewport( 0, 0, w, h );
    di->getCollectionVisitor().setViewport( 0, 0, w, h );
    di->reshape( w, h );
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
    case Qt::Key_Escape:
    case 'q':
        di->shutdown();
        delete di;
        QCoreApplication::instance()->quit();
        break;

    default:
        QGLWidget::keyPressEvent( e );
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

void GLWidget::mousePressEvent( QMouseEvent* e )
{
    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = reinterpret_cast< const jag::draw::platformContextID >( context() );
    jag::draw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    const int width( _width[ contextID ] );
    const int height( _height[ contextID ] );

    normXY( _lastNX, _lastNY, e->pos().x(), e->pos().y(), width, height );
    _lastX = e->pos().x();
    _lastY = e->pos().y();

    if( e->button() == Qt::LeftButton )
        _leftDrag = true;
    if( e->button() == Qt::MiddleButton )
        _middleDrag = true;
    if( e->button() == Qt::RightButton )
        _rightDrag = true;
}
void GLWidget::mouseMoveEvent( QMouseEvent* e )
{
    if( !_leftDrag && !_middleDrag && !_rightDrag )
        return;

    jag::draw::ContextSupport* cs( jag::draw::ContextSupport::instance() );
    const jag::draw::platformContextID pCtxId = reinterpret_cast< const jag::draw::platformContextID >( context() );
    jag::draw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

    jag::mx::MxCorePtr mxCore( di->getMxCore( contextID ) );
    if( mxCore == NULL )
        return;

    const int width( _width[ contextID ] );
    const int height( _height[ contextID ] );

    float nx, ny;
    normXY( nx, ny, e->pos().x(), e->pos().y(), width, height );
    const float deltaX( nx - _lastNX );
    const float deltaY( ny - _lastNY );

    if( _middleDrag || ( _leftDrag && _rightDrag ) )
    {
        if( ( deltaX == 0.f ) && ( deltaY == 0.f ) )
            // Then we're really not dragging...
            return;

        gmtl::Planed panPlane( -( mxCore->getDir() ), 0. );
        gmtl::Vec3d panDelta( jag::mx::pan( mxCore.get(), panPlane, deltaX, deltaY ) );
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
        jag::mx::computeTrackball( angle, axis,
            gmtl::Vec2d( _lastNX, _lastNY ), gmtl::Vec2d( deltaX, deltaY ),
            mxCore->getOrientationMatrix() );

        mxCore->rotateOrbit( angle, axis );
    }

    _lastNX = nx;
    _lastNY = ny;

    update( rect() );
    // Need a way to update all windows, like GLUT does:
    /*
    if( di->mouseMotionRedrawsAllWindows() )
        for( unsigned int idx=0; idx<_nWin; ++idx )
            glutPostWindowRedisplay( idx+1 );
    else
        glutPostRedisplay();
        */
}
void GLWidget::mouseReleaseEvent( QMouseEvent* e )
{
    if( e->button() == Qt::LeftButton )
        _leftDrag = false;
    if( e->button() == Qt::MiddleButton )
        _middleDrag = false;
    if( e->button() == Qt::RightButton )
        _rightDrag = false;
}



// Support for linking statically
JAG3D_REFERENCE_PLUGIN( osgImage );
JAG3D_REFERENCE_PLUGIN( osgModel );
JAG3D_REFERENCE_PLUGIN( shader );
JAG3D_REFERENCE_PLUGIN( text );



int main( int argc, char** argv )
{
    std::vector< int > winsize;
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "help,h", "Help text" )
        ( "version,v", bpo::value< double  >(), "OpenGL context version. Default: 4.0." )
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
    _width.resize( nwin );
    _height.resize( nwin );

    if( winsize.size() != 2 )
        winsize = di->defaultWinSize();

    QGL::FormatOption multisampleFlag( QGL::SampleBuffers );
    if( ( vm.count( "no-ms" ) > 0 ) ||
        !( di->defaultMultisample() ) )
    {
        multisampleFlag = QGL::NoSampleBuffers;
    }


    QApplication app( argc, argv );

    QGLFormat glFormat( QGL::DoubleBuffer | QGL::Rgba | QGL::DepthBuffer | multisampleFlag );
    if( version >= 3.0 )
    {
        glFormat.setVersion( int( versionMajor ), int( versionMinor ) );
        //if( version >= 3.1 )
            // Qt doesn't appear to allow setting the forward compatible flag.
            // Do nothing in this case.
        if( version >= 3.2 )
            glFormat.setProfile( QGLFormat::CoreProfile );
    }

    std::vector< GLWidget* > _widgetVec;
    for( unsigned int idx=0; idx< nwin; ++idx )
    {
        GLWidget* w = new GLWidget( glFormat );
        _widgetVec.push_back( w );
        w->resize( winsize[ 0 ], winsize[ 1 ] );
    }
    if( !( di->startup( nwin ) ) )
        return( 1 );
    for( unsigned int idx=0; idx< nwin; ++idx )
        _widgetVec[ idx ]->show();

    return( app.exec() );

    BOOST_FOREACH( std::vector< GLWidget* >::value_type w, _widgetVec )
        delete w;
}
