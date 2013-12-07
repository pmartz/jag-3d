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

#include <jagUtil/ABuffer.h>

#include <jag/draw/Common.h>
#include <jag/draw/PerContextData.h>
#include <jagSG/Common.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/Profile.h>
#include <jagUtil/DrawGraphCountVisitor.h>
#include <jagUtil/Shapes.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>
#include <sstream>
#include <iomanip>


namespace jagUtil
{


ABuffer::ABuffer( const std::string& logName )
    : jag::base::LogBase( logName.empty() ? "jag.util.abuf" : logName ),
      _startContainer( 1 ),
      _secondaryEnable( false ),
      _width( 0 ),
      _height( 0 ),
      _opaqueContainer( 0 ),
      _resolveMethod( RESOLVE_ALPHA_BLEND_CAD ),
      _fragmentAlpha( 0.2f ),
      _maxFragments( 16 ),
      _numContexts( 0 )
{
}
ABuffer::ABuffer( jag::draw::TexturePtr& depthBuffer, jag::draw::TexturePtr& colorBuffer0,
                 jag::draw::TexturePtr& colorBuffer1,
                 const std::string& logName )
    : jag::base::LogBase( logName.empty() ? "jag.util.abuf" : logName ),
      _colorBuffer0( colorBuffer0 ),
      _colorBuffer1( colorBuffer1 ),
      _depthBuffer( depthBuffer ),
      _startContainer( 1 ),
      _secondaryEnable( colorBuffer1 != NULL ),
      _width( 0 ),
      _height( 0 ),
      _opaqueContainer( 0 ),
      _resolveMethod( RESOLVE_ALPHA_BLEND_CAD ),
      _fragmentAlpha( 0.2f ),
      _maxFragments( 16 ),
      _numContexts( 0 )
{
}
ABuffer::ABuffer( const ABuffer& rhs )
    : jag::base::LogBase( rhs ),
      _colorBuffer0( rhs._colorBuffer0 ),
      _colorBuffer1( rhs._colorBuffer1 ),
      _depthBuffer( rhs._depthBuffer ),
      _startContainer( rhs._startContainer ),
      _width( rhs._width ),
      _height( rhs._height ),
      _opaqueCommands( rhs._opaqueCommands ),
      _opaqueContainer( rhs._opaqueContainer ),
      _resolveMethod( rhs._resolveMethod ),
      _fragmentAlpha( rhs._fragmentAlpha ),
      _maxFragments( rhs._maxFragments ),
      _numContexts( rhs._numContexts )
{
}
ABuffer::~ABuffer()
{
}

void ABuffer::setMaxContexts( const unsigned int numContexts )
{
    _numContexts = numContexts;

    _abufferCntxt._data.resize( numContexts );
    BOOST_FOREACH( ABufferContext& data, _abufferCntxt._data )
        data._logName = _logger.name();


    if( _callback == NULL )
        internalInit();

    _clearProgram->setMaxContexts( numContexts );
    _renderProgram->setMaxContexts( numContexts );
    _resolveProgram->setMaxContexts( numContexts );
    _defaultFBO->setMaxContexts( numContexts );
    _commands->setMaxContexts( numContexts );

    if( _drawGraphTemplate != NULL )
        _drawGraphTemplate->setMaxContexts( numContexts );
}


jag::draw::DrawGraphPtr& ABuffer::createDrawGraphTemplate( const unsigned int startContainer )
{
    if( _callback == NULL )
        internalInit();

    if( _drawGraphTemplate == NULL )
        _drawGraphTemplate.reset( new jag::draw::DrawGraph() );

    // ABuffer needs 3 NodeContainers:
    //   sc:   Clear the ABuffer
    //   sc+1: ABuffer render
    //   sc+2: ABuffer resolve and opaque combine
    // where sc is _startContainer.
    // _startContainer is typically 1, leaving NodeContainer 0 for application use
    // (such as rendering opaque geometry into the _depthBuffer and _colorBufferN.
    _startContainer = startContainer;
    _drawGraphTemplate->resize( startContainer + 3 );
    _callback->setContainer( _startContainer + 1 );


    // First, create the full screen tri pair used by the first and last NodeContainers.
    jag::draw::VertexArrayObjectPtr fstpVAO;
    jag::draw::DrawablePtr fstp;
    {
        jagUtil::VNTCVec data;
        fstp = jagUtil::makePlane( data, gmtl::Point3f( -1., -1., 0. ),
            gmtl::Vec3f( 2., 0., 0. ), gmtl::Vec3f( 0., 2., 0. ) );
        fstpVAO = jagUtil::createVertexArrayObject( data );
    }


    // Element 1: Full screen tri pair to clear the ABuffer.
    {
        jag::draw::DrawNodeContainer& nc( (*_drawGraphTemplate)[ _startContainer ] );

        struct ABufferClearCallback : public jag::draw::DrawNodeContainer::Callback
        {
            ABufferClearCallback( PerContextABufferCntxt& abc )
                : _abufferCntxt( abc )
            {}

            virtual bool operator()( jag::draw::DrawNodeContainer& nc, jag::draw::DrawInfo& di )
            {
                const jag::draw::jagDrawContextID contextID( di._id );
                ABufferContext& abc( _abufferCntxt[ contextID ] );

                if( abc._needsInit )
                    abc.init();

                _clearProgram->execute( di );
                abc.assignUniforms( _clearProgram->getID( contextID ) );
                _fbo->execute( di );
                di._current.insert( _fbo );

                // Set OpenGL state for a-buffer rendering.
                glDisable( GL_DEPTH_TEST );
                glDepthMask( GL_FALSE );
                glDisable( GL_CULL_FACE );

                // Render the tri pair to clear the abuffer.
                nc.internalExecute( di );

                glMemoryBarrierEXT( GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV );

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }

            jag::draw::ProgramPtr _clearProgram;
            PerContextABufferCntxt& _abufferCntxt;
            jag::draw::FramebufferPtr _fbo;
        };
        typedef jag::base::ptr< ABufferClearCallback >::shared_ptr ABufferClearCallbackPtr;
        ABufferClearCallbackPtr abccb( ABufferClearCallbackPtr( new ABufferClearCallback( _abufferCntxt ) ) );
        abccb->_clearProgram = _clearProgram;
        abccb->_fbo = _defaultFBO;
        nc.getCallbacks().push_back( abccb );


        jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new jag::draw::CommandMap() ) );
        commands->insert( fstpVAO );
        commands->insert( _clearProgram );

