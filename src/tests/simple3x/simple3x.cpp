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
    Simple3xDemo() {}
    virtual ~Simple3xDemo() {}

    virtual bool init();
    virtual bool frame();
    virtual bool shutdown() { return( true ); }

protected:
    Poco::Logger* _logger;

    jagDraw::BufferObjectPtr _bop;
    jagDraw::ShaderProgramPtr _spp;
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
    string msg( "GL_VENDOR: " );
    msg.append( (char*)(glGetString( GL_VENDOR )) );
    _logger->information( msg );

    msg = string( "GL_RENDERER: " );
    msg.append( (char*)(glGetString( GL_RENDERER )) );
    _logger->information( msg );

    msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    _logger->information( msg );

    msg = string( "GL_SHADING_LANGUAGE_VERSION: " );
    msg.append( (char*)(glGetString( GL_SHADING_LANGUAGE_VERSION )) );
    _logger->information( msg );


    glClearColor( 0.4f, 0.4f, 0.4f, 0.f );

    {
        float z = .5f;
        typedef std::vector< gmtl::Point3f > Point3fArray;
        Point3fArray p3fa;
        p3fa.push_back( gmtl::Point3f( -.9f, -.9f, z ) );
        p3fa.push_back( gmtl::Point3f( .4f, -.9f, z ) );
        p3fa.push_back( gmtl::Point3f( -.4f, 0.f, z ) );
        p3fa.push_back( gmtl::Point3f( .9f, 0.f, z ) );
        p3fa.push_back( gmtl::Point3f( -.9f, .9f, z ) );
        p3fa.push_back( gmtl::Point3f( .4f, .9f, z ) );
        jagBase::BufferPtr bp( new jagBase::Buffer( p3fa.size() * sizeof( gmtl::Point3f ), (void*)&p3fa[0] ) );
        _bop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, bp ) );
    }

    {
        const char* vShaderSource =
            "#version 130 \n"
            "in vec3 vertex; \n"
            "void main() { \n"
            "    gl_Position = vec4( vertex, 1. ); \n"
            "}";
        jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
        vs->addSourceString( std::string( vShaderSource ) );

        const char* fShaderSource =
            "#version 130 \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( 0., .8, .8, 0. ); \n"
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

    (*_spp)( drawInfo );

    (*_bop)( drawInfo );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0 );
    glEnableVertexAttribArray( 0 );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 6 );

    glFlush ();

    JAG_ERROR_CHECK( "simple3x display()" );

    return( true );
}
