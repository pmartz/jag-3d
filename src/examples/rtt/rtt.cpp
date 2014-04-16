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

#include <jag/draw/Common.h>
#include <jag/draw/PerContextData.h>
#include <jag/sg/Common.h>
#include <jag/base/Profile.h>
#include <jag/util/QuadNode.h>
#include <jag/util/DrawGraphCountVisitor.h>
#include <jag/util/Shapes.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

#include <jag/util/BufferAggregationVisitor.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <string>
#include <sstream>


using namespace std;
namespace bpo = boost::program_options;


class RTTExample : public DemoInterface
{
public:
    RTTExample()
      : DemoInterface( "jag.ex.rtt" ),
        _fileName( "cow.osg" ),
        _moveRate( 1. ),
        _width( 800 ),
        _height( 600 )
    {}
    virtual ~RTTExample() {}

    virtual bool parseOptions( boost::program_options::variables_map& vm );

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );

    // Return a value to bontrol base gamepad move rate in the scene.
    virtual double getMoveRate() const
    {
        return( _moveRate );
    }

    virtual WinSizeType defaultWinSize() const
    {
        WinSizeType ws;
        ws.push_back( 800 ); ws.push_back( 600 );
        return( ws );
    }

protected:
    std::string _fileName;

    jag::sg::NodePtr _root;

    double _moveRate;

    jag::draw::DrawGraphPtr createDrawGraph();

    jag::util::QuadNodePtr _quadNode;
    jag::draw::FramebufferPtr _rttFBO;
    jag::draw::TexturePtr _colorBuffer;
    int _width, _height;

    unsigned int _maxContexts;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    desc.add_options()
        ( "file,f", bpo::value< std::string >(), "Model to load. Default: cow.osg" );

    return( new RTTExample );
}

bool RTTExample::parseOptions( bpo::variables_map& vm )
{
    if( vm.count( "file" ) > 0 )
        _fileName = vm[ "file" ].as< std::string >();
    return( true );
}

jag::draw::DrawGraphPtr RTTExample::createDrawGraph()
{
    // We need 2 containers:
    //   0: Render to texture
    //   3: Display texture with full screen tri pair
    jag::draw::DrawGraphPtr drawGraphTemplate( new jag::draw::DrawGraph() );
    drawGraphTemplate->resize( 2 );

    // Element 0: Scene render to texture. Contents set by CollectionVisitor.
    // We don't need to do any setup on this NodeContainer.

    // Element 1: Full sceen tri pair to display texture.
    {
        // Use QuadNode utility to create a full screen quad in the draw graph.
        _quadNode.reset( new jag::util::QuadNode( _colorBuffer ) );

        // Add this single draw Node to this container.
        (*drawGraphTemplate)[ 1 ].push_back( _quadNode );

        // Never reset this container.
        (*drawGraphTemplate)[ 1 ].setResetEnable( false );

        struct DepthControlCallback : public jag::draw::DrawNodeContainer::Callback
        {
            virtual bool operator()( jag::draw::DrawNodeContainer& nc, jag::draw::DrawInfo& di )
            {
                // Disable depth test for QuadNode rendering.
                glDisable( GL_DEPTH_TEST );
                // Render the geometry.
                nc.internalExecute( di );
                // Enable depth test for next frame.
                glEnable( GL_DEPTH_TEST );
                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }
        };

        (*drawGraphTemplate)[ 1 ].getCallbacks().push_back(
            jag::draw::DrawNodeContainer::CallbackPtr(
                new DepthControlCallback() ) );
    }

    return( drawGraphTemplate );
}

