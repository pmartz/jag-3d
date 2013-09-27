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

#include <jagDraw/Common.h>
#include <jagDraw/PerContextData.h>
#include <jagSG/Common.h>
#include <jagDisk/ReadWrite.h>
#include <jagBase/Profile.h>
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
    virtual bool shutdown()
    {
        return( true );
    }

    // Return a value to bontrol base gamepad move rate in the scene.
    virtual double getMoveRate() const
    {
        return( _moveRate );
    }

protected:
    std::string _fileName;

    jagSG::NodePtr _root;

    double _moveRate;

    jagDraw::DrawGraphPtr createDrawGraph( const unsigned int numContexts );

    jagDraw::FramebufferPtr _defaultFBO, _opaqueFBO;
    jagDraw::TexturePtr _opaqueBuffer, _depthBuffer;
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


#define ABUFFER_PAGE_SIZE 4

struct ABufferContext
{
    ABufferContext()
        : _needsInit( true ),
          _texWidth( 0 ),
          _texHeight( 0 ),
          _queryRequested( false ),
          _sharedPoolSize( 16 ),
          _queryID( 0 ),
          _pageIdxBuffID( 0 ),
          _fragCountBuffID( 0 ),
          _semaphoreBuffID( 0 ),
          _sharedPageListBuffID( 0 ),
          _sharedLinkListBuffID( 0 ),
          _curSharedPageBuffID( 0 )
    {
    }

    bool _needsInit;
    int _texWidth, _texHeight;

    typedef float AbufferType;

    bool _queryRequested;
    GLuint _queryID;

    unsigned int _sharedPoolSize;
    GLuint _pageIdxBuffID, _fragCountBuffID, _semaphoreBuffID;
    GLuint64EXT _pageIdxAddress, _fragCountAddress, _semaphoreAddress;
    GLuint _sharedPageListBuffID, _sharedLinkListBuffID;
    GLuint64EXT _sharedPageListAddress, _sharedLinkListAddress;
    GLuint _curSharedPageBuffID;
    GLuint64EXT _curSharedPageAddress;

    bool init()
    {
        if( _queryID == 0 )
            glGenQueries( 1, &_queryID );

		///Page idx storage///
        if( _pageIdxBuffID == 0 )
    	    glGenBuffers( 1, &_pageIdxBuffID );
		glBindBuffer( GL_TEXTURE_BUFFER, _pageIdxBuffID );
		glBufferData( GL_TEXTURE_BUFFER, _texWidth * _texHeight * sizeof( GLuint ), NULL, GL_STATIC_DRAW );
		glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_pageIdxAddress );
		JAG3D_ERROR_CHECK( "abufferPageIdxID" );

