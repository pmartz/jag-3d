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
#include <jagSG/Common.h>
#include <jagUtil/ABuffer.h>
#include <jagUtil/Blur.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/Profile.h>
#include <jagUtil/DrawGraphCountVisitor.h>
#include <jagUtil/Shapes.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

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


class ABufferJag : public DemoInterface
{
public:
    ABufferJag()
      : DemoInterface( "jag.ex.abuf" ),
        _fileName( "cow.osg" ),
        _moveRate( 1. ),
        _width( 800 ),
        _height( 600 )
    {}
    virtual ~ABufferJag() {}

    virtual bool parseOptions( boost::program_options::variables_map& vm );

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );
    virtual bool keyCommand( const int command );

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
    virtual bool defaultMultisample() const
    {
        return( false );
    }

protected:
    std::string _fileName;

    jagSG::NodePtr _root;

    double _moveRate;

    jagUtil::ABufferPtr _aBuffer;

    jagSG::NodePtr _abufNode, _abufOpaqueChild;
    jag::draw::FramebufferPtr _opaqueFBO;
    jag::draw::TexturePtr _opaqueBuffer, _secondaryBuffer, _glowBuffer, _depthBuffer;
    jagUtil::BlurPtr _blur;
    jag::draw::UniformPtr _glowColor;
    jagSG::NodeMaskCullCallbackPtr _opaqueToggleCB;

    int _width, _height;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    desc.add_options()
        ( "file,f", bpo::value< std::string >(), "Model to load. Default: cow.osg" );

    return( new ABufferJag );
}

bool ABufferJag::parseOptions( bpo::variables_map& vm )
{
    if( vm.count( "file" ) > 0 )
        _fileName = vm[ "file" ].as< std::string >();
    return( true );
}


/** \struct ForceFramebufferExecute
\brief A NodeContainer callback that forces a Framebuffer execution.
\details This is useful is the NodeContainer is empty (as a result of
nothing beind collected). If the geometry would normally execute a
Framebuffer using RTT, client code would typically want the glClear()
call to create an empty texture. This callback ensures that always happens. */
struct ForceFramebufferExecute : public jag::draw::DrawNodeContainer::Callback
{
    ForceFramebufferExecute( jag::draw::FramebufferPtr& fbo )
        : _fbo( fbo )
    {}

    virtual bool operator()( jag::draw::DrawNodeContainer& nc, jag::draw::DrawInfo& di )
    {
        const jag::draw::jagDrawContextID contextID( di._id );

        _fbo->execute( di );
        di._current.insert( _fbo );

        return( true );
    }

protected:
    jag::draw::FramebufferPtr _fbo;
};
typedef jag::base::ptr< ForceFramebufferExecute >::shared_ptr ForceFramebufferExecutePtr;


static gmtl::Point4f glowColors[] = {
    gmtl::Point4f( 0.f, 0.5f, 0.f, 1.f ),
    gmtl::Point4f( 1.f, 0.f,  0.f, 1.f ),
    gmtl::Point4f( 0.f, 0.f,  0.f, 0.f )
};
static unsigned int glowIndex( 0 );