        jag::draw::DrawNodePtr node( jag::draw::DrawNodePtr( new jag::draw::DrawNode() ) );
        node->setCommandMap( commands );
        node->addDrawable( fstp );

        nc.push_back( node );
        nc.setResetEnable( false );
    }

    // Element 2: ABuffer. Contents set by CollectionVisitor.
    {
        jag::draw::DrawNodeContainer& nc( (*_drawGraphTemplate)[ _startContainer + 1 ] );

        struct ABufferRenderCallback : public jag::draw::DrawNodeContainer::Callback
        {
            ABufferRenderCallback( PerContextABufferCntxt& abc )
                : _abufferCntxt( abc )
            {}

            virtual bool operator()( jag::draw::DrawNodeContainer& nc, jag::draw::DrawInfo& di )
            {
                const jag::draw::jagDrawContextID contextID( di._id );
                ABufferContext& abc( _abufferCntxt[ contextID ] );

                _renderProgram->execute( di );
                abc.assignUniforms( _renderProgram->getID( contextID ) );

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

            jag::draw::ProgramPtr _renderProgram;
            PerContextABufferCntxt& _abufferCntxt;
        };
        typedef jag::base::ptr< ABufferRenderCallback >::shared_ptr ABufferRenderCallbackPtr;
        ABufferRenderCallbackPtr abrcb( ABufferRenderCallbackPtr( new ABufferRenderCallback( _abufferCntxt ) ) );
        abrcb->_renderProgram = _renderProgram;
        nc.getCallbacks().push_back( abrcb );
    }

    // Element 3: Full sceen tri pair to resolve ABuffer & combine with opaque.
    {
        jag::draw::DrawNodeContainer& nc( (*_drawGraphTemplate)[ _startContainer + 2 ] );

        struct ABufferResolveCallback : public jag::draw::DrawNodeContainer::Callback
        {
            ABufferResolveCallback( PerContextABufferCntxt& abc )
                : _abufferCntxt( abc )
            {}

            virtual bool operator()( jag::draw::DrawNodeContainer& nc, jag::draw::DrawInfo& di )
            {
                const jag::draw::jagDrawContextID contextID( di._id );

                ABufferContext& abc( _abufferCntxt[ contextID ] );

                _resolveProgram->execute( di );
                abc.assignUniforms( _resolveProgram->getID( contextID ) );

                // Render the tri pair to clear the abuffer.
                nc.internalExecute( di );

                glEnable( GL_DEPTH_TEST );
                glDepthMask( GL_TRUE );
                glEnable( GL_CULL_FACE );

                // We've already rendered; return false to abort this NodeContainer.
                return( false );
            }

            jag::draw::ProgramPtr _resolveProgram;
            PerContextABufferCntxt& _abufferCntxt;
        };
        typedef jag::base::ptr< ABufferResolveCallback >::shared_ptr ABufferResolveCallbackPtr;
        ABufferResolveCallbackPtr abrcb( ABufferResolveCallbackPtr( new ABufferResolveCallback( _abufferCntxt ) ) );
        abrcb->_resolveProgram = _resolveProgram;
        nc.getCallbacks().push_back( abrcb );


        jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new jag::draw::CommandMap() ) );
        commands->insert( fstpVAO );
        commands->insert( _resolveProgram );

        jag::draw::TextureSetPtr texSet( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );
        commands->insert( texSet );
        jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
        commands->insert( usp );

        (*texSet)[ GL_TEXTURE14 ] = _colorBuffer0;
        jag::draw::UniformPtr sampler( jag::draw::UniformPtr( new jag::draw::Uniform( "opaqueBuffer", GL_SAMPLER_2D, 14 ) ) );
        usp->insert( sampler );
        if( _colorBuffer1 != NULL )
        {
            (*texSet)[ GL_TEXTURE15 ] = _colorBuffer1;
            jag::draw::UniformPtr sampler( jag::draw::UniformPtr( new jag::draw::Uniform( "secondaryBuffer", GL_SAMPLER_2D, 15 ) ) );
            usp->insert( sampler );
        }

        jag::draw::DrawNodePtr node( jag::draw::DrawNodePtr( new jag::draw::DrawNode() ) );
        node->setCommandMap( commands );
        node->addDrawable( fstp );

        nc.push_back( node );
        nc.setResetEnable( false );
    }

    _drawGraphTemplate->setMaxContexts( _numContexts );

    return( _drawGraphTemplate );
}


