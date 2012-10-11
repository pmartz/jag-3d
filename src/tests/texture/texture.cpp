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
#include <jagDraw/Texture.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <jagDisk/ReadWrite.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class TextureDemo : public DemoInterface
{
public:
    TextureDemo()
      : DemoInterface( "jag.demo.texture" )
    {}
    virtual ~TextureDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagDraw::DrawNodeSimpleVec _nodes;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioDebug, jagBase::Log::Console );

    return( new TextureDemo );
}

bool TextureDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );


    const char* vShaderSource =
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
    vs->addSourceString( std::string( vShaderSource ) );

    const char* fShaderSource =
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
        "    colorOut = texture2D( texture, tcOut ); \n"
        //"    colorOut = vec4( tcOut, 0., 1. ); \n"
        "}";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program ) );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( new jagDraw::CommandMap() );
    commands->insert( prog );


    jagDraw::UniformPtr textureUniform( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );
    jagDraw::UniformSetPtr uniformSet( new jagDraw::UniformSet() );
    (*uniformSet)[ textureUniform->getNameHash() ] = textureUniform;
    commands->insert( uniformSet );

    // Load image using jagDisk plugin interface.
    jagDraw::ImagePtr image( (jagDraw::Image*) jagDisk::read( "balloon.jpg" ) );
    jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image ) );
    jagDraw::TextureSetPtr texSet( new jagDraw::TextureSet() );
    (*texSet)[ GL_TEXTURE0 ] = tex;
    commands->insert( texSet );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );

    const GLsizei stride = sizeof( GLfloat ) * 5;
    jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jagDraw::VertexAttribPtr iColor( new jagDraw::VertexAttrib(
        "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
    drawable->addDrawCommand( drawArrays );

    {
        const float z = .5f;
        typedef std::vector< float > FloatArray;
        FloatArray i3fa;
        i3fa.push_back( -.9f ); i3fa.push_back( -.9f ); i3fa.push_back( z );
            i3fa.push_back( 0.f ); i3fa.push_back( 0.f );
        i3fa.push_back( .9f ); i3fa.push_back( -.9f ); i3fa.push_back( z );
            i3fa.push_back( 1.f ); i3fa.push_back(  0.f );
        i3fa.push_back( -.9f ); i3fa.push_back( .9f ); i3fa.push_back( z );
            i3fa.push_back( 0.f ); i3fa.push_back( 1.f );
        i3fa.push_back( .9f ); i3fa.push_back( .9f ); i3fa.push_back( z );
            i3fa.push_back( 1.f ); i3fa.push_back( 1.f );
        jagBase::BufferPtr ibp( new jagBase::Buffer( i3fa.size() * sizeof( float ), (void*)&i3fa[0] ) );
        jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayCommand::Vertex );
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );
        vaop->addVertexArrayCommand( iColor );
        commands->insert( vaop );

        jagDraw::DrawNode drawNode( commands );
        drawNode.addDrawable( drawable );
        _nodes.push_back( drawNode );
    }


    // Tell all Jag3D objects how many contexts to expect.
    BOOST_FOREACH( jagDraw::DrawNode& drawNode, _nodes )
    {
        drawNode.setMaxContexts( numContexts );
    }


    return( true );
}

bool TextureDemo::init()
{
    glClearColor( 1.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "TextureDemo init()" );
    return( true );
}


bool TextureDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Render all Drawables.
    BOOST_FOREACH( jagDraw::DrawNode& drawNode, _nodes )
    {
        drawNode.execute( drawInfo );
    }
    
    glFlush();
    JAG3D_ERROR_CHECK( "TextureDemo display()" );

    return( true );
}
