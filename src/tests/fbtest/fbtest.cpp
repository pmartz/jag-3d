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

#include <jagDraw/Common.h>

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
    jagDraw::DrawablePtr makeSceneDrawable(
            jagDraw::ProgramPtr& prog, jagDraw::VertexArrayObjectPtr& vaop );


    jagDraw::NodeContainer _windowNodes, _rttNodes, _quadNodes;
    jagDraw::FramebufferPtr _textureFBO, _defaultFBO;

    const GLsizei _texWidth, _texHeight;
    unsigned int _frames;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioNotice, jagBase::Log::Console );
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, "jag.draw.tex" );
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioDebug, "jag.draw.fbo" );

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
    vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayObject::Vertex );


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
    _defaultFBO->setUserDataName( "defaultFBO" );
    _defaultFBO->setClear( 0 );
    _defaultFBO->setViewport( 0, 0, 300, 300 );

    jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
    commands->insert( prog );
    commands->insert( vaop );
    commands->insert( _defaultFBO );
    jagDraw::DrawNodePtr drawNode( new jagDraw::Node( commands ) );
    drawNode->addDrawable( linesDrawable );
    _windowNodes.push_back( drawNode );


    // Now set up to render the lines to a texture.

    // Create a texture to render into.
    jagDraw::ImagePtr image( new jagDraw::Image() );
    image->set( 0, GL_RGBA, _texWidth, _texHeight, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    tex->getSampler()->getSamplerState()->_minFilter = GL_LINEAR;
    tex->setUserDataName( "texture0" );

    //create the second texture to render into
    jagDraw::TexturePtr tex2( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    tex2->getSampler()->getSamplerState()->_minFilter = GL_LINEAR;
    tex2->setUserDataName( "texture1" );

    // Create an FBO and attach textureS.
    _textureFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _textureFBO->setUserDataName( "textureFBO" );
    _textureFBO->setClearColor( 0., 0., 0., 1. );
    _textureFBO->setClear( GL_COLOR_BUFFER_BIT );
    _textureFBO->setViewport( 0, 0, _texWidth, _texHeight );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT0, tex );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT1, tex2 );
    

    // Render the lines first.
    jagDraw::CommandMapPtr rttCommands( jagDraw::CommandMapPtr( new jagDraw::CommandMap( *commands ) ) );
    rttCommands->insert( _textureFBO );
    jagDraw::DrawNodePtr rttDrawNode( new jagDraw::Node( rttCommands ) );
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
        jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
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
        vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iTexCoord );

        jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
        jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
        drawable->addDrawCommand( drawArrays );

        // And a uniform for each sampler / texture unit.
        jagDraw::UniformPtr textureUniform( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );
        jagDraw::UniformPtr textureUniform2( new jagDraw::Uniform( "texture2", GL_SAMPLER_2D, (GLint)1 ) );


        jagDraw::UniformSetPtr uniformSet( jagDraw::UniformSetPtr( new jagDraw::UniformSet() ) );
        (*uniformSet)[ textureUniform->getNameHash() ] = textureUniform;
        (*uniformSet)[ textureUniform2->getNameHash() ] = textureUniform2;

        jagDraw::TextureSetPtr textureSet( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );

        //add both textures to the texture set
        (*textureSet)[ GL_TEXTURE0 ] = tex2;
        (*textureSet)[ GL_TEXTURE1 ] = tex;

        jagDraw::CommandMapPtr quadCommands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
        quadCommands->insert( prog );
        quadCommands->insert( vaop );
        quadCommands->insert( _defaultFBO );
        quadCommands->insert( textureSet );
        quadCommands->insert( uniformSet );
        jagDraw::DrawNodePtr quadDrawNode( new jagDraw::Node( quadCommands ) );
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
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "RttDemo init()" );
    return( true );
}

bool RttDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

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