		///per-pixel page counter///
        if( _fragCountBuffID == 0 )
		    glGenBuffers( 1, &_fragCountBuffID );
		glBindBuffer( GL_TEXTURE_BUFFER, _fragCountBuffID );
		glBufferData( GL_TEXTURE_BUFFER, _texWidth * _texHeight * sizeof( GLuint ), NULL, GL_STATIC_DRAW );
		glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_fragCountAddress );
		JAG3D_ERROR_CHECK( "abufferFragCountID" );

		///Semaphore///
        if( _semaphoreBuffID == 0 )
		    glGenBuffers( 1, &_semaphoreBuffID );
		glBindBuffer( GL_TEXTURE_BUFFER, _semaphoreBuffID );
		glBufferData( GL_TEXTURE_BUFFER, _texWidth * _texHeight * sizeof( GLuint ), NULL, GL_STATIC_DRAW );
		glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_semaphoreAddress );
		JAG3D_ERROR_CHECK( "semaphoreTexID" );

		///Shared page list///
        if( _sharedPageListBuffID == 0 )
		    glGenBuffers( 1, &_sharedPageListBuffID );
		glBindBuffer( GL_TEXTURE_BUFFER, _sharedPageListBuffID );
		glBufferData( GL_TEXTURE_BUFFER, _sharedPoolSize * sizeof( AbufferType ) * 4, NULL, GL_STATIC_DRAW );
		glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_sharedPageListAddress ); 

		///Shared link pointer list///
        if( _sharedLinkListBuffID == 0 )
		    glGenBuffers( 1, &_sharedLinkListBuffID );
		glBindBuffer( GL_TEXTURE_BUFFER, _sharedLinkListBuffID );
		glBufferData( GL_TEXTURE_BUFFER, _sharedPoolSize / ABUFFER_PAGE_SIZE * sizeof( GLuint ), NULL, GL_STATIC_DRAW );
		glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_sharedLinkListAddress ); 

	    ///Shared page counter///
	    if( _curSharedPageBuffID == 0 )
		    glGenBuffers( 1, &_curSharedPageBuffID );
	    glBindBuffer( GL_ARRAY_BUFFER_ARB, _curSharedPageBuffID );
	    glBufferData( GL_ARRAY_BUFFER_ARB, sizeof( GLuint ), NULL, GL_STATIC_DRAW );
	    glMakeBufferResidentNV( GL_ARRAY_BUFFER_ARB, GL_READ_WRITE );
	    glGetBufferParameterui64vNV( GL_ARRAY_BUFFER_ARB, GL_BUFFER_GPU_ADDRESS_NV, &_curSharedPageAddress );
	    JAG3D_ERROR_CHECK( "curSharedPageBuffID" );

        std::ostringstream ostr;
        ostr << "[ABuffer Linked Lists] Memory usage: " <<
            ( _sharedPoolSize * 4 * sizeof( float ) / 1024 ) / 1024.0f << "MB";
        JAG3D_INFO_STATIC( "jag.ex.abuf", ostr.str() );

        _needsInit = false;

        return( true );
    }

    void assignUniforms( const GLuint prog )
    {
		glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_abufferPageIdx" ), _pageIdxAddress );
		glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_abufferFragCount" ), _fragCountAddress );
		glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_semaphore" ), _semaphoreAddress );

		glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_sharedPageList" ), _sharedPageListAddress );
		glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_sharedLinkList" ), _sharedLinkListAddress );

	    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_curSharedPage" ), _curSharedPageAddress );

	    glProgramUniform1iEXT( prog, glGetUniformLocation( prog, "abufferSharedPoolSize" ), _sharedPoolSize );
    }

    bool managePool()
    {
	    GLuint lastFrameNumFrags( 0 );
	    if( _queryRequested )
        {
		    glGetQueryObjectuiv( _queryID, GL_QUERY_RESULT, &lastFrameNumFrags );
	
		    //A fragments is not discarded each time a page fails to be allocated
		    if( lastFrameNumFrags > 0 )
            {
			    _sharedPoolSize = _sharedPoolSize + ( lastFrameNumFrags / ABUFFER_PAGE_SIZE + 1 ) * ABUFFER_PAGE_SIZE * 2;
                _needsInit = true;
		    }
		    _queryRequested = false;
	    }

	    return( lastFrameNumFrags > 0 );
    }
};
typedef jagDraw::PerContextData< ABufferContext > PerContextABufferCntxt;
PerContextABufferCntxt abufferCntxt;

jagDraw::ProgramPtr _abufferRenderProgram;

