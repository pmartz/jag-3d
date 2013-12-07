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

#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

#include <boost/program_options/options_description.hpp>
#include <boost/foreach.hpp>
#include <string>


using namespace std;
namespace bpo = boost::program_options;


class RttDemo : public DemoInterface
{
public:
    RttDemo()
      : DemoInterface( "jag.demo.rtt" ),
        _texWidth( 80 ),
        _texHeight( 80 ),
        _frames( 0 )
    {
        setContinuousRedraw();
    }
    virtual ~RttDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

protected:
    jag::draw::DrawablePtr makeSceneDrawable(
            jag::draw::ProgramPtr& prog, jag::draw::VertexArrayObjectPtr& vaop );


    jag::draw::DrawNodeContainer _windowNodes, _rttNodes, _quadNodes;
    jag::draw::FramebufferPtr _textureFBO, _defaultFBO;

    const GLsizei _texWidth, _texHeight;
    unsigned int _frames;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioNotice, jag::base::Log::Console );
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioTrace, "jag.draw.tex" );
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioDebug, "jag.draw.fbo" );

    return( new RttDemo );
}


jag::draw::DrawablePtr RttDemo::makeSceneDrawable(
        jag::draw::ProgramPtr& prog, jag::draw::VertexArrayObjectPtr& vaop )
{
    const char* vertShader =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
        "in vec3 vertex; \n"
        "void main() { \n"
        "    gl_Position = vec4( vertex, 1. ); \n"
        "}";
    jag::draw::ShaderPtr vs( new jag::draw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vertShader ) );


    //this now uses multiple inputs. This may not work at version 150 where it may have to be ifdeffed to use gl_Fragdata[]
    const char* fragShader =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
        "layout (location = 0) out vec4 colorOut; \n"
        "layout (location = 1) out vec4 colorOut2; \n"
        "void main() { \n"
        "    colorOut = vec4( 1., .0, .0, 1.0 ); \n"
        "    colorOut2 = vec4( .0, 1., 1., 1.0 ); \n"
        "}";
    jag::draw::ShaderPtr fs( new jag::draw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fragShader ) );

    prog = jag::draw::ProgramPtr( new jag::draw::Program() );
    prog->attachShader( vs );
    prog->attachShader( fs );


    const float z = .5f;
    float vertices[] = {
        -1., 0., z,
        0., -1., z,
        0., 1., z,
        1., 0., z };
    jag::base::BufferPtr ibp( new jag::base::Buffer( sizeof( vertices ), (void*)vertices ) );
    jag::draw::BufferObjectPtr ibop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    jag::draw::VertexAttribPtr iVerts( new jag::draw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );

    vaop = jag::draw::VertexArrayObjectPtr( new jag::draw::VertexArrayObject() );
    vaop->addVertexArrayCommand( ibop, jag::draw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( iVerts, jag::draw::VertexArrayObject::Vertex );


    jag::draw::DrawablePtr drawable( new jag::draw::Drawable() );
    jag::draw::DrawArraysPtr drawArrays( new jag::draw::DrawArrays( GL_LINES, 0, 4 ) );
    drawable->addDrawCommand( drawArrays );

    return( drawable );
}

