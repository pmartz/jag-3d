/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <demoSupport/DemoInterface.h>

#include <jagDraw/Common.h>
#include <jagDraw/types.h>
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
    jagDraw::DrawableVec _drawableVec;
    jagDraw::CommandMapVec _commandVec;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioDebug, jagBase::Log::Console );

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
    jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vShaderSource ) );

    jagDraw::ProgramPtr prog;
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
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        prog = jagDraw::ProgramPtr( new jagDraw::Program );
        prog->attachShader( vs );
        prog->attachShader( fs );
    }

    jagDraw::ProgramPtr prog2;
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
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        prog2 = jagDraw::ProgramPtr( new jagDraw::Program );
        prog2->attachShader( vs );
        prog2->attachShader( fs );
    }

    jagDraw::UniformPtr swizzleOff( new jagDraw::Uniform( "swizzle", false ) );
    jagDraw::UniformSetPtr swizzleOffSet( new jagDraw::UniformSet() );
    swizzleOffSet->insert( swizzleOff );

    jagDraw::UniformPtr swizzleOn( new jagDraw::Uniform( "swizzle", true ) );
    jagDraw::UniformSetPtr swizzleOnSet( new jagDraw::UniformSet() );
    swizzleOnSet->insert( swizzleOn );

    jagDraw::UniformPtr scale( new jagDraw::Uniform( "scale", 0.75f ) );
    jagDraw::UniformSetPtr scaleSet( new jagDraw::UniformSet() );
    scaleSet->insert( scale );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );
    jagDraw::CommandMapPtr commands( new jagDraw::CommandMap() );
    const float z = .5f;

    // Define first drawable: tri strip on the left.
    {
        commands->insert( prog );
        commands->insert( swizzleOffSet );

        jagBase::Point3fVec v3fa;
        v3fa.push_back( gmtl::Point3f( -.9f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, -.9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.7f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.4f, 0.f, z ) );
        v3fa.push_back( gmtl::Point3f( -.9f, .9f, z ) );
        v3fa.push_back( gmtl::Point3f( -.6f, .9f, z ) );
        jagBase::BufferPtr vbp( new jagBase::Buffer( v3fa.size() * sizeof( gmtl::Point3f ), (void*)&v3fa[0] ) );
        jagDraw::BufferObjectPtr vbop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, vbp ) );

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( vbop, jagDraw::VertexArrayObject::Vertex );

        jagDraw::VertexAttribPtr verts( new jagDraw::VertexAttrib(
            "vertex", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( verts, jagDraw::VertexArrayObject::Vertex );

        jagBase::Point3fVec c3fa;
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 0.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 0.f, 1.f, 1.f ) );
        c3fa.push_back( gmtl::Point3f( 1.f, 0.f, 1.f ) );
        jagBase::BufferPtr cbp( new jagBase::Buffer( c3fa.size() * sizeof( gmtl::Point3f ), (void*)&c3fa[0] ) );
        jagDraw::BufferObjectPtr cbop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, cbp ) );
        vaop->addVertexArrayCommand( cbop );

        jagDraw::VertexAttribPtr color( new jagDraw::VertexAttrib(
            "color", 3, GL_FLOAT, GL_FALSE, 0, 0 ) );
        vaop->addVertexArrayCommand( color );

        commands->insert( vaop );

        jagDraw::GLubyteVec elements;
        unsigned int idx;
        for( idx=0; idx<6; idx++ )
            elements.push_back( idx );
        jagBase::BufferPtr elbp( new jagBase::Buffer( elements.size() * sizeof( GLubyte ), (void*)&elements[0] ) );
        jagDraw::BufferObjectPtr elbop( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, elbp ) );
        jagDraw::DrawElementsPtr drawElements( new jagDraw::DrawElements( GL_TRIANGLE_STRIP, (const GLsizei) elements.size(), GL_UNSIGNED_BYTE, 0, elbop ) );
        drawable->addDrawCommand( drawElements );

        _commandVec.push_back( commands );
        _drawableVec.push_back( drawable );
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
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap );

        commands->insert( swizzleOnSet );

        jagBase::Point3fVec i3fa;
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

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iColor );
        commands->insert( vaop );

        drawable->addDrawCommand( drawArrays );

        _commandVec.push_back( commands );
        _drawableVec.push_back( drawable );
    }

    // Define third drawable (on the right)
    {
        drawable = jagDraw::DrawablePtr( new jagDraw::Drawable() );
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap() );

        commands->insert( prog2 );
        commands->insert( scaleSet );

        jagBase::Point3fVec i3fa;
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
        vaop->addVertexArrayCommand( ibop2, jagDraw::VertexArrayObject::Vertex );
        // Enable and specify the "vertex" vertex attrib.
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayObject::Vertex );
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
    BOOST_FOREACH( const jagDraw::DrawableVec::value_type& dp, _drawableVec )
    {
        dp->setMaxContexts( numContexts );
    }

    jagDraw::BoundPtr bound( drawable->getBound( *commands ) );
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
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "DrawableDemo init()" );
    return( true );
}

bool DrawableDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

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
