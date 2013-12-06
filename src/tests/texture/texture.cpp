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

#include <jagDraw/Common.h>
#include <jagDraw/Texture.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <jag/disk/ReadWrite.h>
#include <jagUtil/Shapes.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>


using namespace std;
namespace bpo = boost::program_options;


class TextureDemo : public DemoInterface
{
public:
    TextureDemo()
      : DemoInterface( "jag.demo.texture" ),
        _imageName( "balloon.jpg" ),
        _texObj( false )
    {}
    virtual ~TextureDemo() {}

    virtual bool parseOptions( boost::program_options::variables_map& vm );

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

protected:
    bool init2D();
    bool initTextureObject();

    jagDraw::DrawNodeContainer _nodes;

    std::string _imageName;
    bool _texObj;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    desc.add_options()
        ( "file,f", bpo::value< std::string >(), "Image to load. Default: balloon.jpg" )
        ( "texobj", "Test texture object" )
        ;

    return( new TextureDemo );
}

bool TextureDemo::parseOptions( bpo::variables_map& vm )
{
    if( vm.count( "file" ) > 0 )
        _imageName = vm[ "file" ].as< std::string >();

    _texObj = ( vm.count( "texobj" ) > 0 );

    return( true );
}

bool TextureDemo::init2D()
{
    const char* vShaderSource =
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        "#version 150 \n"
#else
        "#version 400 \n"
#endif
        "in vec3 vertex; \n"
        "in vec2 texcoord; \n"
        "out vec2 tcOut; \n"
        "void main() { \n"
        "    gl_Position = vec4( vertex, 1. ); \n"
        "    tcOut = texcoord; \n"
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
        "uniform sampler2D texture; \n"
        "in vec2 tcOut; \n"
        "out vec4 colorOut; \n"
        "void main() { \n"
        "    colorOut = texture2D( texture, tcOut ); \n"
        //"    colorOut = vec4( tcOut, 0., 1. ); \n"
        "}";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program ) );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( new jagDraw::CommandMap() );
    commands->insert( prog );


    jagDraw::UniformPtr textureUniform( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, (GLint)0 ) );
    jagDraw::UniformSetPtr uniformSet( new jagDraw::UniformSet() );
    (*uniformSet)[ textureUniform->getNameHash() ] = textureUniform;
    commands->insert( uniformSet );


    // Load image using jagDisk plugin interface.
    jagDraw::ImagePtr image( DemoInterface::readImageUtil( _imageName ) );
    if( image == NULL )
        return( false );


    jagDraw::TexturePtr tex( new jagDraw::Texture( GL_TEXTURE_2D, image,
        jagDraw::SamplerPtr( new jagDraw::Sampler() ) ) );
    tex->getSampler()->getSamplerState()->_minFilter = GL_LINEAR;
    jagDraw::TextureSetPtr texSet( new jagDraw::TextureSet() );
    (*texSet)[ GL_TEXTURE0 ] = tex;
    commands->insert( texSet );


    jagDraw::DrawablePtr drawable( new jagDraw::Drawable() );

    const GLsizei stride = sizeof( GLfloat ) * 5;
    jagDraw::VertexAttribPtr iVerts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jagDraw::VertexAttribPtr iColor( new jagDraw::VertexAttrib(
        "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jagDraw::DrawArraysPtr drawArrays( new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
    drawable->addDrawCommand( drawArrays );

    {
        const float z = .5f;
        typedef std::vector< float > FloatArray;
        FloatArray i3fa;
        i3fa.push_back( -.9f ); i3fa.push_back( -.9f ); i3fa.push_back( z );
            i3fa.push_back( 0.f ); i3fa.push_back( 0.f );
        i3fa.push_back( .9f ); i3fa.push_back( -.9f ); i3fa.push_back( z );
            i3fa.push_back( 1.f ); i3fa.push_back(  0.f );
        i3fa.push_back( -.9f ); i3fa.push_back( .9f ); i3fa.push_back( z );
            i3fa.push_back( 0.f ); i3fa.push_back( 1.f );
        i3fa.push_back( .9f ); i3fa.push_back( .9f ); i3fa.push_back( z );
            i3fa.push_back( 1.f ); i3fa.push_back( 1.f );
        jag::base::BufferPtr ibp( new jag::base::Buffer( i3fa.size() * sizeof( float ), (void*)&i3fa[0] ) );
        jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

        jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
        vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iVerts, jagDraw::VertexArrayObject::Vertex );
        vaop->addVertexArrayCommand( iColor );
        commands->insert( vaop );

        jagDraw::DrawNodePtr drawNode( new jagDraw::DrawNode( commands ) );
        drawNode->addDrawable( drawable );
        _nodes.push_back( drawNode );
    }

    return( true );
}
bool TextureDemo::initTextureObject()
{
#if( POCO_OS == POCO_OS_MAC_OS_X )
    JAG3D_FATAL( "Mac OSX OpenGL does not support texture buffer." );
    return( false );
#endif

    jagUtil::VNTCVec data;
    const gmtl::Point3f corners[] = {
        gmtl::Point3f( -.9f, -.9f, 0.f ),
        gmtl::Point3f( .1f, -.9f, 0.f ),
        gmtl::Point3f( -.9f, .1f, 0.f ),
        gmtl::Point3f( .1f, .1f, 0.f ) };
    const gmtl::Vec3f uVec( .8f, 0.f, 0.f );
    const gmtl::Vec3f vVec( 0.f, .8f, 0.f );
    for( unsigned int idx=0; idx<4; ++idx )
    {
        jagDraw::DrawNodePtr node( new jagDraw::DrawNode() );

        jagDraw::DrawablePtr quad( jagUtil::makePlane(
            data, corners[ idx ], uVec, vVec ) );
        node->addDrawable( quad );

        jagDraw::CommandMapPtr commands( new jagDraw::CommandMap() );
        node->setCommandMap( commands );

        jagDraw::UniformSetPtr usp( new jagDraw::UniformSet() );
        commands->insert( usp );
        usp->insert( jagDraw::UniformPtr( new jagDraw::Uniform( "texture", GL_SAMPLER_2D, 0 ) ) );
        usp->insert( jagDraw::UniformPtr( new jagDraw::Uniform( "index", (int)idx ) ) );

        _nodes.push_back( node );
    }

    jagDraw::CommandMapPtr& commands( _nodes[ 0 ]->getCommandMap() );
    commands->insert( jagUtil::createVertexArrayObject( data ) );

    jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "texobj.vert" ) );
    jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "texobj.frag" ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );
    commands->insert( prog );

    float texData[] = {
        1.f, 0.f, 0.f, 1.f,
        0.f, 1.f, 0.f, 1.f,
        0.f, 0.f, 1.f, 1.f,
        .5f, .5f, .5f, 1.f
    };
    jag::base::BufferPtr bufferData( new jag::base::Buffer( sizeof( texData ), texData ) );
    jagDraw::TextureBufferPtr textureBuffer( new jagDraw::TextureBuffer( bufferData ) );

    jagDraw::TextureSetPtr ts( new jagDraw::TextureSet() );
    commands->insert( ts );
    jagDraw::TexturePtr texture( new jagDraw::Texture( GL_TEXTURE_BUFFER, GL_RGBA32F, textureBuffer ) );
    (*ts)[ GL_TEXTURE0 ] = texture;

    return( true );
}

bool TextureDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    bool result;
    if( _texObj )
        result = initTextureObject();
    else
        result = init2D();
    if( !result )
        return( false );

    // Tell all Jag3D objects how many contexts to expect.
    _nodes.setMaxContexts( numContexts );


    return( true );
}

bool TextureDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    JAG3D_ERROR_CHECK( "TextureDemo init()" );
    return( true );
}


bool TextureDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Render all Drawables.
    _nodes.execute( drawInfo );
    
    glFlush();
    JAG3D_ERROR_CHECK( "TextureDemo display()" );

    return( true );
}
