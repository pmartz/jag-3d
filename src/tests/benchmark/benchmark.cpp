/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <demoSupport/DemoInterface.h>

#include <jag/draw/Common.h>
#include <jag/draw/types.h>
#include <jag/base/Profile.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <Poco/Foundation.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>



const size_t numTestCases( 5 );
const unsigned int numVerts( 1000000 );
const unsigned int trisPerCommand( 6 );


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

protected:
    jag::draw::DrawableVec _drawables;
    jag::draw::CommandMap _commands;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioDebug, jag::base::Log::Console );

    return( new BenchmarkTest );
}


jag::draw::BufferObjectPtr createTriangleVertices( unsigned int n,
    const float x = .05f, const float y = .05f, const float z = .5f )
{
    const float xMin=-.9f;
    const float xMax=.9f;
    const float yMin=-.9f;
    const float yMax=.9f;
    jag::base::Point3fVec v3fa( n );

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
    jag::base::BufferPtr vbp( new jag::base::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );

    return( jag::draw::BufferObjectPtr( new jag::draw::BufferObject( GL_ARRAY_BUFFER, vbp ) ) );
}

jag::draw::ProgramPtr createProgram()
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
    jag::draw::ShaderPtr vs( new jag::draw::Shader( GL_VERTEX_SHADER ) );
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
    jag::draw::ShaderPtr fs( new jag::draw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    return( prog );
}

void createSingleTriStrip( const unsigned int numVerts, jag::draw::DrawablePtr& drawable )
{
    drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    jag::draw::GLuintVec elements;
    unsigned int idx;
    for( idx=0; idx<numVerts; ++idx )
        elements.push_back( idx );
    jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
    jag::draw::DrawElementsPtr drawElements( new jag::draw::DrawElements( GL_TRIANGLE_STRIP, (const GLsizei) elements.size(), GL_UNSIGNED_INT, 0, elbop ) );

    drawable->addDrawCommand( drawElements );

    drawable->setMaxContexts( 1 );
}
void createSeveralTriStrips( const unsigned int numVerts, const unsigned int trisPerCommand, jag::draw::DrawablePtr& drawable )
{
    drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    jag::draw::GLuintVec elements;
    const unsigned int vertsPerCommand( trisPerCommand + 2 );
    const unsigned int totalTris( numVerts - 2 );
    const unsigned int intCommandCount( totalTris/trisPerCommand );
    const unsigned int totalCommands( ( (float)totalTris/(float)trisPerCommand == intCommandCount ) ?
        intCommandCount : intCommandCount+1 );

    unsigned int vertsRemaining( numVerts );
    unsigned int startIdx( 0 );
    for( unsigned int numCommands = totalCommands; numCommands > 0; --numCommands )
    {
        unsigned int idx;
        unsigned int currentVertCount( std::min( vertsPerCommand, vertsRemaining ) );
        for( idx=0; idx < currentVertCount; idx++ )
            elements.push_back( idx + startIdx );
        vertsRemaining -= trisPerCommand;
        startIdx += trisPerCommand;
    }

    jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );

    vertsRemaining = numVerts;
    startIdx = 0;
    for( unsigned int numCommands = totalCommands; numCommands > 0; --numCommands )
    {
        unsigned int currentVertCount( std::min( vertsPerCommand, vertsRemaining ) );
        jag::draw::DrawElementsPtr drawElements( new jag::draw::DrawElements( GL_TRIANGLE_STRIP,
            (const GLsizei)currentVertCount, GL_UNSIGNED_INT, (const GLvoid*)(startIdx*sizeof(GLint)), elbop ) );
        drawable->addDrawCommand( drawElements );
        vertsRemaining -= trisPerCommand;
        startIdx += vertsPerCommand;
    }
    std::cout << drawable->getDrawCommandVec().size() << std::endl;

    drawable->setMaxContexts( 1 );
}
void createSingleTriStripRestart( const unsigned int numVerts, const unsigned int trisPerCommand, jag::draw::DrawablePtr& drawable )
{
    drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    const unsigned int vertsPerCommand( trisPerCommand + 2 );

    jag::draw::GLuintVec elements;
    unsigned int vertsRemaining( numVerts );
    unsigned int idx;
    for( idx=0; idx<numVerts;  )
    {
        unsigned int currentVertCount( std::min( vertsPerCommand, vertsRemaining ) );
        for( unsigned int n=0; n<currentVertCount; ++n )
        {
            elements.push_back( idx++ );
        }
        if( idx >= numVerts )
            break;
        elements.push_back( 0xffffffff );
        idx -= 2;
        vertsRemaining -= trisPerCommand;
    }
    jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );

    jag::draw::PrimitiveRestartPtr restart( new jag::draw::PrimitiveRestart( 0xffffffff ) );
    drawable->addDrawCommand( restart );

    jag::draw::DrawElementsPtr drawElements( new jag::draw::DrawElements( GL_TRIANGLE_STRIP, (const GLsizei) elements.size(), GL_UNSIGNED_INT, 0, elbop ) );
    drawable->addDrawCommand( drawElements );

    restart.reset( new jag::draw::PrimitiveRestart( 0, false ) );
    drawable->addDrawCommand( restart );

    drawable->setMaxContexts( 1 );
}
void createMultiTriStrip( const unsigned int numVerts, const unsigned int trisPerCommand, jag::draw::DrawablePtr& drawable )
{
    drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    const unsigned int vertsPerCommand( trisPerCommand + 2 );
    const unsigned int totalTris( numVerts - 2 );
    const unsigned int intCommandCount( totalTris/trisPerCommand );
    const GLsizei primCount( ( (float)totalTris/(float)trisPerCommand == intCommandCount ) ?
        intCommandCount : intCommandCount+1 );

    jag::draw::GLuintVec elements;
    jag::draw::GLsizeiVec counts( primCount );
    jag::draw::GLvoidPtrVec indices( primCount );

    unsigned int vertsRemaining( numVerts );
    unsigned int idx;
    unsigned int prim( 0 );
    for( idx=0; idx<numVerts;  )
    {
        unsigned int currentVertCount( std::min( vertsPerCommand, vertsRemaining ) );

        indices[ prim ] = (GLvoid*)( elements.size() * sizeof( GLint ) );
        counts[ prim ] = currentVertCount;
        for( unsigned int n=0; n<currentVertCount; ++n )
        {
            elements.push_back( idx++ );
        }
        if( idx >= numVerts )
            break;
        idx -= 2;
        vertsRemaining -= trisPerCommand;
        ++prim;
    }
    jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );

    jag::draw::MultiDrawElementsPtr multiDrawElements(
        new jag::draw::MultiDrawElements( GL_TRIANGLE_STRIP, counts, GL_UNSIGNED_INT, indices, primCount, elbop ) );
    drawable->addDrawCommand( multiDrawElements );

    drawable->setMaxContexts( 1 );
}
void createSingleTriangles( const unsigned int numVerts, jag::draw::DrawablePtr& drawable )
{
    drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    jag::draw::GLuintVec elements;
    unsigned int idx;
    for( idx=0; idx<(numVerts-2); idx+=2 )
    {
        elements.push_back( idx );
        elements.push_back( idx+1 );
        elements.push_back( idx+2 );
        elements.push_back( idx+2 );
        elements.push_back( idx+1 );
        elements.push_back( idx+3 );
    }
    jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLint ), (void*)&elements[0] ) );
    jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
    jag::draw::DrawElementsPtr drawElements( new jag::draw::DrawElements( GL_TRIANGLES, (const GLsizei) elements.size(), GL_UNSIGNED_INT, 0, elbop ) );

    drawable->addDrawCommand( drawElements );

    drawable->setMaxContexts( 1 );
}



