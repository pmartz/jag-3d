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

#include <jagDraw/Common.h>
#include <jagDraw/PerContextData.h>
#include <jagSG/Common.h>
#include <jagBase/Profile.h>
#include <jagUtil/QuadNode.h>
#include <jagUtil/DrawGraphCountVisitor.h>
#include <jagUtil/Shapes.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

#include <jagUtil/BufferAggregationVisitor.h>

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

    jagSG::NodePtr _root;

    double _moveRate;

    jagDraw::DrawGraphPtr createDrawGraph();

    jagUtil::QuadNodePtr _quadNode;
    jagDraw::FramebufferPtr _rttFBO;
    jagDraw::TexturePtr _colorBuffer;
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

jagDraw::DrawGraphPtr RTTExample::createDrawGraph()
{
    // We need 2 containers:
    //   0: Render to texture
    //   3: Display texture with full screen tri pair
    jagDraw::DrawGraphPtr drawGraphTemplate( new jagDraw::DrawGraph() );
    drawGraphTemplate->resize( 2 );

    // Element 0: Scene render to texture. Contents set by CollectionVisitor.
    // We don't need to do any setup on this NodeContainer.

    // Element 1: Full sceen tri pair to display texture.
    {
        // Use QuadNode utility to create a full screen quad in the draw graph.
        _quadNode.reset( new jagUtil::QuadNode( _colorBuffer ) );

        // Add this single draw Node to this container.
        (*drawGraphTemplate)[ 1 ].push_back( _quadNode );

        // Never reset this container.
        (*drawGraphTemplate)[ 1 ].setResetEnable( false );

        struct DepthControlCallback : public jagDraw::DrawNodeContainer::Callback
        {
            virtual bool operator()( jagDraw::DrawNodeContainer& nc, jagDraw::DrawInfo& di )
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
            jagDraw::DrawNodeContainer::CallbackPtr(
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
    jagSG::NodePtr model( DemoInterface::readSceneGraphNodeUtil( _fileName ) );
    if( model == NULL )
        return( false );

    _root.reset( new jagSG::Node );
    _root->addChild( model );


    // For gamepad speed control
    _moveRate = _root->getBound()->getRadius();

        
    // Add callbacks to control collection.
    jagSG::FrustumCullDistributionVisitor fcdv;
    model->accept( fcdv );
    jagSG::SmallFeatureDistributionVisitor sfdv;
    model->accept( sfdv );

    // Optimize VAO binding.
    jagUtil::BufferAggregationVisitor bav( _root );


    // Create CommandMap for scene rendering.
    jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagmodel.vert" ) );
    jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "jagmodel.frag" ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( _root->getOrCreateCommandMap() );
    commands->insert( prog );

    // Create a framebuffer object for RTT. After rendering to texture,
    // the _colorBuffer texture will be displayed on a full screen tri
    // pair (in the second NodeContainer of the DrawGraph).
    jagDraw::ImagePtr image( new jagDraw::Image() );
    image->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _colorBuffer.reset( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    _colorBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _colorBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;

    jagDraw::RenderbufferPtr depthBuffer;
    depthBuffer.reset( new jagDraw::Renderbuffer( GL_DEPTH_COMPONENT, _width, _height ) );

    // Create an FBO and attach the texture and renderbuffer.
    _rttFBO.reset( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _rttFBO->setViewport( 0, 0, _width, _height );
    _rttFBO->setClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _rttFBO->addAttachment( GL_COLOR_ATTACHMENT0, _colorBuffer );
    _rttFBO->addAttachment( GL_DEPTH_ATTACHMENT, depthBuffer );
    commands->insert( _rttFBO );

    // Set up lighting uniforms
    jagDraw::UniformBlockPtr lights( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "LightingLight" ) ) );
    gmtl::Vec3f dir( 0.f, 0.f, 1.f );
    gmtl::normalize( dir );
    gmtl::Point4f lightVec( dir[0], dir[1], dir[2], 0. );
    lights->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "position", lightVec ) ) );
    lights->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambient", gmtl::Point4f( 0.f, 0.f, 0.f, 1.f ) ) ) );
    lights->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuse", gmtl::Point4f( 1.f, 1.f, 1.f, 1.f ) ) ) );
    lights->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "specular", gmtl::Point4f( 1.f, 1.f, 1.f, 1.f ) ) ) );

    jagDraw::UniformBlockPtr backMaterials( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "LightingMaterialBack" ) ) );
    backMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambient", gmtl::Point4f( .1f, .1f, .1f, 1.f ) ) ) );
    backMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuse", gmtl::Point4f( .7f, .7f, .7f, 1.f ) ) ) );
    backMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "specular", gmtl::Point4f( .5f, .5f, .5f, 1.f ) ) ) );
    backMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "shininess", 16.f ) ) );

    jagDraw::UniformBlockPtr frontMaterials( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "LightingMaterialFront" ) ) );
    frontMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambient", gmtl::Point4f( .1f, .1f, .1f, 1.f ) ) ) );
    frontMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuse", gmtl::Point4f( .7f, .7f, .7f, 1.f ) ) ) );
    frontMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "specular", gmtl::Point4f( .5f, .5f, .5f, 1.f ) ) ) );
    frontMaterials->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "shininess", 16.f ) ) );

    jagDraw::UniformBlockSetPtr ubsp( jagDraw::UniformBlockSetPtr(
        new jagDraw::UniformBlockSet() ) );
    ubsp->insert( lights );
    ubsp->insert( backMaterials );
    ubsp->insert( frontMaterials );
    commands->insert( ubsp );


    // We have potentially different views per window, so we keep an MxCore
    // per context. Initialize the MxCore objects and create default views.
    const jagDraw::BoundPtr bound( _root->getBound() );
    const gmtl::Point3d pos( bound->getCenter() + gmtl::Vec3d( 0., -1., 0. ) );
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
    {
        jagMx::MxCorePtr mxCore( new jagMx::MxCore() );
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
    jagDraw::DrawGraphPtr drawGraph( createDrawGraph() );
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
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    return( true );
}