void ABuffer::setTransparencyEnable( jagSG::Node& node, const bool enable )
{
    if( _callback == NULL )
        internalInit();

    /*
    Get commandmap from node.
    if enable
      if commandmap == abuf commandmap
        return
      else if commandmap == opaque commandmap
        set it to null
      if commandmap == null
        set abuf commandmap
      else
        save commandmap as user data
        set commandmap + abuf commandmap
      remove opaque nodecontainer
      set abuf nodecontainer
    else if disable
      if commandmap == opaque commandmap
        return
      if user data commandmap exists
        set user data commandmap
      else if commandmap == null
        set opaque command map
      else
        set commandmap + opaque command map
      remove abuf nodecontainer
      set opaque nodecontainer
    */

    jag::draw::CommandMapPtr commands( node.getCommandMap() );
    if( enable )
    {
        if( commands == _commands )
            // This is a redundant enable.
            return;
        else if( commands == _opaqueCommands )
            commands = NULL;

        if( commands == NULL )
            node.setCommandMap( _commands );
        else
        {
            node.setUserDataPair( "ABufferOriginalCommandMap", commands );
            node.setCommandMap( jag::draw::CommandMapPtr( new jag::draw::CommandMap(
                *commands + *_commands ) ) );
        }
        // Specify NodeContainer selection callback.
        node.getCollectionCallbacks().remove( _opaqueCallback );
        node.getCollectionCallbacks().addUnique( _callback );
    }
    else // disable
    {
        if( commands == _opaqueCommands )
            // This is a redundant disable.
            return;
        if( node.hasUserData( "ABufferOriginalCommandMap" ) )
        {
            jag::draw::CommandMapPtr savedCommands( node.getUserDataValue(
                "ABufferOriginalCommandMap" ).extract< jag::draw::CommandMapPtr >() );
            node.setCommandMap( savedCommands );
        }
        else if( commands == NULL )
            node.setCommandMap( _opaqueCommands );
        else
            node.setCommandMap( jag::draw::CommandMapPtr( new jag::draw::CommandMap(
                *commands + *_opaqueCommands ) ) );
        // Specify the opaque NodeContainer selection callback.
        node.getCollectionCallbacks().remove( _callback );
        node.getCollectionCallbacks().addUnique( _opaqueCallback );
    }
}
void ABuffer::toggleTransparencyEnable( jagSG::Node& node )
{
    const bool currentlyEnabled( node.getCollectionCallbacks().contains( _callback ) );
    setTransparencyEnable( node, !currentlyEnabled );
}
void ABuffer::setOpaqueControls( const jag::draw::CommandMapPtr& opaqueCommands, const unsigned int opaqueContainer )
{
    if( _callback == NULL )
        internalInit();

    _opaqueCommands = opaqueCommands;
    _opaqueContainer = opaqueContainer;
    _opaqueCallback->setContainer( _opaqueContainer );
}
unsigned int ABuffer::getStartContainer() const
{
    return( _startContainer );
}
void ABuffer::getABufferControls( jag::draw::CommandMapPtr& commands, jagSG::SelectContainerCallbackPtr& callback )
{
    if( _callback == NULL )
        internalInit();
    commands = _commands;
    callback = _callback;
}
void ABuffer::getABufferControls( jagSG::SelectContainerCallbackPtr& callback )
{
    if( _callback == NULL )
        internalInit();
    callback = _callback;
}
unsigned int ABuffer::getTransparentNodeContainer() const
{
    return( getStartContainer() + 1 );
}

