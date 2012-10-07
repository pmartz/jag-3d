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

    jagDraw::DrawablePtr makeSceneDrawable();

protected:
    jagDraw::DrawableVec _windowVec, _rttVec, _quadVec;
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


jagDraw::DrawablePtr RttDemo::makeSceneDrawable()
{
    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );

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

    jagDraw::ProgramPtr prog( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );
    drawable->addDrawablePrep( prog );


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

    jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
    vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayCommand::Vertex );
    vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );
    drawable->addDrawablePrep( vaop );

    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_LINES, 0, 4 ) );
    drawable->addDrawCommand( drawArrays );

    return( drawable );
}

bool RttDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );


    // Drawable for rendering lines, same Drawable whether we
    // render to window or to texture.
    jagDraw::DrawablePtr linesDrawable( makeSceneDrawable() );

    // DrawableVec for rendering to window
    _windowVec.push_back( linesDrawable );


    // Now set up to render the lines to a texture.

    // Create a texture to render into.
    jagDraw::ImagePtr image( new jagDraw::Image() );
    image->set( 0, GL_RGBA, _texWidth, _texHeight, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        jagBase::BufferPtr( (jagBase::Buffer*) NULL ) );
    jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image ) );

    // Create and FBO and attach the texture.
    _textureFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _textureFBO->addAttachment( GL_COLOR_ATTACHMENT0, tex );

    // Create an FBO for the default framebuffer (the window)
    _defaultFBO = jagDraw::FramebufferPtr( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );


    // Render the lines first.
    _rttVec.push_back( linesDrawable );

    // Now set up for drawing a textured quad.
    {
        jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );

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
        drawable->addDrawablePrep( prog );


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
        drawable->addDrawablePrep( vaop );

        jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
        drawable->addDrawCommand( drawArrays );


        // This is the texture we rendered the lines into.
        drawable->addDrawablePrep( tex );

        // And a uniform for the sampler / texture unit.
        jagDraw::UniformPtr textureUniform( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );
        drawable->addDrawablePrep( textureUniform );

        _quadVec.push_back( drawable );
    }


    // Tell all Jag3D objects how many contexts to expect.
    _textureFBO->setMaxContexts( numContexts );
    _defaultFBO->setMaxContexts( numContexts );
    BOOST_FOREACH( const jagDraw::DrawableVec::value_type& dp, _windowVec )
    {
        dp->setMaxContexts( numContexts );
    }
    BOOST_FOREACH( const jagDraw::DrawableVec::value_type& dp, _rttVec )
    {
        dp->setMaxContexts( numContexts );
    }
    BOOST_FOREACH( const jagDraw::DrawableVec::value_type& dp, _quadVec )
    {
        dp->setMaxContexts( numContexts );
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

    // drawInfo stores the contextID (used by many Jag3D objects to
    // look up their object ID), and the current Program
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

    // Render all Drawables.
    if( ( ( ++_frames / 100 ) & 0x1 ) == 0 )
    {
        // Render lines to the window.

        glClear( GL_COLOR_BUFFER_BIT );

        BOOST_FOREACH( jagDraw::DrawablePtr dp, _windowVec )
        {
            (*(dp))( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Line rendering" );
    }
    else
    {
        // Render lines to the FBO, then display the texture with a quad.

        // Bind the texture FBO
        (*_textureFBO)( drawInfo );
        // Change the viewport
        glViewport( 0, 0, _texWidth, _texHeight );

        glClear( GL_COLOR_BUFFER_BIT );

        BOOST_FOREACH( jagDraw::DrawablePtr dp, _rttVec )
        {
            (*(dp))( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Render to FBO" );

        // Bind the default FBO
        (*_defaultFBO)( drawInfo );
        // Restore the viewport
        glViewport( 0, 0, 300, 300 );
        // No clear is necessary

        BOOST_FOREACH( jagDraw::DrawablePtr dp, _quadVec )
        {
            (*(dp))( drawInfo );
        }
        JAG3D_ERROR_CHECK( "Display texture on quad" );
    }

    glFlush ();
    JAG3D_ERROR_CHECK( "RttDemo display()" );

    return( true );
}
