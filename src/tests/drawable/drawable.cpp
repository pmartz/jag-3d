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
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <Poco/Foundation.h>
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
      : DemoInterface( "jag.demo.drawable" )
    {}
    virtual ~DrawableDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

protected:
    jag::draw::DrawableVec _drawableVec;
    jag::draw::CommandMapVec _commandVec;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioDebug, jag::base::Log::Console );

    return( new DrawableDemo );
}

bool DrawableDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    const char* vShaderSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
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
    jag::draw::ShaderPtr vs( new jag::draw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vShaderSource ) );

    jag::draw::ProgramPtr prog;
    {
        const char* fShaderSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
            // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
            "#version 150 \n"
#else
            "#version 400 \n"
#endif
            "in vec3 cOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( cOut, 1. ); \n"
            "}";
        jag::draw::ShaderPtr fs( new jag::draw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        prog = jag::draw::ProgramPtr( new jag::draw::Program );
        prog->attachShader( vs );
        prog->attachShader( fs );
    }

    jag::draw::ProgramPtr prog2;
    {
        const char* fShaderSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
            // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
            "#version 150 \n"
#else
            "#version 400 \n"
#endif
            "uniform float scale; \n"
            "in vec3 cOut; \n"
            "out vec4 colorOut; \n"
            "void main() { \n"
            "    colorOut = vec4( cOut * scale, 1. ); \n"
            "}";
        jag::draw::ShaderPtr fs( new jag::draw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        prog2 = jag::draw::ProgramPtr( new jag::draw::Program );
        prog2->attachShader( vs );
        prog2->attachShader( fs );
    }

    jag::draw::UniformPtr swizzleOff( new jag::draw::Uniform( "swizzle", false ) );
    jag::draw::UniformSetPtr swizzleOffSet( new jag::draw::UniformSet() );
    swizzleOffSet->insert( swizzleOff );

    jag::draw::UniformPtr swizzleOn( new jag::draw::Uniform( "swizzle", true ) );
    jag::draw::UniformSetPtr swizzleOnSet( new jag::draw::UniformSet() );
    swizzleOnSet->insert( swizzleOn );

    jag::draw::UniformPtr scale( new jag::draw::Uniform( "scale", 0.75f ) );
    jag::draw::UniformSetPtr scaleSet( new jag::draw::UniformSet() );
    scaleSet->insert( scale );


    jag::draw::DrawablePtr drawable( new jag::draw::Drawable() );
    jag::draw::CommandMapPtr commands( new jag::draw::CommandMap() );
    const float z = .5f;

    // Define first drawable: tri strip on the left.
    {
        commands->insert( prog );
        commands->insert( swizzleOffSet );

        jag::base::Point3fVec v3fa;
        v3fa.push_back( gmtl::Point3f( -.9f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.7f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.4f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.9f, .9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, .9f, z ) );
        jag::base::BufferPtr vbp( new jag::base::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );
        jag::draw::BufferObjectPtr vbop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, vbp ) );

        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
        vaop->addVertexArrayCommand( vbop, jag::draw::VertexArrayObject::Vertex );

        jag::draw::VertexAttribPtr verts( new jag::draw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( verts, jag::draw::VertexArrayObject::Vertex );

        jag::base::Point3fVec c3fa;
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jag::base::BufferPtr cbp( new jag::base::Buffer( c3fa.size() * sizeof( gmtl::Point3f ), (void*)&c3fa[0] ) );
        jag::draw::BufferObjectPtr cbop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, cbp ) );
        vaop->addVertexArrayCommand( cbop );

        jag::draw::VertexAttribPtr color( new jag::draw::VertexAttrib(
            "color", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( color );

        commands->insert( vaop );

        jag::draw::GLubyteVec elements;
        unsigned int idx;
        for( idx=0; idx<6; idx++ )
            elements.push_back( idx );
        jag::base::BufferPtr elbp( new jag::base::Buffer( elements.size() * sizeof( GLubyte ), (void*)&elements[0] ) );
        jag::draw::BufferObjectPtr elbop( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
        jag::draw::DrawElementsPtr drawElements( new jag::draw::DrawElements( GL_TRIANGLE_STRIP, (const GLsizei) elements.size(), GL_UNSIGNED_BYTE, 0, elbop ) );
        drawable->addDrawCommand( drawElements );

        _commandVec.push_back( commands );
        _drawableVec.push_back( drawable );
    }

    // Define elements shared by second and third drawables.
    const GLsizei stride = sizeof( GLfloat ) * 3 * 2;
    jag::draw::VertexAttribPtr iVerts( new jag::draw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jag::draw::VertexAttribPtr iColor( new jag::draw::VertexAttrib(
        "color", 3, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jag::draw::DrawArraysPtr drawArrays( new jag::draw::DrawArrays( GL_TRIANGLE_STRIP, 0, 6 ) );

    // Define second drawable (middle)
    {
        drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap );

        commands->insert( swizzleOnSet );

        jag::base::Point3fVec i3fa;
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
        jag::base::BufferPtr ibp( new jag::base::Buffer( i3fa.size() * sizeof( gmtl::Point3f ), (void*)&i3fa[0] ) );
        jag::draw::BufferObjectPtr ibop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jag::draw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iVerts, jag::draw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iColor );
        commands->insert( vaop );

        drawable->addDrawCommand( drawArrays );

        _commandVec.push_back( commands );
        _drawableVec.push_back( drawable );
    }

    // Define third drawable (on the right)
    {
        drawable = jag::draw::DrawablePtr( new jag::draw::Drawable() );
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );

        commands->insert( prog2 );
        commands->insert( scaleSet );

        jag::base::Point3fVec i3fa;
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
        jag::base::BufferPtr ibp( new jag::base::Buffer( i3fa.size() * sizeof( gmtl::Point3f ), (void*)&i3fa[0] ) );
        jag::draw::BufferObjectPtr ibop2( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
        // Bind the GL_ARRAY_BUFFER for interleaved vertices and colors
        // (different from _ibop dur to vertex positions).
        vaop->addVertexArrayCommand( ibop2, jag::draw::VertexArrayObject::Vertex );
        // Enable and specify the "vertex" vertex attrib.
        vaop->addVertexArrayCommand( iVerts, jag::draw::VertexArrayObject::Vertex );
        // Enable and specify the "color" vertex attrib.
        vaop->addVertexArrayCommand( iColor );

        commands->insert( vaop );

        drawable->addDrawCommand( drawArrays );

        _commandVec.push_back( commands );
        _drawableVec.push_back( drawable );
    }


    // Tell all Jag3D objects how many contexts to expect.
    for( unsigned int idx=0; idx<_drawableVec.size(); ++idx )
    {
        _commandVec[ idx ]->setMaxContexts( numContexts );
        _drawableVec[ idx ]->setMaxContexts( numContexts );
    }
    BOOST_FOREACH( const jag::draw::DrawableVec::value_type& dp, _drawableVec )
    {
        dp->setMaxContexts( numContexts );
    }

    jag::draw::BoundPtr bound( drawable->getBound( *commands ) );
    gmtl::AABoxd box( bound->asAABox() );
    std::ostringstream ostr;
    ostr << "Min: " << box.mMin << "  Max: " << box.mMax;
    JAG3D_INFO_STATIC( _logName, "Bounding box: " + ostr.str() );


    return( true );
}

bool DrawableDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "DrawableDemo init()" );
    return( true );
}

bool DrawableDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Render all Drawables.
    for( unsigned int idx=0; idx<_drawableVec.size(); ++idx )
    {
        _commandVec[ idx ]->execute( drawInfo );
        _drawableVec[ idx ]->execute( drawInfo );
    }
    
    glFlush();
    JAG3D_ERROR_CHECK( "DrawableDemo display()" );

    return( true );
}