jagDraw::DrawGraphPtr ABufferJag::createDrawGraph( const unsigned int numContexts )
{
    // Create program for rendering into the abuffer.
    {
        jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "abufferRender.vert" ) );
        jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "abufferRender.frag" ) );

        _abufferRenderProgram.reset( new jagDraw::Program() );
        _abufferRenderProgram->attachShader( vs );
        _abufferRenderProgram->attachShader( fs );
        _abufferRenderProgram->addVertexAttribAlias( "vertexPos", "vertex" );
        _abufferRenderProgram->addVertexAttribAlias( "vertexNormal", "normal" );
        _abufferRenderProgram->addUniformAlias( "projectionMat", "jagProjMatrix" );
        _abufferRenderProgram->addUniformAlias( "modelViewMat", "jagModelViewMatrix" );
        _abufferRenderProgram->addUniformAlias( "modelViewMatIT", "jagModelViewInvTrans4Matrix" );
        _abufferRenderProgram->setMaxContexts( numContexts );
    }

    // We need 4 containers:
    //   0: Opaque
    //   1: Clear the ABuffer
    //   2: ABuffer render
    //   3: ABuffer resolve and opaque combine
    jagDraw::DrawGraphPtr drawGraphTemplate( new jagDraw::DrawGraph() );
    drawGraphTemplate->resize( 4 );

    // Element 0: Opaque. Contents set by CollectionVisitor.
    {
        jagDraw::NodeContainer& nc( (*drawGraphTemplate)[ 0 ] );

        struct OpaqueControlCallback : public jagDraw::NodeContainer::Callback
        {
            virtual bool operator()( jagDraw::NodeContainer& nc, jagDraw::DrawInfo& di )
            {
                // Enable depth test and face culling for opaque rendering.
                glEnable( GL_DEPTH_TEST );
                glDepthMask( GL_TRUE );
                //glEnable(GL_CULL_FACE);

                // Render the opaque geometry.
                nc.internalExecute( di );

                // Disable depth test and face culling for the rest of the frame.
                glDisable( GL_DEPTH_TEST );
                glDepthMask( GL_FALSE );
                glDisable(GL_CULL_FACE);

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }
        };

        nc.getCallbacks().push_back(
            jagDraw::NodeContainer::CallbackPtr(
                new OpaqueControlCallback() ) );
    }

    // Element 1: Full screen tri pair to clear the ABuffer.
    jagDraw::VertexArrayObjectPtr fstpVAO; // Shared with container elements 1 and 3.
    jagDraw::DrawablePtr fstp; // Shared with container elements 1 and 3.
    {
        jagDraw::NodeContainer& nc( (*drawGraphTemplate)[ 1 ] );

        jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );

        jagUtil::VNTCVec data;
        fstp = jagUtil::makePlane( data, gmtl::Point3f( -1., -1., 0. ),
            gmtl::Vec3f( 2., 0., 0. ), gmtl::Vec3f( 0., 2., 0. ) );
        fstpVAO = jagUtil::createVertexArrayObject( data );
        commands->insert( fstpVAO );

        struct ABufferClearCallback : public jagDraw::NodeContainer::Callback
        {
            jagDraw::FramebufferPtr fbo;

            virtual bool operator()( jagDraw::NodeContainer& nc, jagDraw::DrawInfo& di )
            {
                const jagDraw::jagDrawContextID contextID( di._id );

                ABufferContext& abc( abufferCntxt[ contextID ] );
                if( abc._needsInit )
                    abc.init();

                fbo->execute( di );
                di._current.insert( fbo );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                _clearProgram->execute( di );
                abc.assignUniforms( _clearProgram->getID( contextID ) );

                // Render the tri pair to clear the abuffer.
                nc.internalExecute( di );

                glMemoryBarrierEXT( GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV );

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }

            jagDraw::ProgramPtr _clearProgram;
        };
        typedef jagBase::ptr< ABufferClearCallback >::shared_ptr ABufferClearCallbackPtr;
        ABufferClearCallbackPtr abccb( ABufferClearCallbackPtr( new ABufferClearCallback() ) );
        abccb->fbo = _defaultFBO;
        nc.getCallbacks().push_back( abccb );

        {
            jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "abufferClear.vert" ) );
            jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "abufferClear.frag" ) );

            abccb->_clearProgram.reset( new jagDraw::Program() );
            abccb->_clearProgram->attachShader( vs );
            abccb->_clearProgram->attachShader( fs );
            abccb->_clearProgram->addVertexAttribAlias( "vertexPos", "vertex" );
            commands->insert( abccb->_clearProgram );
        }

        jagDraw::DrawNodePtr node( jagDraw::DrawNodePtr( new jagDraw::Node() ) );
        node->setCommandMap( commands );
        node->addDrawable( fstp );

        nc.push_back( node );
        nc.setResetEnable( false );
    }

    // Element 2: ABuffer. Contents set by CollectionVisitor.
    {
        jagDraw::NodeContainer& nc( (*drawGraphTemplate)[ 2 ] );

        struct ABufferRenderCallback : public jagDraw::NodeContainer::Callback
        {
            virtual bool operator()( jagDraw::NodeContainer& nc, jagDraw::DrawInfo& di )
            {
                //return( false );
                const jagDraw::jagDrawContextID contextID( di._id );

                ABufferContext& abc( abufferCntxt[ contextID ] );

                _abufferRenderProgram->execute( di );
                abc.assignUniforms( _abufferRenderProgram->getID( contextID ) );

                // Count fragments that were not discarded. This is an indication
                // that we need to resize our page pool and re-render.
                glBeginQuery( GL_SAMPLES_PASSED, abc._queryID );

                // Render the tri pair to clear the abuffer.
                nc.internalExecute( di );

                glEndQuery( GL_SAMPLES_PASSED );
                abc._queryRequested = true;

                glMemoryBarrierEXT( GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV );

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }

        };
        typedef jagBase::ptr< ABufferRenderCallback >::shared_ptr ABufferRenderCallbackPtr;
        ABufferRenderCallbackPtr abrcb( ABufferRenderCallbackPtr( new ABufferRenderCallback() ) );
        nc.getCallbacks().push_back( abrcb );
    }

    // Element 3: Full sceen tri pair to resolve ABuffer & combine with opaque.
    {
        jagDraw::NodeContainer& nc( (*drawGraphTemplate)[ 3 ] );

        struct ABufferResolveCallback : public jagDraw::NodeContainer::Callback
        {
            virtual bool operator()( jagDraw::NodeContainer& nc, jagDraw::DrawInfo& di )
            {
                const jagDraw::jagDrawContextID contextID( di._id );

                ABufferContext& abc( abufferCntxt[ contextID ] );

                _resolveProgram->execute( di );
                abc.assignUniforms( _resolveProgram->getID( contextID ) );

                // Render the tri pair to clear the abuffer.
                nc.internalExecute( di );

                bool needsRedraw( abc.managePool() );

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }

            jagDraw::ProgramPtr _resolveProgram;
        };
        typedef jagBase::ptr< ABufferResolveCallback >::shared_ptr ABufferResolveCallbackPtr;
        ABufferResolveCallbackPtr abrcb( ABufferResolveCallbackPtr( new ABufferResolveCallback() ) );
        nc.getCallbacks().push_back( abrcb );

        jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
        commands->insert( fstpVAO );

        {
            jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "abufferResolve.vert" ) );
            jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "abufferResolve.frag" ) );

            abrcb->_resolveProgram.reset( new jagDraw::Program() );
            abrcb->_resolveProgram->attachShader( vs );
            abrcb->_resolveProgram->attachShader( fs );
            abrcb->_resolveProgram->addVertexAttribAlias( "vertexPos", "vertex" );
            commands->insert( abrcb->_resolveProgram );
        }

        jagDraw::TextureSetPtr texSet( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
        (*texSet)[ GL_TEXTURE15 ] = _opaqueBuffer;
        commands->insert( texSet );
        jagDraw::UniformPtr sampler( jagDraw::UniformPtr( new jagDraw::Uniform( "opaqueBuffer", GL_SAMPLER_2D, 15 ) ) );
        jagDraw::UniformSetPtr usp( jagDraw::UniformSetPtr( new jagDraw::UniformSet() ) );
        usp->insert( sampler );
        commands->insert( usp );

        jagDraw::DrawNodePtr node( jagDraw::DrawNodePtr( new jagDraw::Node() ) );
        node->setCommandMap( commands );
        node->addDrawable( fstp );

        nc.push_back( node );
        nc.setResetEnable( false );
    }

    //drawGraphTemplate->resize( 3 );
    return( drawGraphTemplate );
}