#define ENABLE_SORT

bool RTTExample::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    JAG3D_PROFILE( "frame" );

#ifdef ENABLE_SORT
    jagDraw::DrawablePrep::CommandTypeVec plist;
    plist.push_back( jagDraw::DrawablePrep::UniformBlockSet_t );
#endif

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jagMx::MxCorePtr mxCore( _mxCore._data[ contextID ] );

    jagSG::CollectionVisitor& collect( getCollectionVisitor() );
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
            jagDraw::DrawGraphPtr drawGraph( collect.getDrawGraph() );
            BOOST_FOREACH( jagDraw::DrawNodeContainer& nc, *drawGraph )
            {
                std::sort( nc.begin(), nc.end(), jagDraw::DrawNodeCommandSorter( plist ) );
            }
        }
#endif
    }

    {
        JAG3D_PROFILE( "Render" );

        // Execute the draw graph.
        jagDraw::DrawGraphPtr drawGraph( collect.getDrawGraph() );

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
        jagUtil::DrawGraphCountVisitor dgcv;
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

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    _mxCore._data[ contextID ]->setAspect( ( double ) w / ( double ) h );

    // Recreate textures, set FBO viewports, resize QuadNodes, etc.

    _quadNode->reshape( _width, _height );

    _colorBuffer->getImage()->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _colorBuffer->markAllDirty();

    jagDraw::RenderbufferPtr depthBuffer;
    depthBuffer.reset( new jagDraw::Renderbuffer( GL_DEPTH_COMPONENT, _width, _height ) );
    depthBuffer->setMaxContexts( _maxContexts );

    _rttFBO->setViewport( 0, 0, _width, _height );
    _rttFBO->addAttachment( GL_DEPTH_ATTACHMENT, depthBuffer );
}


/** \defgroup exampleJagRTT The rtt Example
This example demonstrates rendering to texture and the jagUtil::QuadNode utility.

Command line options:
\li --help,-h Help text
\li --version,-v OpenGL context version. Default: 4.0.
\li --nwin Number of windows. Default: 1.
\li --winsize,-w Window width and height. Default: 800 600.
\li --no-ms Disable multisampling
\li --file,-f Model to load.
*/
