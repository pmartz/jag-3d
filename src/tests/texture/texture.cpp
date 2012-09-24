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
#include <Poco/Message.h>
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
    jagDraw::DrawableVec _drawableVec;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    return( new TextureDemo );
}

bool TextureDemo::startup( const unsigned int numContexts )
{
    const char* vShaderSource =
        "#version 130 \n"
        "in vec3 vertex; \n"
        "in vec2 texcoord; \n"
        "out vec2 tcOut; \n"
        "void main() { \n"
        "    gl_Position = vec4( vertex, 1. ); \n"
        "    tcOut = texcoord; \n"
        "}";
    jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vShaderSource ) );

    jagDraw::ShaderProgramPtr prog;
    {
        const char* fShaderSource =
            "#version 130 \n"
            "uniform sampler2D texture; \n"
            "in vec2 tcOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = texture2D( texture, tcOut ); \n"
            //"    colorOut = vec4( tcOut, 0., 1. ); \n"
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        prog = jagDraw::ShaderProgramPtr( new jagDraw::Program );
        prog->attachShader( vs );
        prog->attachShader( fs );
    }

    jagDraw::UniformPtr texture( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
    const float z = .5f;
    typedef std::vector< float > FloatArray;

    const GLsizei stride = sizeof( GLfloat ) * 5;
    jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jagDraw::VertexAttribPtr iColor( new jagDraw::VertexAttrib(
        "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );

    {
        drawable = jagDraw::DrawablePtr( new jagDraw::Drawable() );

        drawable->addDrawablePrep( prog );
        drawable->addDrawablePrep( texture );

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
        drawable->addDrawablePrep( vaop );

        // Load image using jagDisk plugin interface.
        jagDraw::ImagePtr image( (jagDraw::Image*) jagDisk::read( "balloon.jpg" ) );
        jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image ) );
        drawable->addDrawablePrep( tex );

        drawable->addDrawCommand( drawArrays );

        _drawableVec.push_back( drawable );
    }


    // Tell all Jag objects how many contexts to expect.
    BOOST_FOREACH( const jagDraw::DrawableVec::value_type& dp, _drawableVec )
    {
        dp->setMaxContexts( numContexts );
    }


    return( true );
}

bool TextureDemo::init()
{
    glClearColor( 1.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    JAG3D_INFO_STATIC( _logName, msg );

    JAG3D_ERROR_CHECK( "TextureDemo init()" );
    return( true );
}

bool TextureDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    glClear( GL_COLOR_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current Program
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

    // Render all Drawables.
    BOOST_FOREACH( jagDraw::DrawablePtr dp, _drawableVec )
    {
        (*(dp))( drawInfo );
    }
    
    glFlush ();
    JAG3D_ERROR_CHECK( "TextureDemo display()" );

    return( true );
}
