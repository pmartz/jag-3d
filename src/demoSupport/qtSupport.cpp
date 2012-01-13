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
#include <jagDraw/init.h>

#include <demoSupport/qtGlWidget.h>
#include <QApplication>
#include <QGLFormat>
#include <QCoreApplication>
#include <QKeyEvent>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

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
    jagDraw::init();

    di->init();
}

void GLWidget::paintGL()
{
    di->frame();
}

void GLWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
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



int main( int argc, char* argv[] )
{
    bpo::options_description desc( "Options" );
    // Add qt test/demo options
    desc.add_options()
        ( "version", bpo::value< double  >(), "OpenGL context version. Default: 3.1." );

    // Create test/demo-specific DemoInterface, and allow it to
    // add test/demo-specific options.
    di = DemoInterface::create( desc );

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

    double version( 3.1 );
    if( vm.count( "version" ) > 0 )
        version = vm[ "version" ].as< double >();
    double versionMajor;
    modf( version, &versionMajor );
    float versionMinor = (float)( version * 10. - versionMajor * 10. );



    QApplication app( argc, argv );

    QGLFormat glFormat( QGL::DoubleBuffer | QGL::Rgba );
    if( version >= 3.0 )
    {
        glFormat.setVersion( int( versionMajor ), int( versionMinor ) );
        if( version >= 3.2 )
            glFormat.setProfile( QGLFormat::CoreProfile );
        //else
            // Qt doesn't appear to allow setting the forward compatible flag.
            // Do nothing in this case.
    }

    GLWidget widget( glFormat );
    widget.show();

    return( app.exec() );
}
