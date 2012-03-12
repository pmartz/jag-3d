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


class Simple3xDemo : public DemoInterface
{
public:
    Simple3xDemo() : _first( true ) {}
    virtual ~Simple3xDemo() {}

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
    jagDraw::VertexArrayObjectPtr _vaop;

    bool _first;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    return( new Simple3xDemo );
}

bool Simple3xDemo::init()
{
    _logger = Poco::Logger::has( "jag3d.demo" );
    if( _logger == NULL )
    {
        _logger = &( Poco::Logger::create( "jag3d.demo",
                (Poco::Channel*)( jagBase::Log::instance()->getConsole() ), Poco::Message::PRIO_INFORMATION ) );
    }

    _logger->information( jagBase::getVersionString() );


    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    _logger->information( msg );


    glClearColor( 0.f, 0.f, 0.f, 0.f );

    _vaop = jagDraw::VertexArrayObjectPtr( new jagDraw::VertexArrayObject );

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

bool Simple3xDemo::frame()
{
    glClear( GL_COLOR_BUFFER_BIT );

    jagDraw::DrawInfo drawInfo;
    drawInfo._id = 0;

    _spp->use( drawInfo );

    (*_vbop)( drawInfo );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0 );
    glEnableVertexAttribArray( 0 );
    (*_cbop)( drawInfo );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0 );
    glEnableVertexAttribArray( 1 );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 6 );


    const GLsizei stride = sizeof( GLfloat ) * 3 * 2;
    (*_ibop)( drawInfo );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)( sizeof( GLfloat ) * 3 ) );
    glEnableVertexAttribArray( 1 );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 6 );


    // This is an app-managed vertex array object. The VertexArrayObject
    // class actually supports adding BufferObject and VertexArrrib instances,
    // which VAO manages internall, so that code like the following is nor
    // necessary. See the drawcommand example.
    (*_vaop)( drawInfo );
    if( _first )
    {
        _first = false;

        (*_ibop2)( drawInfo );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0 );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)( sizeof( GLfloat ) * 3 ) );
        glEnableVertexAttribArray( 1 );
    }
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 6 );
    glBindVertexArray( 0 );


    glFlush ();

    JAG_ERROR_CHECK( "simple3x display()" );

    return( true );
}