bool RTTExample::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    JAG3D_INFO_STATIC( _logName, _fileName );

    _maxContexts = numContexts;

    if( _fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--file <fileName>' on command line." );
        return( false );
    }


    // Prepare the scene graph.
    jag::sg::NodePtr model( DemoInterface::readSceneGraphNodeUtil( _fileName ) );
    if( model == NULL )
        return( false );

    _root.reset( new jag::sg::Node );
    _root->addChild( model );


    // For gamepad speed control
    _moveRate = _root->getBound()->getRadius();

        
    // Add callbacks to control collection.
    jag::sg::FrustumCullDistributionVisitor fcdv;
    model->accept( fcdv );
    jag::sg::SmallFeatureDistributionVisitor sfdv;
    model->accept( sfdv );

    // Optimize VAO binding.
    jag::util::BufferAggregationVisitor bav( _root );


    // Create CommandMap for scene rendering.
    jag::draw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagmodel.vert" ) );
    jag::draw::ShaderPtr fs( DemoInterface::readShaderUtil( "jagmodel.frag" ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jag::draw::CommandMapPtr commands( _root->getOrCreateCommandMap() );
    commands->insert( prog );

    // Create a framebuffer object for RTT. After rendering to texture,
    // the _colorBuffer texture will be displayed on a full screen tri
    // pair (in the second NodeContainer of the DrawGraph).
    jag::draw::ImagePtr image( new jag::draw::Image() );
    image->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _colorBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _colorBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _colorBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;

    jag::draw::RenderbufferPtr depthBuffer;
    depthBuffer.reset( new jag::draw::Renderbuffer( GL_DEPTH_COMPONENT, _width, _height ) );

    // Create an FBO and attach the texture and renderbuffer.
    _rttFBO.reset( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _rttFBO->setViewport( 0, 0, _width, _height );
    _rttFBO->setClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _rttFBO->addAttachment( GL_COLOR_ATTACHMENT0, _colorBuffer );
    _rttFBO->addAttachment( GL_DEPTH_ATTACHMENT, depthBuffer );
    commands->insert( _rttFBO );

    // Set up lighting uniforms
    jag::draw::UniformBlockPtr lights( jag::draw::UniformBlockPtr(
        new jag::draw::UniformBlock( "LightingLight" ) ) );
    gmtl::Vec3f dir( 0.f, 0.f, 1.f );
    gmtl::normalize( dir );
    gmtl::Point4f lightVec( dir[0], dir[1], dir[2], 0. );
    lights->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "position", lightVec ) ) );
    lights->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ambient", gmtl::Point4f( 0.f, 0.f, 0.f, 1.f ) ) ) );
    lights->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "diffuse", gmtl::Point4f( 1.f, 1.f, 1.f, 1.f ) ) ) );
    lights->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "specular", gmtl::Point4f( 1.f, 1.f, 1.f, 1.f ) ) ) );

    jag::draw::UniformBlockPtr backMaterials( jag::draw::UniformBlockPtr(
        new jag::draw::UniformBlock( "LightingMaterialBack" ) ) );
    backMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ambient", gmtl::Point4f( .1f, .1f, .1f, 1.f ) ) ) );
    backMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "diffuse", gmtl::Point4f( .7f, .7f, .7f, 1.f ) ) ) );
    backMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "specular", gmtl::Point4f( .5f, .5f, .5f, 1.f ) ) ) );
    backMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "shininess", 16.f ) ) );

    jag::draw::UniformBlockPtr frontMaterials( jag::draw::UniformBlockPtr(
        new jag::draw::UniformBlock( "LightingMaterialFront" ) ) );
    frontMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ambient", gmtl::Point4f( .1f, .1f, .1f, 1.f ) ) ) );
    frontMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "diffuse", gmtl::Point4f( .7f, .7f, .7f, 1.f ) ) ) );
    frontMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "specular", gmtl::Point4f( .5f, .5f, .5f, 1.f ) ) ) );
    frontMaterials->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "shininess", 16.f ) ) );

    jag::draw::UniformBlockSetPtr ubsp( jag::draw::UniformBlockSetPtr(
        new jag::draw::UniformBlockSet() ) );
    ubsp->insert( lights );
    ubsp->insert( backMaterials );
    ubsp->insert( frontMaterials );
    commands->insert( ubsp );


    // We have potentially different views per window, so we keep an MxCore
    // per context. Initialize the MxCore objects and create default views.
    const jag::draw::BoundPtr bound( _root->getBound() );
    const gmtl::Point3d pos( bound->getCenter() + gmtl::Vec3d( 0., -1., 0. ) );
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
    {
        jag::mx::MxCorePtr mxCore( new jag::mx::MxCore() );
        mxCore->setAspect( 1. );
        mxCore->setFovy( 30. );
        mxCore->setPosition( pos );
        mxCore->setOrbitCenterPoint( bound->getCenter() );
        mxCore->lookAtAndFit( bound->asSphere() );
        _mxCore._data.push_back( mxCore );
    }


    // Tell scene graph how many contexts to expect.
    _root->setMaxContexts( numContexts );


    // Prepare the draw graph.
    jag::draw::DrawGraphPtr drawGraph( createDrawGraph() );
    getCollectionVisitor().setDrawGraphTemplate( drawGraph );
    // Tell draw graph how many contexts to expect.
    drawGraph->setMaxContexts( numContexts );


    return( true );
}

