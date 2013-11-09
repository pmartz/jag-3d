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
    jagBase::ProfileManager::instance()->reset();
#endif
}



int main( int argc, char* argv[] )
{
    bpo::options_description desc( "Options" );
    // Add test/demo options
    desc.add_options()
        ( "help,h", "Help text" )
#if 0
        // Not supported for VRJ; taken from config files.
        ( "version,v", bpo::value< double >(), "OpenGL context version. Default: 4.0." )
        ( "nwin", bpo::value< int >(), "Number of windows. Default: 1." )
        ( "winsize,w", bpo::value< std::vector< int > >( &winsize )->multitoken(), "Window width and height. Default: 300 300." )
        ( "no-ms", "Disable multisampling" )
#endif
        ( "jconf,j", bpo::value< std::string >(), "VRJ configuration file" )
        ;

    DemoInterface* di( DemoInterface::create( desc ) );

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

    if( !( di->parseOptions( vm ) ) ||
        ( vm.count( "help" ) > 0 ) )
    {
        std::cout << desc << std::endl;
        return( 1 );
    }

    Kernel* kernel = Kernel::instance();
    JagDemoApp* application = new JagDemoApp( di );

    kernel->init( argc, argv );

    // Load any config files specified on the command line
    if( vm.count( "jconf" ) > 0 )
    {
        std::string jconfName = vm[ "jconf" ].as< std::string >();
        kernel->loadConfigFile( jconfName );
    }
    else
    {
        std::cerr << "'--jconf <jconfName>' is required." << std::endl;
        std::cout << desc << std::endl;
        return( 1 );
    }

    kernel->start();
    kernel->setApplication( application );
    kernel->waitForKernelStop();

    di->shutdown();
    delete( di );
    delete application;

    return( EXIT_SUCCESS );
}
