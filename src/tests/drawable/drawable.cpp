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
#include <Poco/Message.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class DrawableDemo : public DemoInterface
{
public:
    DrawableDemo()
      : DemoInterface( "jag3d.demo.drawable" )
    {}
    virtual ~DrawableDemo() {}

    virtual bool startup();
    virtual bool init();
    virtual bool frame();
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagDraw::DrawableList _drawableList;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    return( new DrawableDemo );
}

bool DrawableDemo::startup()
{
    jagBase::Log::instance()->setPriority( 10, jagBase::Log::Console );
    return( true );
}

bool DrawableDemo::init()
{
    // Auto-log the version string.
    jagBase::getVersionString();


    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    JAG3D_INFO_STATIC( _logName, msg );


    glClearColor( 0.f, 0.f, 0.f, 0.f );

    

    const char* vShaderSource =
        "#version 130 \n"
        "uniform bool swizzle; \n"
        "in vec3 vertex; \n"
        "in vec3 color; \n"
        "out vec3 cOut; \n"
        "void main() { \n"
        "    gl_Position = vec4( vertex, 1. ); \n"
        "    if( swizzle ) \n"
        "        cOut = color.bgr; \n"
        "    else \n"
        "        cOut = color; \n"
        "}";
    jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vShaderSource ) );

    jagDraw::ShaderProgramPtr spp;
    {
        const char* fShaderSource =
            "#version 130 \n"
            "in vec3 cOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( cOut, 1. ); \n"
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        spp = jagDraw::ShaderProgramPtr( new jagDraw::ShaderProgram );
        spp->attachShader( vs );
        spp->attachShader( fs );
    }

    jagDraw::ShaderProgramPtr spp2;
    {
        const char* fShaderSource =
            "#version 130 \n"
            "uniform float scale; \n"
            "in vec3 cOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( cOut * scale, 1. ); \n"
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        spp2 = jagDraw::ShaderProgramPtr( new jagDraw::ShaderProgram );
        spp2->attachShader( vs );
        spp2->attachShader( fs );
    }

    jagDraw::UniformPtr swizzleOff( new jagDraw::Uniform( "swizzle", false ) );
    jagDraw::UniformPtr swizzleOn( new jagDraw::Uniform( "swizzle", true ) );
    jagDraw::UniformPtr scale( new jagDraw::Uniform( "scale", 0.75f ) );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
    const float z = .5f;
    typedef std::vector< gmtl::Point3f > Point3fArray;

    // Define first drawable: tri strip on the left.
    {
        drawable->addDrawablePrep( spp );
        drawable->addDrawablePrep( swizzleOff );

        Point3fArray v3fa;
        v3fa.push_back( gmtl::Point3f( -.9f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.7f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.4f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.9f, .9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, .9f, z ) );
        jagBase::BufferPtr vbp( new jagBase::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );
        jagDraw::BufferObjectPtr vbop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, vbp ) );
        drawable->addVertexArrayCommand( vbop, jagDraw::VertexArrayCommand::Vertex );

        jagDraw::VertexAttribPtr verts( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        drawable->addVertexArrayCommand( verts, jagDraw::VertexArrayCommand::Vertex );

        Point3fArray c3fa;
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jagBase::BufferPtr cbp( new jagBase::Buffer( c3fa.size() * sizeof( gmtl::Point3f ), (void*)&c3fa[0] ) );
        jagDraw::BufferObjectPtr cbop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, cbp ) );
        drawable->addVertexArrayCommand( cbop );

        jagDraw::VertexAttribPtr color( new jagDraw::VertexAttrib(
            "color", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        drawable->addVertexArrayCommand( color );

        typedef std::vector< GLubyte > GLubyteArray;
        GLubyteArray elements;
        unsigned int idx;
        for( idx=0; idx<6; idx++ )
            elements.push_back( idx );
        jagBase::BufferPtr elbp( new jagBase::Buffer( elements.size() * sizeof( GLubyte ), (void*)&elements[0] ) );
        jagDraw::BufferObjectPtr elbop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
        drawable->addVertexArrayCommand( elbop );

        jagDraw::DrawElementsPtr drawElements( new jagDraw::DrawElements( GL_TRIANGLE_STRIP, elements.size(), GL_UNSIGNED_BYTE, 0 ) );
        drawable->addDrawCommand( drawElements );

        _drawableList.push_back( drawable );
    }

    // Define elements shared by second and third drawables.
    const GLsizei stride = sizeof( GLfloat ) * 3 * 2;
    jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jagDraw::VertexAttribPtr iColor( new jagDraw::VertexAttrib(
        "color", 3, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 6 ) );

    // Define second drawable (middle)
    {
        drawable = jagDraw::DrawablePtr( new jagDraw::Drawable() );

        drawable->addDrawablePrep( swizzleOn );

        Point3fArray i3fa;
        i3fa.push_back( gmtl::Point3f( -.3f, -.9f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        i3fa.push_back( gmtl::Point3f( 0.f, -.9f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        i3fa.push_back( gmtl::Point3f( -.1f, 0.f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( .2f, 0.f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( -.3f, .9f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( 0.f, .9f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jagBase::BufferPtr ibp( new jagBase::Buffer( i3fa.size() * sizeof( gmtl::Point3f ), (void*)&i3fa[0] ) );
        jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );
        drawable->addVertexArrayCommand( ibop, jagDraw::VertexArrayCommand::Vertex );

        drawable->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );
        drawable->addVertexArrayCommand( iColor );

        drawable->addDrawCommand( drawArrays );

        _drawableList.push_back( drawable );
    }

    // Define third drawable (on the right)
    {
        drawable = jagDraw::DrawablePtr( new jagDraw::Drawable() );

        drawable->addDrawablePrep( spp2 );
        drawable->addDrawablePrep( scale );

        Point3fArray i3fa;
        i3fa.push_back( gmtl::Point3f( .3f, -.9f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        i3fa.push_back( gmtl::Point3f( .6f, -.9f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        i3fa.push_back( gmtl::Point3f( .5f, 0.f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( .8f, 0.f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( .3f, .9f, z ) );
            i3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        i3fa.push_back( gmtl::Point3f( .6f, .9f, z ) );
            i3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jagBase::BufferPtr ibp( new jagBase::Buffer( i3fa.size() * sizeof( gmtl::Point3f ), (void*)&i3fa[0] ) );
        jagDraw::BufferObjectPtr ibop2( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        // Bind the GL_ARRAY_BUFFER for interleaved vertices and colors
        // (different from _ibop dur to vertex positions).
        vaop->addVertexArrayCommand( ibop2, jagDraw::VertexArrayCommand::Vertex );
        // Enable and specify the "vertex" vertex attrib.
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayCommand::Vertex );
        // Enable and specify the "color" vertex attrib.
        vaop->addVertexArrayCommand( iColor );

        // A DrawCommand to unbind the VAO after we're done drawing.
        jagDraw::VertexArrayObjectUnbindPtr vaoUnbind( new jagDraw::VertexArrayObjectUnbind() );

        drawable->addVertexArrayCommand( vaop );

        drawable->addDrawCommand( drawArrays );
        drawable->addDrawCommand( vaoUnbind );

        _drawableList.push_back( drawable );
    }

    JAG_ERROR_CHECK( "DrawableDemo init()" );

    return( true );
}

bool DrawableDemo::frame()
{
    glClear( GL_COLOR_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current ShaderProgram
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = 0;

    // Render all Drawables.
    BOOST_FOREACH( jagDraw::DrawablePtr dp, _drawableList )
    {
        (*(dp))( drawInfo );
    }
    
    glFlush ();
    JAG_ERROR_CHECK( "DrawableDemo display()" );

    return( true );
}
