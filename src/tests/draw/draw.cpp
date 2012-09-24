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
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class DrawDemo : public DemoInterface
{
public:
    DrawDemo()
      : DemoInterface( "jag.demo.draw" ),
        _first( true )
    {}
    virtual ~DrawDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagDraw::BufferObjectPtr _vbop;
    jagDraw::BufferObjectPtr _cbop;
    jagDraw::BufferObjectPtr _ibop;
    jagDraw::BufferObjectPtr _ibop2;
    jagDraw::ShaderProgramPtr _prog;
    jagDraw::VertexArrayObjectPtr _vaop;

    bool _first;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    return( new DrawDemo );
}

bool DrawDemo::startup( const unsigned int numContexts )
{
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

        _prog = jagDraw::ShaderProgramPtr( new jagDraw::Program );
        _prog->attachShader( vs );
        _prog->attachShader( fs );
    }


    // Tell all Jag objects how many contexts to expect.
    _vbop->setMaxContexts( numContexts );
    _cbop->setMaxContexts( numContexts );
    _ibop->setMaxContexts( numContexts );
    _ibop2->setMaxContexts( numContexts );
    _prog->setMaxContexts( numContexts );
    _vaop->setMaxContexts( numContexts );


    return( true );
}

bool DrawDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getVersionString();

    return( true );
}

bool DrawDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    glClear( GL_COLOR_BUFFER_BIT );

    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

    (*_prog)( drawInfo );

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

    JAG3D_ERROR_CHECK( "DrawDemo frame()" );

    return( true );
}
