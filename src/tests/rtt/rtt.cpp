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

#include <jagDraw/DrawCommon.h>

#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

#include <boost/program_options/options_description.hpp>
#include <boost/foreach.hpp>
#include <string>


using namespace std;
namespace bpo = boost::program_options;


class RttDemo : public DemoInterface
{
public:
    RttDemo()
      : DemoInterface( "jag.demo.texture" ),
        _frames( 0 ),
        _texWidth( 80 ),
        _texHeight( 80 )
    {
        _continuousRedraw = true;
    }
    virtual ~RttDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagDraw::DrawablePtr makeSceneDrawable(
            jagDraw::ProgramPtr& prog, jagDraw::VertexArrayObjectPtr& vaop );


    jagDraw::DrawNodeSimpleVec _windowNodes, _rttNodes, _quadNodes;
    jagDraw::FramebufferPtr _textureFBO, _defaultFBO;

    const GLsizei _texWidth, _texHeight;
    unsigned int _frames;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioNotice, jagBase::Log::Console );
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, "jag.draw.tex" );
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, "jag.draw.fbo" );

    return( new RttDemo );
}


jagDraw::DrawablePtr RttDemo::makeSceneDrawable(
        jagDraw::ProgramPtr& prog, jagDraw::VertexArrayObjectPtr& vaop )
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
    jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vertShader ) );

    const char* fragShader =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
        "out vec4 colorOut; \n"
        "void main() { \n"
        "    colorOut = vec4( 1., 1., 1., 1. ); \n"
        "}";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fragShader ) );

    prog = jagDraw::ProgramPtr( new jagDraw::Program() );
    prog->attachShader( vs );
    prog->attachShader( fs );


    const float z = .5f;
    float vertices[] = {
        -1., 0., z,
        0., -1., z,
        0., 1., z,
        1., 0., z };
    jagBase::BufferPtr ibp( new jagBase::Buffer( sizeof( vertices ), (void*)vertices ) );
    jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );

    vaop = jagDraw::VertexArrayObjectPtr( new jagDraw::VertexArrayObject() );
    vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayCommand::Vertex );
    vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_LINES, 0, 4 ) );
    drawable->addDrawCommand( drawArrays );

    return( drawable );
}

bool RttDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );


    // Drawable for rendering lines, same Drawable whether we
    // render to window or to texture.
    jagDraw::ProgramPtr prog( new jagDraw::Program() );
    jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject() );
    jagDraw::DrawablePtr linesDrawable( makeSceneDrawable( prog, vaop ) );


    // Create an FBO for the default framebuffer (the window)
    _defaultFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _defaultFBO->setViewport( 0, 0, 300, 300 );

    jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
    commands->insert( prog );
    commands->insert( vaop );
    commands->insert( _defaultFBO );
    jagDraw::DrawNode drawNode( commands );
    drawNode.addDrawable( linesDrawable );
    _windowNodes.push_back( drawNode );


    // Now set up to render the lines to a texture.

    // Create a texture to render into.
    jagDraw::ImagePtr image( new jagDraw::Image() );
    image->set( 0, GL_RGBA, _texWidth, _texHeight, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        jagBase::BufferPtr( (jagBase::Buffer*) NULL ) );
    jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image ) );

    // Create an FBO and attach the texture.
    _textureFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _textureFBO->setViewport( 0, 0, _texWidth, _texHeight );
    _textureFBO->setClear( GL_COLOR_BUFFER_BIT );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT0, tex );

    // Render the lines first.
    jagDraw::CommandMapPtr rttCommands( jagDraw::CommandMapPtr( new jagDraw::CommandMap( *commands ) ) );
    rttCommands->insert( _textureFBO );
    jagDraw::DrawNode rttDrawNode( rttCommands );
    rttDrawNode.addDrawable( linesDrawable );
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
        jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
        vs->addSourceString( std::string( vertSource ) );

        const char* fragSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
            // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
            "#version 150 \n"
#else
            "#version 400 \n"
#endif
            "uniform sampler2D texture; \n"
            "in vec2 tcOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            //"    colorOut = texture2D( texture, tcOut ); \n"
            "    colorOut = texture2D( texture, tcOut ) + vec4( .5, 0., 0., 0. ); \n"
            //"    colorOut = vec4( tcOut, 0., 1. ); \n"
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fragSource ) );

        jagDraw::ProgramPtr prog( new jagDraw::Program );
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
        jagBase::BufferPtr ibp( new jagBase::Buffer( sizeof( vertices ), (void*)vertices ) );
        jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        const GLsizei stride = sizeof( GLfloat ) * 5;
        jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
        jagDraw::VertexAttribPtr iTexCoord( new jagDraw::VertexAttrib(
            "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayCommand::Vertex );
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );
        vaop->addVertexArrayCommand( iTexCoord );

        jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
        jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
        drawable->addDrawCommand( drawArrays );

        // And a uniform for the sampler / texture unit.
        jagDraw::UniformPtr textureUniform( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );



        jagDraw::UniformSetPtr uniformSet( jagDraw::UniformSetPtr( new jagDraw::UniformSet() ) );
        (*uniformSet)[ textureUniform->getNameHash() ] = textureUniform;

        jagDraw::TextureSetPtr textureSet( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
        (*textureSet)[ GL_TEXTURE0 ] = tex;

        jagDraw::CommandMapPtr quadCommands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
        quadCommands->insert( prog );
        quadCommands->insert( vaop );
        quadCommands->insert( _defaultFBO );
        quadCommands->insert( textureSet );
        quadCommands->insert( uniformSet );
        jagDraw::DrawNode quadDrawNode( quadCommands );
        quadDrawNode.addDrawable( drawable );
        _quadNodes.push_back( quadDrawNode );
    }


    // Tell all Jag3D objects how many contexts to expect.
    _textureFBO->setMaxContexts( numContexts );
    _defaultFBO->setMaxContexts( numContexts );
    BOOST_FOREACH( jagDraw::DrawNodeSimpleVec::value_type& dp, _windowNodes )
    {
        dp.setMaxContexts( numContexts );
    }
    BOOST_FOREACH( jagDraw::DrawNodeSimpleVec::value_type& dp, _rttNodes )
    {
        dp.setMaxContexts( numContexts );
    }
    BOOST_FOREACH( jagDraw::DrawNodeSimpleVec::value_type& dp, _quadNodes )
    {
        dp.setMaxContexts( numContexts );
    }


    return( true );
}

bool RttDemo::init()
{
    glClearColor( 0.f, 0.f, 1.f, 0.f );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "RttDemo init()" );
    return( true );
}

bool RttDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    if( !getStartupCalled() )
        return( true );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Render all Drawables.
    if( ( ( ++_frames / 100 ) & 0x1 ) == 0 )
    {
        // Render lines to the window.

        glClear( GL_COLOR_BUFFER_BIT );

        BOOST_FOREACH( jagDraw::DrawNode& drawNode, _windowNodes )
        {
            drawNode( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Line rendering" );
    }
    else
    {
        // Render lines to the FBO, then display the texture with a quad.

        BOOST_FOREACH( jagDraw::DrawNode& drawNode, _rttNodes )
        {
            drawNode( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Render to FBO" );

        BOOST_FOREACH( jagDraw::DrawNode& drawNode, _quadNodes )
        {
            drawNode( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Display texture on quad" );
    }

    glFlush();
    JAG3D_ERROR_CHECK( "RttDemo display()" );

    return( true );
}