void ABuffer::renderFrame( jagSG::CollectionVisitor& collect, jag::draw::DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );
    jag::draw::DrawGraphPtr drawGraph( collect.getDrawGraph() );
    ABufferContext& abc( _abufferCntxt._data[ contextID ] );

    // From jag::draw::DrawGraph::execute():
    drawInfo.startFrame();

    // Draw everything that comes before the ABuffer.
    unsigned int idx( 0 );
    while( idx < _startContainer )
        (*drawGraph)[ idx++ ].execute( drawInfo );

    // Render the ABuffer, possibly multiple times if necessary
    // to ensure proper page pool size.
    jag::draw::DrawNodeContainer& clear( (*drawGraph)[ _startContainer ] );
    jag::draw::DrawNodeContainer& render( (*drawGraph)[ _startContainer + 1 ] );
    jag::draw::DrawNodeContainer& resolve( (*drawGraph)[ _startContainer + 2 ] );
    bool frameOK( false );
    while( !frameOK )
    {
        clear.execute( drawInfo );
        render.execute( drawInfo );
        resolve.execute( drawInfo );

        // managePool() returns true if 0 pixels exceeded the page pool.
        // In that case, the frame is OK and doesn't need to be re-rendered.
        frameOK = abc.managePool();
    }
    idx += 3;

    // Render anything that comes after the ABuffer.
    while( idx < drawGraph->size() )
        (*drawGraph)[ idx++ ].execute( drawInfo );
}