bool BenchmarkTest::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    // Set up CommandMap with Program and VAO.
    {
        _commands.insert( createProgram() );

        jag::draw::BufferObjectPtr vbop( createTriangleVertices( numVerts ) );
        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
        vaop->addVertexArrayCommand( vbop, jag::draw::VertexArrayObject::Vertex );
        jag::draw::VertexAttribPtr verts( new jag::draw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( verts, jag::draw::VertexArrayObject::Vertex );
        _commands.insert( vaop );

        _commands.setMaxContexts( 1 );
    }

    // Create Drawables for all test cases
    _drawables.resize( numTestCases );
    createSingleTriStrip( numVerts, _drawables[ 0 ] );
    createSeveralTriStrips( numVerts, trisPerCommand, _drawables[ 1 ] );
    createSingleTriStripRestart( numVerts, trisPerCommand, _drawables[ 2 ] );
    createMultiTriStrip( numVerts, trisPerCommand, _drawables[ 3 ] );
    createSingleTriangles( numVerts, _drawables[ 4 ] );

    return( true );
}

bool BenchmarkTest::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "BenchmarkTest init()" );
    return( true );
}

bool BenchmarkTest::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    _commands.execute( drawInfo );
    // Clear GL pipe before timing anything.
    glFinish();

    if( true )
    {
        JAG3D_PROFILE( "Single GL_TRIANGLE_STRIP command" );
        _drawables[ 0 ]->execute( drawInfo );
        glFinish();
    }
    if( true )
    {
        JAG3D_PROFILE( "Several GL_TRIANGLE_STRIP commands" );
        _drawables[ 1 ]->execute( drawInfo );
        glFinish();
    }
    if( true )
    {
        JAG3D_PROFILE( "Single GL_TRIANGLE_STRIP command with restart" );
        _drawables[ 2 ]->execute( drawInfo );
        glFinish();
    }
    if( true )
    {
        JAG3D_PROFILE( "Multi GL_TRIANGLE_STRIP command" );
        _drawables[ 3 ]->execute( drawInfo );
        glFinish();
    }
    if( true )
    {
        JAG3D_PROFILE( "Single GL_TRIANGLES command" );
        _drawables[ 4 ]->execute( drawInfo );
        glFinish();
    }
    
    glFlush();
    JAG3D_ERROR_CHECK( "BenchmarkTest display()" );

    return( true );
}
