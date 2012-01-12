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
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/Error.h>
#include <jagBase/Version.h>

#include <demoSupport/qtGlWidget.h>
#include <QApplication>
#include <QGLFormat>
#include <QCoreApplication>
#include <QKeyEvent>

#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;


DemoInterface* di( NULL );


GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{
}

void GLWidget::initializeGL()
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
    int idx;
    bool no3( false );
    for( idx=1; idx<argc; idx++ )
    {
        no3 = string( argv[ idx ] ) == string( "--no3" );
        if( no3 ) break;
    }

    QApplication app( argc, argv );

    QGLFormat glFormat( QGL::DoubleBuffer | QGL::Rgba );
    if( !no3 )
    {
        glFormat.setVersion( 3, 1 );
        glFormat.setProfile( QGLFormat::CoreProfile );
    }

    GLWidget widget( glFormat );
    widget.show();

    return( app.exec() );
}