bool RttDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );


    // Drawable for rendering lines, same Drawable whether we
    // render to window or to texture.
    jag::draw::ProgramPtr prog( new jag::draw::Program() );
    jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject() );
    jag::draw::DrawablePtr linesDrawable( makeSceneDrawable( prog, vaop ) );


    // Create an FBO for the default framebuffer (the window)
    _defaultFBO = jag::draw::FramebufferPtr( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _defaultFBO->setUserDataName( "defaultFBO" );
    _defaultFBO->setClear( 0 );
    _defaultFBO->setViewport( 0, 0, 300, 300 );

    jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new jag::draw::CommandMap() ) );
    commands->insert( prog );
    commands->insert( vaop );
    commands->insert( _defaultFBO );
    jag::draw::DrawNodePtr drawNode( new jag::draw::DrawNode( commands ) );
    drawNode->addDrawable( linesDrawable );
    _windowNodes.push_back( drawNode );


    // Now set up to render the lines to a texture.

    // Create a texture to render into.
    jag::draw::ImagePtr image( new jag::draw::Image() );
    image->set( 0, GL_RGBA, _texWidth, _texHeight, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    jag::draw::TexturePtr tex( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    tex->getSampler()->getSamplerState()->_minFilter = GL_LINEAR;
    tex->setUserDataName( "texture0" );

    //create the second texture to render into
    jag::draw::TexturePtr tex2( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    tex2->getSampler()->getSamplerState()->_minFilter = GL_LINEAR;
    tex2->setUserDataName( "texture1" );

    // Create an FBO and attach textureS.
    _textureFBO = jag::draw::FramebufferPtr( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _textureFBO->setUserDataName( "textureFBO" );
    _textureFBO->setClearColor( 0., 0., 0., 1. );
    _textureFBO->setClear( GL_COLOR_BUFFER_BIT );
    _textureFBO->setViewport( 0, 0, _texWidth, _texHeight );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT0, tex );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT1, tex2 );
    

    // Render the lines first.
    jag::draw::CommandMapPtr rttCommands( jag::draw::CommandMapPtr( new jag::draw::CommandMap( *commands ) ) );
    rttCommands->insert( _textureFBO );
    jag::draw::DrawNodePtr rttDrawNode( new jag::draw::DrawNode( rttCommands ) );
    rttDrawNode->addDrawable( linesDrawable );
    _rttNodes.push_back( rttDrawNode );

    // Now set up for drawing a textured quad.
    {
        const char* vertSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
            // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
            "#version 150 \n"
#else
            "#version 400 \n"
#endif
            "in vec3 vertex; \n"
            "in vec2 texcoord; \n"
            "out vec2 tcOut; \n"
            "void main() { \n"
            "    gl_Position = vec4( vertex, 1. ); \n"
            "    tcOut = texcoord; \n"
            "}";
        jag::draw::ShaderPtr vs( new jag::draw::Shader( GL_VERTEX_SHADER ) );
        vs->addSourceString( std::string( vertSource ) );

        const char* fragSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
            // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
            "#version 150 \n"
#else
            "#version 400 \n"
#endif
            "uniform sampler2D texture, texture2; \n"
            "in vec2 tcOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = texture2D( texture, tcOut ) + texture2D(texture2, tcOut);// + vec4( .5, 0., 0., 0. ); \n"
            //"    colorOut = vec4( tcOut, 0., 1. ); \n"
            "}";
        jag::draw::ShaderPtr fs( new jag::draw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fragSource ) );

        jag::draw::ProgramPtr prog( new jag::draw::Program );
        prog->attachShader( vs );
        prog->attachShader( fs );


        const float z = .5f;
        float vertices[] = {
            -1., -1., z,
            0., 0.,
            1., -1., z,
            1., 0.,
            -1., 1., z,
            0., 1.,
            1., 1., z,
            1., 1. };
        jag::base::BufferPtr ibp( new jag::base::Buffer( sizeof( vertices ), (void*)vertices ) );
        jag::draw::BufferObjectPtr ibop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        const GLsizei stride = sizeof( GLfloat ) * 5;
        jag::draw::VertexAttribPtr iVerts( new jag::draw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
        jag::draw::VertexAttribPtr iTexCoord( new jag::draw::VertexAttrib(
            "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );

        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jag::draw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iVerts, jag::draw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iTexCoord );

        jag::draw::DrawablePtr drawable( new jag::draw::Drawable() );
        jag::draw::DrawArraysPtr drawArrays( new jag::draw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
        drawable->addDrawCommand( drawArrays );

        // And a uniform for each sampler / texture unit.
        jag::draw::UniformPtr textureUniform( new jag::draw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );
        jag::draw::UniformPtr textureUniform2( new jag::draw::Uniform( "texture2", GL_SAMPLER_2D, (GLint)1 ) );


        jag::draw::UniformSetPtr uniformSet( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
        (*uniformSet)[ textureUniform->getNameHash() ] = textureUniform;
        (*uniformSet)[ textureUniform2->getNameHash() ] = textureUniform2;

        jag::draw::TextureSetPtr textureSet( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );

        //add both textures to the texture set
        (*textureSet)[ GL_TEXTURE0 ] = tex2;
        (*textureSet)[ GL_TEXTURE1 ] = tex;

        jag::draw::CommandMapPtr quadCommands( jag::draw::CommandMapPtr( new jag::draw::CommandMap() ) );
        quadCommands->insert( prog );
        quadCommands->insert( vaop );
        quadCommands->insert( _defaultFBO );
        quadCommands->insert( textureSet );
        quadCommands->insert( uniformSet );
        jag::draw::DrawNodePtr quadDrawNode( new jag::draw::DrawNode( quadCommands ) );
        quadDrawNode->addDrawable( drawable );
        _quadNodes.push_back( quadDrawNode );
    }


    // Tell all Jag3D objects how many contexts to expect.
    _textureFBO->setMaxContexts( numContexts );
    _defaultFBO->setMaxContexts( numContexts );
    _windowNodes.setMaxContexts( numContexts );
    _rttNodes.setMaxContexts( numContexts );
    _quadNodes.setMaxContexts( numContexts );


    return( true );
}

bool RttDemo::init()
{
    glDisable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "RttDemo init()" );
    return( true );
}

bool RttDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Render all Drawables.
    if( ( ( ++_frames / 100 ) & 0x1 ) == 0 )
    {
        // Render lines to the window.
        glClearColor( 0.f, 0.f, 1.f, 1. );
        glClear( GL_COLOR_BUFFER_BIT );
        _windowNodes.execute( drawInfo );
    }
    else
    {
        // Render lines to the FBO, then display the texture with a quad.
        _rttNodes.execute( drawInfo );
        _quadNodes.execute( drawInfo );
    }

    glFlush();
    JAG3D_ERROR_CHECK( "RttDemo display()" );

    return( true );
}
