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

#include <vrj/Kernel/Kernel.h>
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenGL/DrawManager.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>


using namespace std;
using namespace vrj;
namespace bpo = boost::program_options;


class JagDemoApp : public opengl::App
{
public:
    JagDemoApp( DemoInterface* di )
      : opengl::App(),
        _di( di ),
        _numContexts( 0 ),
        _initialized( false )
    {}
    virtual ~JagDemoApp()
    {}

    virtual void init();

    virtual void bufferPreDraw();
    virtual void preFrame();
    virtual void intraFrame();
    virtual void postFrame();

    virtual void contextInit();
    virtual void draw();

protected:
    DemoInterface* _di;

    unsigned int _numContexts;
    bool _initialized;
    vpr::Mutex _guard;
};

void JagDemoApp::init()
{
    opengl::App::init();
}

void JagDemoApp::bufferPreDraw()
{
    opengl::App::bufferPreDraw();

    // VRJ seems to have depth test enabled by default.
    glClear( GL_DEPTH_BUFFER_BIT );
}
void JagDemoApp::preFrame()
{
    opengl::App::preFrame();
}
void JagDemoApp::intraFrame()
{
    opengl::App::intraFrame();
}
void JagDemoApp::postFrame()
{
    opengl::App::postFrame();
}

void JagDemoApp::contextInit()
{
    opengl::App::contextInit();

    vrj::opengl::DrawManager* gl_manager(
        dynamic_cast< vrj::opengl::DrawManager* >( getDrawManager() ) );
    const int ctx( gl_manager->getCurrentContext() );

    const jagDraw::platformContextID pCtxId( static_cast<
        const jagDraw::platformContextID >( ctx ) );
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    jagDraw::jagDrawContextID contextID( cs->registerContext( pCtxId ) );

    cs->setActiveContext( contextID );
    cs->initContext();

    {
        vpr::Guard< vpr::Mutex > guard( _guard );
        _numContexts += 1;
    }

    _di->init();
}
void JagDemoApp::draw()
{
    {
        vpr::Guard< vpr::Mutex > guard( _guard );
        if( !_initialized )
        {
            _di->startup( _numContexts );
            _initialized = true;
        }
    }

    vrj::opengl::DrawManager* gl_manager(
        dynamic_cast< vrj::opengl::DrawManager* >( getDrawManager() ) );
    vprASSERT(gl_manager != NULL);

    const int ctx( gl_manager->getCurrentContext() );

    const jagDraw::platformContextID pCtxId( static_cast<
        const jagDraw::platformContextID >( ctx ) );
    jagDraw::ContextSupport* cs( jagDraw::ContextSupport::instance() );
    jagDraw::jagDrawContextID contextID( cs->getJagContextID( pCtxId ) );

    cs->setActiveContext( contextID );


    vrj::opengl::UserData* user_data( gl_manager->currentUserData() );

    _di->frame();
}



int main( int argc, char* argv[] )
{
    bpo::options_description desc( "Options" );
    // Add freeglut test/demo options
    desc.add_options()
        ( "version", bpo::value< double >(), "OpenGL context version. Default: 3.1." );
    desc.add_options()
        ( "nwin", bpo::value< int >(), "Number of windows. Default: 1." );

    DemoInterface* di( DemoInterface::create( desc ) );

    Kernel* kernel = Kernel::instance();
    JagDemoApp* application = new JagDemoApp( di );

#if( !defined( VRJ_USE_COCOA ) )
    // IF not args passed to the program
    //    Display usage information and exit
    if (argc <= 1)
    {
        std::cout << "\n\n";
        std::cout << "Usage: " << argv[0] << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]" << std::endl;
        std::exit(EXIT_FAILURE);
    }
#endif

    kernel->init( argc, argv );

    // Load any config files specified on the command line
    for( int i = 1; i < argc; ++i )
    {
        kernel->loadConfigFile( argv[i] );
    }

    kernel->start();
    kernel->setApplication( application );
    kernel->waitForKernelStop();

    di->shutdown();
    delete( di );
    delete application;

    return( EXIT_SUCCESS );
}