void ABuffer::reshape( const int w, const int h )
{
    _width = w;
    _height = h;

    _defaultFBO->setViewport( 0, 0, _width, _height );

    // TBD Actually need to store w and h possibly different per context.
    // There's no guarantee all displays will be the same size.
    BOOST_FOREACH( ABufferContext& data, _abufferCntxt._data )
    {
        data._texWidth = w;
        data._texHeight = h;
        data._needsInit = true;
    }
}

void ABuffer::internalInit()
{
#define __READ_UTIL( _RESULT, _NAME, _LOG ) \
    { \
        boost::any anyTemp( jag::disk::read( std::string(_NAME) ) ); \
        try { \
            _RESULT = boost::any_cast< jag::draw::ShaderPtr >( anyTemp ); \
        } \
        catch( boost::bad_any_cast bac ) \
        { \
            bac.what(); \
        } \
        if( _RESULT == NULL ) \
        { \
            JAG3D_FATAL_STATIC( _LOG, "Can't load \"" + std::string(_NAME) + "\"." ); \
            return; \
        } \
        _RESULT->setMaxContexts( _numContexts ); \
    }

    {
        jag::draw::ShaderPtr vs; __READ_UTIL( vs, "abufferClear.vert", "jag.util.abuf" );
        jag::draw::ShaderPtr fs; __READ_UTIL( fs, "abufferClear.frag", "jag.util.abuf" );

        if( _clearProgram == NULL )
            _clearProgram.reset( new jag::draw::Program() );
        else
            _clearProgram->detachAllShaders();
        _clearProgram->attachShader( vs );
        _clearProgram->attachShader( fs );
        _clearProgram->addVertexAttribAlias( "vertexPos", "vertex" );
    }
    // Create program for rendering into the abuffer.
    {
        jag::draw::ShaderPtr vs; __READ_UTIL( vs, "abufferRender.vert", "jag.util.abuf" );
        jag::draw::ShaderPtr fs; __READ_UTIL( fs, "abufferRender.frag", "jag.util.abuf" );

        if( _renderProgram == NULL )
            _renderProgram.reset( new jag::draw::Program() );
        else
            _renderProgram->detachAllShaders();
        _renderProgram->attachShader( vs );
        _renderProgram->attachShader( fs );
        _renderProgram->addVertexAttribAlias( "vertexPos", "vertex" );
        _renderProgram->addVertexAttribAlias( "vertexNormal", "normal" );
        _renderProgram->addUniformAlias( "projectionMat", "jagProjMatrix" );
        _renderProgram->addUniformAlias( "modelViewMat", "jagModelViewMatrix" );
        _renderProgram->addUniformAlias( "modelViewMatIT", "jagModelViewInvTrans4Matrix" );
    }
    {
        jag::draw::ShaderPtr vs; __READ_UTIL( vs, "abufferResolve.vert", "jag.util.abuf" );
        jag::draw::ShaderPtr fs; __READ_UTIL( fs, "abufferResolve.frag", "jag.util.abuf" );

        fs->insertSourceString( shaderResolveParameters() );

        if( _resolveProgram == NULL )
            _resolveProgram.reset( new jag::draw::Program() );
        else
            _resolveProgram->detachAllShaders();
        _resolveProgram->attachShader( vs );
        _resolveProgram->attachShader( fs );
        _resolveProgram->addVertexAttribAlias( "vertexPos", "vertex" );
    }
#undef __READ_UTIL

    // Create default FBO. First NodeContainer renders to _opaqueFBO,
    // but other NodeContainers render directly to the window.
    if( _defaultFBO == NULL )
        _defaultFBO.reset( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _defaultFBO->setViewport( 0, 0, _width, _height );
    _defaultFBO->setClear( 0 ); // No need to clear anything.


    // This is the CommandMap that the app must attach to any transparent geometry.
    {
        if( _commands == NULL )
            _commands.reset( new jag::draw::CommandMap() );
        _commands->insert( _renderProgram );
        _commands->insert( _defaultFBO );

        jag::draw::TextureSetPtr texSet( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );
        (*texSet)[ GL_TEXTURE13 ] = _depthBuffer;
        _commands->insert( texSet );
        jag::draw::UniformPtr sampler( jag::draw::UniformPtr( new jag::draw::Uniform( "depthBuffer", GL_SAMPLER_2D, 13 ) ) );
        jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
        usp->insert( sampler );
        _commands->insert( usp );
    }

    // This is the NodeContainer selection callback that the app must attach to any transparent geometry.
    // ABuffer container is container the next container after _startContainer.
    if( _callback == NULL )
        _callback.reset( new jagSG::SelectContainerCallback() );
    _callback->setContainer( _startContainer + 1 );

    // Opaque container, for restoring a node to opaque status.
    if( _opaqueCallback == NULL )
        _opaqueCallback.reset( new jagSG::SelectContainerCallback() );
    _opaqueCallback->setContainer( _opaqueContainer );
}

std::string ABuffer::shaderResolveParameters() const
{
    std::ostringstream alpha;
    alpha << std::fixed << std::setw( 7 ) << std::setprecision( 4 ) << _fragmentAlpha << "f";
    std::ostringstream maxfrags;
    maxfrags << _maxFragments;
    const std::string on( "1" );
    const std::string off( "0" );
    const std::string eoln( "\n" );
    return( std::string(

        // Resolve method
        std::string( "#define RESOLVE_GELLY " ) +
            ( ( _resolveMethod == RESOLVE_GELLY ) ? on : off ) + eoln +
        std::string( "#define RESOLVE_ALPHA_BLEND " ) +
            ( ( _resolveMethod == RESOLVE_ALPHA_BLEND ) ? on : off ) + eoln +
        std::string( "#define RESOLVE_ALPHA_BLEND_CAD " ) +
            ( ( _resolveMethod == RESOLVE_ALPHA_BLEND_CAD ) ? on : off ) + eoln +
        
        // Fragment slpha value
        std::string( "#define FRAGMENT_ALPHA " ) + alpha.str() + eoln +

        // Maximum fragments per pixel
        std::string( "#define ABUFFER_SIZE " ) + maxfrags.str() + eoln +

        // Enable secondary color buffer
        std::string( "#define USE_SECONDARY_COLOR_BUFFER " ) +
            ( _secondaryEnable ? on : off ) + eoln +
        eoln
    ) );
}


unsigned int ABuffer::getRequiredMatrixUniforms()
{
    return(
        jag::base::TransformD::PROJ |
        jag::base::TransformD::MODEL_VIEW_PROJ |
        jag::base::TransformD::MODEL_VIEW |
        jag::base::TransformD::MODEL_VIEW_INV_TRANS |
        jag::base::TransformD::MODEL_VIEW_INV_TRANS_4
        );
}

void ABuffer::setResolveMethod( const ResolveMethod resolveMethod )
{
    JAG3D_DEBUG( "Resolve method: " + resolveMethodToString( resolveMethod ) );

    if( _resolveMethod == resolveMethod )
        return;
    _resolveMethod = resolveMethod;

    // Rebuild shaders.
    internalInit();
}
ABuffer::ResolveMethod ABuffer::getResolveMethod() const
{
    return( _resolveMethod );
}

std::string ABuffer::resolveMethodToString( const ResolveMethod& resolveMethod )
{
    switch( resolveMethod )
    {
    case RESOLVE_GELLY: return( "RESOLVE_GELLY" ); break;
    case RESOLVE_ALPHA_BLEND: return( "RESOLVE_ALPHA_BLEND" ); break;
    case RESOLVE_ALPHA_BLEND_CAD: return( "RESOLVE_ALPHA_BLEND_CAD" ); break;
    default: return( "UNSPECIFIED" ); break;
    }
}
ABuffer::ResolveMethod ABuffer::stringToResolveMethod( const std::string& resolveMethod )
{
    if( resolveMethod == "RESOLVE_GELLY" )
        return( RESOLVE_GELLY );
    else if( resolveMethod == "RESOLVE_ALPHA_BLEND" )
        return( RESOLVE_ALPHA_BLEND );
    else if( resolveMethod == "RESOLVE_ALPHA_BLEND_CAD" )
        return( RESOLVE_ALPHA_BLEND_CAD );
    else
        return( UNSPECIFIED );
}

void ABuffer::setFragmentAlpha( const float fragmentAlpha )
{
    if( _fragmentAlpha == fragmentAlpha )
        return;
    _fragmentAlpha = fragmentAlpha;

    if( _fragmentAlpha < 0.f ) _fragmentAlpha = 0.f;
    else if( _fragmentAlpha > 1.f ) _fragmentAlpha = 1.f;

    // Rebuild shaders.
    internalInit();
}
float ABuffer::getFragmentAlpha() const
{
    return( _fragmentAlpha );
}

void ABuffer::setMaxFragments( const unsigned int maxFragments )
{
    if( _maxFragments == maxFragments )
        return;
    _maxFragments = maxFragments;

    // Rebuild shaders.
    internalInit();
}
unsigned int ABuffer::getMaxFragments() const
{
    return( _maxFragments );
}

void ABuffer::setSecondaryColorBufferEnable( const bool secondaryEnable )
{
    if( _secondaryEnable == secondaryEnable )
        return;
    _secondaryEnable = secondaryEnable;

    // Rebuild shaders.
    internalInit();
}
bool ABuffer::getSecondaryColorBufferEnable() const
{
    return( _secondaryEnable );
}





#define ABUFFER_PAGE_SIZE 4

ABufferContext::ABufferContext()
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

bool ABufferContext::init()
{
	JAG3D_ERROR_CHECK( "Start of ABufferContext::init()" );

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
    JAG3D_INFO_STATIC( _logName, ostr.str() );

    _needsInit = false;

    return( true );
}

void ABufferContext::assignUniforms( const GLuint prog )
{
    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_abufferPageIdx" ), _pageIdxAddress );
    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_abufferFragCount" ), _fragCountAddress );
    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_semaphore" ), _semaphoreAddress );

    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_sharedPageList" ), _sharedPageListAddress );
    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_sharedLinkList" ), _sharedLinkListAddress );

    glProgramUniformui64NV( prog, glGetUniformLocation( prog, "d_curSharedPage" ), _curSharedPageAddress );

    glProgramUniform1iEXT( prog, glGetUniformLocation( prog, "abufferSharedPoolSize" ), _sharedPoolSize );

    glProgramUniform1iEXT( prog, glGetUniformLocation( prog, "screenWidth" ), _texWidth );
    glProgramUniform1iEXT( prog, glGetUniformLocation( prog, "screenHeight" ), _texHeight );
}

bool ABufferContext::managePool()
{
	GLuint lastFrameNumFrags( 0 );
	if( _queryRequested )
    {
		glGetQueryObjectuiv( _queryID, GL_QUERY_RESULT, &lastFrameNumFrags );
	
		//A fragments is not discarded each time a page fails to be allocated
		if( lastFrameNumFrags > 0 )
        {
            std::ostringstream ostr;
            ostr << lastFrameNumFrags;
            JAG3D_DEBUG_STATIC( _logName, std::string( ostr.str() ) + std::string( " discarded pixels." ) );

			_sharedPoolSize = _sharedPoolSize + ( lastFrameNumFrags / ABUFFER_PAGE_SIZE + 1 ) * ABUFFER_PAGE_SIZE * 2;
            _needsInit = true;
		}
		_queryRequested = false;
	}

	return( lastFrameNumFrags == 0 );
}


// jagUtil
}
