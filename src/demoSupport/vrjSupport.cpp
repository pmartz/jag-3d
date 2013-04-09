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
#include <jagBase/gmtlSupport.h>
#include <jagBase/Profile.h>
#include <jagDraw/PlatformOpenGL.h>
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

    // Set the up the viewport (since OSG clears it out)
    float vp_ox, vp_oy, vp_sx, vp_sy;   // The float vrj sizes of the view ports
    int w_ox, w_oy, w_width, w_height;  // Origin and size of the window
    user_data->getViewport()->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
    user_data->getGlWindow()->getOriginSize(w_ox, w_oy, w_width, w_height);

    // compute GLint versions of the viewport info (for passing to glViewport)
    const GLint ll_x( static_cast< GLint >( vp_ox * w_width ) );
    const GLint ll_y( static_cast< GLint >( vp_oy * w_height ) );
    const GLsizei x_size( static_cast< GLsizei >( vp_sx * w_width ) );
    const GLsizei y_size( static_cast< GLsizei >( vp_sy * w_height ) );

    glViewport( ll_x, ll_y, x_size, y_size );
    _di->getCollectionVisitor().setViewport( ll_x, ll_y, x_size, y_size );


    ProjectionPtr project( user_data->getProjection() );
    const Frustum& vrjFrustum( project->getFrustum() );
    gmtl::Matrix44d proj; gmtl::setFrustum< double >( proj,
        vrjFrustum[ Frustum::VJ_LEFT ], vrjFrustum[ Frustum::VJ_TOP ],
        vrjFrustum[ Frustum::VJ_RIGHT ], vrjFrustum[ Frustum::VJ_BOTTOM ], 
        vrjFrustum[ Frustum::VJ_NEAR ], vrjFrustum[ Frustum::VJ_FAR ] );

    // VRJ does its own drawing, so we need to tell Jag that some
    // commands have been issued. I.e., we need to "dirty state".
    jagDraw::DrawInfo& drawInfo( _di->getDrawInfo( contextID ) );
    drawInfo._current.clear( jagDraw::DrawablePrep::Program_t );
    drawInfo._current.clear( jagDraw::DrawablePrep::VertexArrayObject_t );

    gmtl::Matrix44d viewd;
    gmtl::convert( viewd, project->getViewMatrix() );
    _di->frame( viewd, proj );

#ifdef JAG3D_ENABLE_PROFILING
    jagBase::ProfileManager::instance()->dumpAll();
#endif
}



int main( int argc, char* argv[] )
{
    bpo::options_description desc( "Options" );
    // Add test/demo options
#if 0
    // Not supported for VRJ; taken from config files.
    desc.add_options()
        ( "version", bpo::value< double >(), "OpenGL context version. Default: 4.0." );
    // Not supported for VRJ; taken from config files.
    desc.add_options()
        ( "nwin", bpo::value< int >(), "Number of windows. Default: 1." );
#endif

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
