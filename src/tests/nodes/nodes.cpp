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
#include <jagSG/Common.h>
#include <jagSG/ExecuteVisitor.h>
#include <jag/draw/PerContextData.h>
#include <jag/base/Transform.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>
#include <sstream>



using namespace std;
namespace bpo = boost::program_options;


class NodesDemo : public DemoInterface
{
public:
    NodesDemo()
      : DemoInterface( "jag.demo.nodes" )
    {}
    virtual ~NodesDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );

protected:
    jagSG::NodePtr makeScene( const gmtl::Point3f& offset, const gmtl::Matrix44d& trans=gmtl::MAT_IDENTITY44D );

    gmtl::Matrix44d computeProjection( double aspect );
    gmtl::Matrix44d computeView();

    jagSG::NodePtr _scene;

    typedef jag::draw::PerContextData< gmtl::Matrix44d > PerContextMatrix44d;
    PerContextMatrix44d _proj;
    jag::draw::BoundPtr _bSphere;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioInfo, jag::base::Log::Console );
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioWarning, "jag.draw.ctx" );

    return( new NodesDemo );
}

jagSG::NodePtr NodesDemo::makeScene( const gmtl::Point3f& offset, const gmtl::Matrix44d& trans )
{
    jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new jag::draw::CommandMap() ) );
    jag::draw::DrawablePtr drawable( jag::draw::DrawablePtr( new jag::draw::Drawable() ) );
    jagSG::NodePtr node( jagSG::NodePtr( new jagSG::Node() ) );

    // Interleaved vertices and normals.
    jag::base::Point3fVec data;
    data.push_back( gmtl::Point3f( -1.f, -1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( 1.f, -1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( -1.f, 1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( 1.f, 1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    jag::base::BufferPtr ibp( new jag::base::Buffer( data.size() * sizeof( gmtl::Point3f ), (void*)&(data[0]) ) );
    jag::draw::BufferObjectPtr ibop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    const GLsizei stride( sizeof( float ) * 6 );
    jag::draw::VertexAttribPtr verts( new jag::draw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    const GLsizei startOffset( sizeof( float ) * 3 );
    jag::draw::VertexAttribPtr norms( new jag::draw::VertexAttrib(
        "normal", 3, GL_FLOAT, GL_FALSE, stride, startOffset ) );

    jag::draw::VertexArrayObjectPtr vaop = jag::draw::VertexArrayObjectPtr( new jag::draw::VertexArrayObject() );
    vaop->addVertexArrayCommand( ibop, jag::draw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( verts, jag::draw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( norms );
    commands->insert( vaop );

    drawable->addDrawCommand( jag::draw::DrawCommandPtr(
        new jag::draw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) ) );

    node->setCommandMap( commands );
    node->addDrawable( drawable );

    node->setTransform( trans );

    return( node );
}

bool NodesDemo::startup( const unsigned int numContexts )
{
    _scene = makeScene( gmtl::Point3f( 0.f, 1.f, .5f ) );
    _scene->addChild( makeScene( gmtl::Point3f( -1.f, 0.f, 0.f ) ) );

    gmtl::Matrix44d mat;
    mat = gmtl::make( gmtl::AxisAngled( 1., 1., 0., 0. ), gmtl::Type2Type< gmtl::Matrix44d >() );
    _scene->addChild( makeScene( gmtl::Point3f( 1.1f, -.25f, 0.f ), mat ) );


    jag::draw::CommandMapPtr commands( _scene->getCommandMap() );
    if( commands == NULL )
    {
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
        _scene->setCommandMap( commands );
    }

    // Test bounding sphere computation.
    {
        _bSphere = ((jag::draw::BoundOwner*)(_scene.get()))->getBound( *commands );
        const gmtl::Sphered s( _bSphere->asSphere() );

        if( !( s.isInitialized() ) )
        {
            JAG3D_CRITICAL_STATIC( _logName, "Bound: Uninitialized." );
        }
        else
        {
            std::ostringstream ostr;
            ostr << "Bound: " << s;
            JAG3D_CRITICAL_STATIC( _logName, ostr.str() );
        }
    }

    jag::draw::ShaderPtr vs( DemoInterface::readShaderUtil( "nodes.vert" ) );
    jag::draw::ShaderPtr fs( DemoInterface::readShaderUtil( "nodes.frag" ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    commands->insert( prog );

    // Test uniform blocks
    jag::draw::UniformBlockPtr ubp( jag::draw::UniformBlockPtr(
        new jag::draw::UniformBlock( "blockTest" ) ) );
    ubp->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ambientScene", .2f ) ) );
    ubp->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "diffuseMat", gmtl::Point3f( 0.f, .7f, 0.9f ) ) ) );
    jag::draw::UniformBlockSetPtr ubsp( new jag::draw::UniformBlockSet() );
    ubsp->insert( ubp );
    commands->insert( ubsp );

    gmtl::Vec3f lightVec( 0.5f, .7f, 1.f );
    gmtl::normalize( lightVec );
    jag::draw::UniformSetPtr usp( new jag::draw::UniformSet() );
    usp->insert( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ecLightDir", lightVec ) ) );
    commands->insert( usp );


    // We keep a different project matrix per context (to support different
    // window sizes). Initialize them all to a reasonable default.
    const gmtl::Matrix44d defaultProjMat( computeProjection( 1. ) );
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
        _proj._data.push_back( defaultProjMat );


    // Tell all Jag3D objects how many contexts to expect.
    _scene->setMaxContexts( numContexts );


    return( DemoInterface::startup( numContexts ) );
}

bool NodesDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    return( true );
}

bool NodesDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jagSG::ExecuteVisitor execVisitor( drawInfo );

    // Systems such as VRJ will pass view and projection matrices.
    if( view.mState != gmtl::Matrix44f::IDENTITY || proj.mState != gmtl::Matrix44f::IDENTITY )
        execVisitor.setViewProj( view, proj );
    else
        execVisitor.setViewProj( computeView(), _proj._data[ contextID ] );


    // Render all Drawables.
    _scene->accept( execVisitor );

    glFlush();

    JAG3D_ERROR_CHECK( "nodes display()" );

    return( true );
}

void NodesDemo::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    _proj._data[ contextID ] = computeProjection( (double)w/(double)h );
}

gmtl::Matrix44d NodesDemo::computeProjection( double aspect )
{
    const gmtl::Sphered s( _bSphere->asSphere() );

    if( s.getRadius() <= 0.f )
        return( gmtl::MAT_IDENTITY44D );

    gmtl::Matrix44d proj;
    const double zNear = 3.5 * s.getRadius();
    const double zFar = 5.75 * s.getRadius();
    gmtl::setPerspective< double >( proj, 30., aspect, zNear, zFar );

    return( proj );
}

gmtl::Matrix44d NodesDemo::computeView()
{
    const gmtl::Sphered s( _bSphere->asSphere() );
    const gmtl::Point3d center( s.getCenter() );
    const double radius( (float)s.getRadius() );

    const gmtl::Point3d eye( center + ( gmtl::Point3d( 1.5, -4., 1.5 ) * radius ) );
    const gmtl::Vec3d up( 0.f, 0.f, 1.f );

    gmtl::Matrix44d view;
    gmtl::setLookAt( view, eye, center, up );
    return( view );
}
