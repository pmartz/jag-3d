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
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class DrawCommandDemo : public DemoInterface
{
public:
    DrawCommandDemo() {}
    virtual ~DrawCommandDemo() {}

    virtual bool init();
    virtual bool frame();
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    Poco::Logger* _logger;

    jagDraw::BufferObjectPtr _vbop;
    jagDraw::BufferObjectPtr _cbop;
    jagDraw::BufferObjectPtr _ibop;
    jagDraw::BufferObjectPtr _ibop2;
    jagDraw::ShaderProgramPtr _spp;
    jagDraw::VertexAttribPtr _verts, _color;
    jagDraw::VertexAttribPtr _iVerts, _iColor;
    jagDraw::VertexArrayObjectPtr _vaop;

    jagDraw::DrawArraysPtr _drawArrays;

    jagDraw::BufferObjectPtr _elbop;
    jagDraw::DrawElementsPtr _drawElements;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    return( new DrawCommandDemo );
}

bool DrawCommandDemo::init()
{
    _logger = Poco::Logger::has( "jag3d.demo.drawcommand" );
    if( _logger == NULL )
    {
        _logger = &( Poco::Logger::create( "jag3d.demo.drawcommand",
                (Poco::Channel*)( jagBase::Log::instance()->getConsole() ), Poco::Message::PRIO_INFORMATION ) );
    }

    _logger->information( jagBase::getVersionString() );


    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    _logger->information( msg );


    glClearColor( 0.f, 0.f, 0.f, 0.f );

    const float z = .5f;
    typedef std::vector< gmtl::Point3f > Point3fArray;
    {
        Point3fArray v3fa;
        v3fa.push_back( gmtl::Point3f( -.9f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.7f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.4f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.9f, .9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, .9f, z ) );
        jagBase::BufferPtr vbp( new jagBase::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );
        _vbop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, vbp ) );

        Point3fArray c3fa;
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jagBase::BufferPtr cbp( new jagBase::Buffer( c3fa.size() * sizeof( gmtl::Point3f ), (void*)&c3fa[0] ) );
        _cbop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, cbp ) );

        _verts = jagDraw::VertexAttribPtr( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        _color = jagDraw::VertexAttribPtr( new jagDraw::VertexAttrib(
            "color", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
    }
    {
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
        _ibop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        const GLsizei stride = sizeof( GLfloat ) * 3 * 2;
        _iVerts = jagDraw::VertexAttribPtr( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
        _iColor = jagDraw::VertexAttribPtr( new jagDraw::VertexAttrib(
            "color", 3, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    }
    {
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
        _ibop2 = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );
    }

    _vaop = jagDraw::VertexArrayObjectPtr( new jagDraw::VertexArrayObject );
    {
        // Bind the GL_ARRAY_BUFFER for interleaved vertices and colors
        // (different from _ibop dur to vertex positions).
        _vaop->addVertexArrayCommand( _ibop2 );
        // Enable and specify the "vertex" vertex attrib.
        _vaop->addVertexArrayCommand( _iVerts );
        // Enable and specify the "color" vertex attrib.
        _vaop->addVertexArrayCommand( _iColor );
    }

    _drawArrays = jagDraw::DrawArraysPtr( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 6 ) );

    typedef std::vector< GLubyte > GLubyteArray;
    GLubyteArray elements;
    {
        unsigned int idx;
        for( idx=0; idx<6; idx++ )
            elements.push_back( idx );
        jagBase::BufferPtr elbp( new jagBase::Buffer( elements.size() * sizeof( GLubyte ), (void*)&elements[0] ) );
        _elbop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
    }
    _drawElements = jagDraw::DrawElementsPtr( new jagDraw::DrawElements( GL_TRIANGLE_STRIP, elements.size(), GL_UNSIGNED_BYTE, 0 ) );

    {
        const char* vShaderSource =
            "#version 130 \n"
            "in vec3 vertex; \n"
            "in vec3 color; \n"
            "out vec3 cOut; \n"
            "void main() { \n"
            "    gl_Position = vec4( vertex, 1. ); \n"
            "    cOut = color; \n"
            "}";
        jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
        vs->addSourceString( std::string( vShaderSource ) );

        const char* fShaderSource =
            "#version 130 \n"
            "in vec3 cOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( cOut, 1. ); \n"
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        _spp = jagDraw::ShaderProgramPtr( new jagDraw::ShaderProgram );
        _spp->attachShader( vs );
        _spp->attachShader( fs );
    }

    return( true );
}

bool DrawCommandDemo::frame()
{
    glClear( GL_COLOR_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current ShaderProgram
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = 0;

    // glUseProgram for our ShaderProgram.
    (*_spp)( drawInfo );

    // Bind the GL_ARRAY_BUFFER for vertices
    (*_vbop)( drawInfo );
    // Enable and specify the "vertex" vertex attrib.
    (*_verts)( drawInfo );
    // Bind the GL_ARRAY_BUFFER for color
    (*_cbop)( drawInfo );
    // Enable and specify the "color" vertex attrib.
    (*_color)( drawInfo );

    // Bind the GL_ELEMENT_BUFFER
    (*_elbop)( drawInfo );
    // Draw the triangle strip on the left.
    (*_drawElements)( drawInfo );


    // Bind the GL_ARRAY_BUFFER for interleaved vertices and colors.
    (*_ibop)( drawInfo );
    // Enable and specify the "vertex" vertex attrib.
    (*_iVerts)( drawInfo );
    // Enable and specify the "color" vertex attrib.
    (*_iColor)( drawInfo );

    // Draw the triangle strip in the middle.
    (*_drawArrays)( drawInfo );


    // Bind the vertex array object.
    (*_vaop)( drawInfo );
    // Draw the triangle strip on the right.
    (*_drawArrays)( drawInfo );
    // Unbind vertex array object for non-VAO rendering in next frame.
    glBindVertexArray( 0 );


    glFlush ();

    JAG_ERROR_CHECK( "simple3x display()" );

    return( true );
}