bool RTTExample::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    return( true );
}


#define ENABLE_SORT

bool RTTExample::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    JAG3D_PROFILE( "frame" );

#ifdef ENABLE_SORT
    jag::draw::Command::CommandTypeVec plist;
    plist.push_back( jag::draw::Command::UniformBlockSet_t );
#endif

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jag::mx::MxCorePtr mxCore( _mxCore._data[ contextID ] );

    jag::sg::CollectionVisitor& collect( getCollectionVisitor() );
    collect.reset();

    gmtl::Matrix44d viewMatrix;
    {
        JAG3D_PROFILE( "Collection" );

        // Set view and projection to define the collection frustum.
        if( view.mState != gmtl::Matrix44d::IDENTITY )
            viewMatrix = view;
        else
            viewMatrix = mxCore->getInverseMatrix();
        collect.setViewProj( viewMatrix, mxCore->computeProjection( .1, 25000. ) );

        {
            JAG3D_PROFILE( "Collection traverse" );
            // Collect a draw graph.
            _root->accept( collect );
        }
#ifdef ENABLE_SORT
        {
            JAG3D_PROFILE( "Collection sort" );
            jag::draw::DrawGraphPtr drawGraph( collect.getDrawGraph() );
            BOOST_FOREACH( jag::draw::DrawNodeContainer& nc, *drawGraph )
            {
                std::sort( nc.begin(), nc.end(), jag::draw::DrawNodeCommandSorter( plist ) );
            }
        }
#endif
    }

    {
        JAG3D_PROFILE( "Render" );

        // Execute the draw graph.
        jag::draw::DrawGraphPtr drawGraph( collect.getDrawGraph() );

        // Set view and projection to use for drawing. Create projection using
        // the computed near and far planes.
        double minNear, maxFar;
        collect.getNearFar( minNear, maxFar );
        drawGraph->setViewProj( viewMatrix, mxCore->computeProjection( minNear, maxFar ) );

        drawGraph->execute( drawInfo );
    }
#ifdef JAG3D_ENABLE_PROFILING
    {
        // If profiling, dump out draw graph info.
        jag::util::DrawGraphCountVisitor dgcv;
        dgcv.traverse( *( collect.getDrawGraph() ) );
        dgcv.dump( std::cout ); 
    }
#endif

    glFlush();

    JAG3D_ERROR_CHECK( "jagmodel display()" );

    return( true );
}

void RTTExample::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    _width = w;
    _height = h;

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    _mxCore._data[ contextID ]->setAspect( ( double ) w / ( double ) h );

    // Recreate textures, set FBO viewports, resize QuadNodes, etc.

    _quadNode->reshape( _width, _height );

    _colorBuffer->getImage()->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _colorBuffer->markAllDirty();

    jag::draw::RenderbufferPtr depthBuffer;
    depthBuffer.reset( new jag::draw::Renderbuffer( GL_DEPTH_COMPONENT, _width, _height ) );
    depthBuffer->setMaxContexts( _maxContexts );

    _rttFBO->setViewport( 0, 0, _width, _height );
    _rttFBO->addAttachment( GL_DEPTH_ATTACHMENT, depthBuffer );
}


/** \defgroup exampleJagRTT The rtt Example
This example demonstrates rendering to texture and the jag::util::QuadNode utility.

Command line options:
\li --help,-h Help text
\li --version,-v OpenGL context version. Default: 4.0.
\li --nwin Number of windows. Default: 1.
\li --winsize,-w Window width and height. Default: 800 600.
\li --no-ms Disable multisampling
\li --file,-f Model to load.
*/
