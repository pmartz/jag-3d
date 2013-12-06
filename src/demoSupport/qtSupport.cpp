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
#include <jagDraw/ContextSupport.h>
#include <jagBase/Profile.h>

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


GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{
}

void GLWidget::initializeGL()
{
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    const jagDraw::platformContextID pCtxId = reinterpret_cast< const jagDraw::platformContextID >( context() );
    jagDraw::jagDrawContextID contextID = cs->registerContext( pCtxId );

    cs->setActiveContext( contextID );
    cs->initContext();

    di->init();
}

void GLWidget::paintGL()
{
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    const jagDraw::platformContextID pCtxId = reinterpret_cast< const jagDraw::platformContextID >( context() );
    jagDraw::jagDrawContextID contextID = cs->getJagContextID( pCtxId );

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
