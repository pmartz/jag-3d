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
}

void GLWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
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
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "version", bpo::value< double  >(), "OpenGL context version. Default: 4.0." );
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


    QApplication app( argc, argv );

    QGLFormat glFormat( QGL::DoubleBuffer | QGL::Rgba );
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
    while( nwin-- )
    {
        GLWidget* w = new GLWidget( glFormat );
        _widgetVec.push_back( w );
        w->resize( 300, 300 );
        w->show();
    }
    if( !( di->startup( jagDraw::ContextSupport::instance()->getNumRegisteredContexts() ) ) )
        return( 1 );

    return( app.exec() );

    BOOST_FOREACH( std::vector< GLWidget* >::value_type w, _widgetVec )
        delete w;
}