bool ABufferJag::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    JAG3D_INFO_STATIC( _logName, _fileName );

    if( _fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--file <fileName>' on command line." );
        return( false );
    }


    // Create default FBO. First NodeContainer renders to _opaqueFBO,
    // but other NodeContainers render directly to the window.
    _defaultFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _defaultFBO->setViewport( 0, 0, _width, _height );
    _defaultFBO->setClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _defaultFBO->setMaxContexts( numContexts );

    // Create the texture used by _opaqueFBO to store the opaque
    // color buffer. After NodeContainer #0 renders into it, 
    // NodeContainer #3 uses it during the abuffer resolve.
    jagDraw::ImagePtr image( new jagDraw::Image() );
    image->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _opaqueBuffer.reset( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    _opaqueBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _opaqueBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _opaqueBuffer->setMaxContexts( numContexts );

    // Depth texture used as depth buffer for opaque pass.
    // Also used during abuffer render to discard occluded fragments.
    image.reset( new jagDraw::Image() );
    image->set( 0, GL_DEPTH_COMPONENT, _width, _height, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
    _depthBuffer.reset( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    _depthBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _depthBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;
    _depthBuffer->setMaxContexts( numContexts );

    // Prepare the draw graph.
    // Node: createDrawGraph() initializes _abufferRenderProgram, so we must
    // call createDrawGraph() before adding _abufferRenderProgram to a CommandMap.
    jagDraw::DrawGraphPtr drawGraph( createDrawGraph( numContexts ) );
    getCollectionVisitor().setDrawGraphTemplate( drawGraph );
    // Tell draw graph how many contexts to expect.
    drawGraph->setMaxContexts( numContexts );

    jagDraw::TransformCallback* xformCB( getCollectionVisitor().getTransformCallback() );
    xformCB->setRequiredMatrixUniforms(
        jagBase::TransformD::PROJ |
        jagBase::TransformD::MODEL_VIEW_PROJ |
        jagBase::TransformD::MODEL_VIEW |
        jagBase::TransformD::MODEL_VIEW_INV_TRANS |
        jagBase::TransformD::MODEL_VIEW_INV_TRANS_4
        );

    // Per context ABuffer data.
    abufferCntxt._data.resize( numContexts );


    // Prepare the scene graph.
    jagSG::NodePtr model( DemoInterface::readSceneGraphNodeUtil( _fileName ) );
    if( model == NULL )
        return( false );

    // Add model instance as opaque
    _root.reset( new jagSG::Node );
    _root->addChild( model );

    // Add translated model instance for ABuffer transparency
    jagSG::NodePtr xformNode( jagSG::NodePtr( new jagSG::Node() ) );
    gmtl::setTrans( xformNode->getTransform(), gmtl::Vec3d( 0., model->getBound()->getRadius() * -1.5, 0. ) );
    _root->addChild( xformNode );
    xformNode->addChild( model );
    {
        // Specify commands for abuffer rendering.
        jagDraw::CommandMapPtr& commands( xformNode->getOrCreateCommandMap() );
        commands->insert( _abufferRenderProgram );
        commands->insert( _defaultFBO );

        jagDraw::TextureSetPtr texSet( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
        (*texSet)[ GL_TEXTURE14 ] = _depthBuffer;
        commands->insert( texSet );
        jagDraw::UniformPtr sampler( jagDraw::UniformPtr( new jagDraw::Uniform( "depthBuffer", GL_SAMPLER_2D, 14 ) ) );
        jagDraw::UniformSetPtr usp( jagDraw::UniformSetPtr( new jagDraw::UniformSet() ) );
        usp->insert( sampler );
        commands->insert( usp );
    }
    // ABuffer container is container #2.
    xformNode->getCollectionCallbacks().push_back(
        jagSG::Node::CallbackPtr( new jagSG::SelectContainerCallback( 2 ) ) );


    // For gamepad speed control
    _moveRate = _root->getBound()->getRadius();

        

    jagSG::FrustumCullDistributionVisitor fcdv;
    _root->accept( fcdv );
    jagSG::SmallFeatureDistributionVisitor sfdv;
    _root->accept( sfdv );

    jagUtil::BufferAggregationVisitor bav( _root );


    // The scene graph will have two CommandMaps, one for opaque
    // rendering and the other for ABuffer transparency. The opaque
    // CommandMap will sit at the root of the scene graph, and the
    // application will add or remove the ABuffer CommandMap as
    // needed to toggle transparency on and off.

    // Create opaque CommandMap.
    jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagmodel.vert" ) );
    jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "jagmodel.frag" ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( _root->getOrCreateCommandMap() );
    commands->insert( prog );

    // Create a framebuffer object. The color texture will store opaque
    // rendering. The depth texture is shared with ABuffer to avoid
    // rendering behind opaque geometry.
    _opaqueFBO.reset( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _opaqueFBO->setViewport( 0, 0, _width, _height );
    _opaqueFBO->setClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _opaqueFBO->addAttachment( GL_COLOR_ATTACHMENT0, _opaqueBuffer );
    _opaqueFBO->addAttachment( GL_DEPTH_ATTACHMENT, _depthBuffer );
    commands->insert( _opaqueFBO );

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


    return( true );
}

bool ABufferJag::init()
{
    glClearColor( 1.f, 1.f, 1.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    return( true );
}


#define ENABLE_SORT

bool ABufferJag::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
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
            BOOST_FOREACH( jagDraw::NodeContainer& nc, *drawGraph )
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

void ABufferJag::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    _width = w;
    _height = h;

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    _mxCore._data[ contextID ]->setAspect( ( double ) w / ( double ) h );

    ABufferContext& abc( abufferCntxt[ contextID ] );
    abc._texWidth = w;
    abc._texHeight = h;
    abc._needsInit = true;
}
