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

#include <jagDraw/Common.h>
#include <jagDraw/types.h>
#include <jagBase/Profile.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <Poco/Foundation.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class BenchmarkTest : public DemoInterface
{
public:
    BenchmarkTest()
      : DemoInterface( "jag.test.bench" )
    {}
    virtual ~BenchmarkTest() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagDraw::DrawableVec _drawables;
    jagDraw::CommandMap _commands;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioDebug, jagBase::Log::Console );

    return( new BenchmarkTest );
}


jagDraw::BufferObjectPtr createTriangleVertices( unsigned int n,
    const float x = .05f, const float y = .05f, const float z = .5f )
{
    const float xMin=-.9f;
    const float xMax=.9f;
    const float yMin=-.9f;
    const float yMax=.9f;
    jagBase::Point3fVec v3fa( n );

    float yVal( yMin );
    float yInc( y );
    for( unsigned int idx=0; idx<n; ++idx )
    {
        float xVal( ( idx & 1 ) ? ( xMin + x ) : xMin );
        xVal = std::min( xVal, xMax );

        v3fa[ idx ].set( xVal, yVal, z );

        if( idx & 1 )
        {
            yVal += yInc;
            if( yVal > yMax )
            {
                yVal = yMax;
                yInc = -yInc;
            }
            else if( yVal < yMin )
            {
                yVal = yMin;
                yInc = -yInc;
            }
        }
    }
    jagBase::BufferPtr vbp( new jagBase::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );

    return( jagDraw::BufferObjectPtr( new jagDraw::BufferObject( GL_ARRAY_BUFFER, vbp ) ) );
}

jagDraw::ProgramPtr createProgram()
{
    const char* vShaderSource =
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
    vs->addSourceString( std::string( vShaderSource ) );

    const char* fShaderSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
        "out vec4 colorOut; \n"
        "void main() { \n"
        "    colorOut = vec4( 1. ); \n"
        "}";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    return( prog );
}

void createSingleTriStrip( const unsigned int numVerts, jagDraw::DrawablePtr& drawable )
{
    drawable = jagDraw::DrawablePtr( new jagDraw::Drawable() );

    jagDraw::GLuintVec elements;
    unsigned int idx;
    for( idx=0; idx<numVerts; idx++ )
        elements.push_back( idx );
    jagBase::BufferPtr elbp( new jagBase::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jagDraw::BufferObjectPtr elbop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
    jagDraw::DrawElementsPtr drawElements( new jagDraw::DrawElements( GL_TRIANGLE_STRIP, (const GLsizei) elements.size(), GL_UNSIGNED_INT, 0, elbop ) );

    drawable->addDrawCommand( drawElements );

    drawable->setMaxContexts( 1 );
}

const size_t numTestCases( 1 );
const unsigned int numVerts( 10000000 );

bool BenchmarkTest::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    // Set up CommandMap with Program and VAO.
    {
        _commands.insert( createProgram() );

        jagDraw::BufferObjectPtr vbop( createTriangleVertices( numVerts ) );
        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( vbop, jagDraw::VertexArrayObject::Vertex );
        jagDraw::VertexAttribPtr verts( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( verts, jagDraw::VertexArrayObject::Vertex );
        _commands.insert( vaop );

        _commands.setMaxContexts( 1 );
    }

    _drawables.resize( numTestCases );

    createSingleTriStrip( numVerts, _drawables[ 0 ] );

    return( true );
}

bool BenchmarkTest::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "BenchmarkTest init()" );
    return( true );
}

bool BenchmarkTest::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    _commands.execute( drawInfo );

    {
        JAG3D_PROFILE( "benchmark" );
        _drawables[ 0 ]->execute( drawInfo );
        glFinish();
    }
    
    glFlush();
    JAG3D_ERROR_CHECK( "BenchmarkTest display()" );

    return( true );
}