bool ABufferJag::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    JAG3D_INFO_STATIC( _logName, _fileName );

    if( _fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--file <fileName>' on command line." );
        return( false );
    }


    // Create the texture used by _opaqueFBO to store the opaque
    // color buffer. After our opaque NodeContainer #0 renders into it, 
    // the ABuffer class's third NodeContainer uses it during the
    // abuffer resolve.
    jag::draw::ImagePtr image( new jag::draw::Image() );
    image->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _opaqueBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _opaqueBuffer->setUserDataName( "opaqueBuffer" );
    _opaqueBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _opaqueBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _opaqueBuffer->setMaxContexts( numContexts );

    // Create second color buffer for glow effect.
    _secondaryBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _secondaryBuffer->setUserDataName( "secondaryBuffer" );
    _secondaryBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _secondaryBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _secondaryBuffer->setMaxContexts( numContexts );

    // Create glow effect output buffer
    _glowBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _glowBuffer->setUserDataName( "glowBuffer" );
    _glowBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _glowBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _glowBuffer->setMaxContexts( numContexts );

    // Depth texture used as depth buffer for opaque pass.
    // Also used during abuffer render to discard occluded fragments.
    image->set( 0, GL_DEPTH_COMPONENT, _width, _height, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
    _depthBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _depthBuffer->setUserDataName( "depthBuffer" );
    _depthBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _depthBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _depthBuffer->setMaxContexts( numContexts );

    // Create a framebuffer object. The color texture will store opaque
    // rendering. The depth texture is shared with ABuffer to avoid
    // rendering behind opaque geometry.
    _opaqueFBO.reset( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _opaqueFBO->setViewport( 0, 0, _width, _height );
    _opaqueFBO->setClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _opaqueFBO->setClearColor( .15f, .1f, .5f, 0.f ); // Must clear alpha to 0 for glow.
    _opaqueFBO->addAttachment( GL_DEPTH_ATTACHMENT, _depthBuffer );
    _opaqueFBO->addAttachment( GL_COLOR_ATTACHMENT0, _opaqueBuffer );
    _opaqueFBO->addAttachment( GL_COLOR_ATTACHMENT1, _secondaryBuffer );


    // Create the ABuffer management object.
    _aBuffer.reset( new jagUtil::ABuffer( _depthBuffer, _opaqueBuffer, _glowBuffer ) );
    _aBuffer->setMaxContexts( numContexts );
    //_aBuffer->setSecondaryColorBufferEnable( false );

    // Obtain the draw graph from the ABuffer object.
    // Default behavior is that the ABuffer owns NodeContainers 1-3, and we put
    // all opaque geometry in NodeContainer 0.
    jag::draw::DrawGraphPtr drawGraph( _aBuffer->createDrawGraphTemplate( 2 ) );

    // Force Framebuffer::execute() on first NodeContainer so that we
    // get a glClear() on the FBO even when it container no jagDraw Nodes.
    ForceFramebufferExecutePtr fbocb( ForceFramebufferExecutePtr( new ForceFramebufferExecute( _opaqueFBO ) ) );
    (*drawGraph)[ 0 ].getCallbacks().push_back( fbocb );

    // Create blur effect for second NodeContainer
    _blur.reset( new jagUtil::Blur( _secondaryBuffer, _glowBuffer ) );
    _blur->setMaxContexts( numContexts );
    (*drawGraph)[ 1 ] = _blur->getNodeContainer();

    getCollectionVisitor().setDrawGraphTemplate( drawGraph );

    // Allow ABuffer object to specify which matrices it needs.
    jag::draw::TransformCallback* xformCB( getCollectionVisitor().getTransformCallback() );
    xformCB->setRequiredMatrixUniforms(
        xformCB->getRequiredMatrixUniforms() |
        _aBuffer->getRequiredMatrixUniforms() );


    // Prepare the scene graph.
    jagSG::NodePtr model( DemoInterface::readSceneGraphNodeUtil( _fileName ) );
    if( model == NULL )
        return( false );

    // Add model instance as opaque
    _root.reset( new jagSG::Node );
    jagSG::NodePtr opaqueNode( jagSG::NodePtr( new jagSG::Node() ) );
    opaqueNode->addChild( model );
    _root->addChild( opaqueNode );
    {
        // Add glow uniform to opaquwNode.
        _glowColor.reset( new jag::draw::Uniform( "glowColor", glowColors[ glowIndex ] ) );
        jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
        usp->insert( _glowColor );
        opaqueNode->getOrCreateCommandMap()->insert( usp );

        // Allow opaqueNode to be toggled/masked.
        _opaqueToggleCB.reset( new jagSG::NodeMaskCullCallback() );
        _opaqueToggleCB->setOverride( jagSG::NodeMaskCullCallback::OVERRIDE_TRUE );
        opaqueNode->getCollectionCallbacks().push_back( _opaqueToggleCB );
    }

    // Add translated model instance for ABuffer transparency
    _abufNode.reset( new jagSG::Node() );
    gmtl::setTrans( _abufNode->getTransform(), gmtl::Vec3d( 0., model->getBound()->getRadius() * -1.5, 0. ) );
    _root->addChild( _abufNode );
    _abufNode->addChild( model );
    _aBuffer->setTransparencyEnable( *_abufNode );

    _abufOpaqueChild.reset(new jagSG::Node() );
    gmtl::setTrans( _abufOpaqueChild->getTransform(), gmtl::Vec3d( model->getBound()->getRadius() * -1.5,0., 0. ) );
    _abufOpaqueChild->addChild(model);
    _abufNode->addChild(_abufOpaqueChild);


    // For gamepad speed control
    _moveRate = _root->getBound()->getRadius();


    // Prepare for culling.
    jagSG::FrustumCullDistributionVisitor fcdv;
    _root->accept( fcdv );
    jagSG::SmallFeatureDistributionVisitor sfdv;
    _root->accept( sfdv );

    // Optimize VAO and element buffers.
    jagUtil::BufferAggregationVisitor bav( _root );


    // The scene graph will have two CommandMaps, one for opaque
    // rendering and the other for ABuffer transparency. The opaque
    // CommandMap will sit at the root of the scene graph, and the
    // application will add or remove the ABuffer CommandMap as
    // needed to toggle transparency on and off.

    // Create opaque CommandMap.
    jag::draw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagmodel.vert" ) );
    jag::draw::ShaderPtr fs( DemoInterface::readShaderUtil( "abufferJagGlow.frag" ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jag::draw::CommandMapPtr commands( _root->getOrCreateCommandMap() );
    commands->insert( prog );

    {
        // Root node needs a default value for glowColor uniform.
        jag::draw::UniformPtr glowUniform( jag::draw::UniformPtr( new jag::draw::Uniform( "glowColor", gmtl::Point4f(0.f,0.f,0.f,0.f) ) ) );
        jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
        usp->insert( glowUniform );
        commands->insert( usp );
    }

    // Set the FBO for main rendering.
    commands->insert( _opaqueFBO );

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
	_aBuffer->setOpaqueControls( commands, 0 );
	_aBuffer->setTransparencyEnable( *_abufOpaqueChild, false );
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


    return( true );
}

bool ABufferJag::init()
{
    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    return( true );
}


#define ENABLE_SORT

bool ABufferJag::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    JAG3D_TRACE_STATIC( _logName, "frame()" );

    JAG3D_PROFILE( "frame" );

#ifdef ENABLE_SORT
    jag::draw::Command::CommandTypeVec plist;
    plist.push_back( jag::draw::Command::UniformBlockSet_t );
#endif

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jag::mx::MxCorePtr mxCore( _mxCore._data[ contextID ] );

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

        // The ABuffer object handles rendering.
        // This line replaces drawGraph->execute( drawInfo );
        _aBuffer->renderFrame( collect, drawInfo );
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

void ABufferJag::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    _width = w;
    _height = h;


    jag::draw::ImagePtr image( new jag::draw::Image() );
    image->set( 0, GL_RGBA, w, h, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _opaqueBuffer->setImage( image );
    _secondaryBuffer->setImage( image );
    _glowBuffer->setImage( image );

    image.reset( new jag::draw::Image() );
    image->set( 0, GL_DEPTH_COMPONENT, w, h, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
    _depthBuffer->setImage( image );

    _opaqueFBO->setViewport( 0, 0, w, h );

    _blur->reshape( w, h );
    _aBuffer->reshape( w, h );

    // Set aspect for all matrix control objects.
    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    _mxCore._data[ contextID ]->setAspect( ( double ) w / ( double ) h );
}

bool ABufferJag::keyCommand( const int command )
{
    switch( command )
    {
    default:
        return( false ); // Unhandled. Do not redraw.
        break;

    case (int)'g': // Toggle glow
        ++glowIndex;
        if( glowIndex >= sizeof( glowColors ) / sizeof( gmtl::Point4f ) )
            glowIndex = 0;
        _glowColor->set( glowColors[ glowIndex ] );
        break;

    case (int)'o': // Toggle opaque
        if( _opaqueToggleCB->getOverride() == jagSG::NodeMaskCullCallback::OVERRIDE_TRUE )
            _opaqueToggleCB->setOverride( jagSG::NodeMaskCullCallback::OVERRIDE_FALSE );
        else
            _opaqueToggleCB->setOverride( jagSG::NodeMaskCullCallback::OVERRIDE_TRUE );
        break;

    case (int)'e': // Toggle transparency
        _aBuffer->toggleTransparencyEnable( *_abufNode );
        break;

    case (int)'c': // Toggle transparent child
        _aBuffer->toggleTransparencyEnable( *_abufOpaqueChild );
        break;

    case (int)'a': // Cycle transparency resolve methods
        _aBuffer->setResolveMethod( jagUtil::ABuffer::ResolveMethod( _aBuffer->getResolveMethod() + 1 ) );
        if( _aBuffer->getResolveMethod() == jagUtil::ABuffer::UNSPECIFIED )
            _aBuffer->setResolveMethod( jagUtil::ABuffer::RESOLVE_GELLY );
        JAG3D_CRITICAL_STATIC( _logName, "Using " + jagUtil::ABuffer::resolveMethodToString( _aBuffer->getResolveMethod() ) );
        break;

    case (int)'t': // Decrease/increate fragment alpha.
        _aBuffer->setFragmentAlpha( _aBuffer->getFragmentAlpha() - 0.033f );
        break;
    case (int)'T':
        _aBuffer->setFragmentAlpha( _aBuffer->getFragmentAlpha() + 0.033f );
        break;

	case (int)'f': // Decrease/increase maximum fragments per pixel.
		_aBuffer->setMaxFragments( max<int>(_aBuffer->getMaxFragments()-5,5));
		break;
	case (int)'F':
		_aBuffer->setMaxFragments( _aBuffer->getMaxFragments() + 5);
		break;
    }

    return( true ); // cause redraw.
}


/** \defgroup exampleJagABufferJag The abufferJag Example
This example demonstrates use of jagUtil::Blur and jagUtil::ABuffer.

Command line options:
\li --help,-h Help text
\li --version,-v OpenGL context version. Default: 4.0.
\li --nwin Number of windows. Default: 1.
\li --winsize,-w Window width and height. Default: 800 600.
\li --no-ms Disable multisampling
\li --file,-f Model to load.

Key controls:
\li g Toggle opaque object's glow effect
\li o Toggle opaque object on/off
\li e Toggle transparent subgraph to opaque
\li c Toggle transparent child to opaque
\li a Cycle transparency resolve methods
\li t/T Decrease/increate fragment alpha.
\li f/F Decrease/increase maximum fragments per pixel.
*/
