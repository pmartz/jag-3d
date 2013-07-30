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
#include <jagSG/Common.h>
#include <jagSG/ExecuteVisitor.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/Transform.h>
#include <jagDisk/ReadWrite.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
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
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagSG::NodePtr makeScene( const gmtl::Point3f& offset, const gmtl::Matrix44d& trans=gmtl::MAT_IDENTITY44D );

    gmtl::Matrix44d computeProjection( double aspect );
    gmtl::Matrix44d computeView();

    jagSG::NodePtr _scene;

    typedef jagDraw::PerContextData< gmtl::Matrix44d > PerContextMatrix44d;
    PerContextMatrix44d _proj;
    jagDraw::BoundPtr _bSphere;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioInfo, jagBase::Log::Console );

    return( new NodesDemo );
}

jagSG::NodePtr NodesDemo::makeScene( const gmtl::Point3f& offset, const gmtl::Matrix44d& trans )
{
    jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new jagDraw::CommandMap() ) );
    jagDraw::DrawablePtr drawable( jagDraw::DrawablePtr( new jagDraw::Drawable() ) );
    jagSG::NodePtr node( jagSG::NodePtr( new jagSG::Node() ) );

    // Interleaved vertices and normals.
    jagBase::Point3fVec data;
    data.push_back( gmtl::Point3f( -1.f, -1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( 1.f, -1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( -1.f, 1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    data.push_back( gmtl::Point3f( 1.f, 1.f, 0.f ) + offset );
    data.push_back( gmtl::Point3f( 0.f, 0.f, 1.f ) );
    jagBase::BufferPtr ibp( new jagBase::Buffer( data.size() * sizeof( gmtl::Point3f ), (void*)&(data[0]) ) );
    jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    const GLsizei stride( sizeof( float ) * 6 );
    jagDraw::VertexAttribPtr verts( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    const GLsizei startOffset( sizeof( float ) * 3 );
    jagDraw::VertexAttribPtr norms( new jagDraw::VertexAttrib(
        "normal", 3, GL_FLOAT, GL_FALSE, stride, startOffset ) );

    jagDraw::VertexArrayObjectPtr vaop = jagDraw::VertexArrayObjectPtr( new jagDraw::VertexArrayObject() );
    vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( verts, jagDraw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( norms );
    commands->insert( vaop );

    drawable->addDrawCommand( jagDraw::DrawCommandPtr(
        new jagDraw::DrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) ) );

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


    jagDraw::CommandMapPtr commands( _scene->getCommandMap() );
    if( commands == NULL )
    {
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap() );
        _scene->setCommandMap( commands );
    }

    // Test bounding sphere computation.
    {
        _bSphere = _scene->getBound( *commands );
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

    jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "nodes.vert" ) );
    jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "nodes.frag" ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    commands->insert( prog );

    // Test uniform blocks
    jagDraw::UniformBlockPtr ubp( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "blockTest" ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambientScene", .2f ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuseMat", gmtl::Point3f( 0.f, .7f, 0.9f ) ) ) );
    jagDraw::UniformBlockSetPtr ubsp( new jagDraw::UniformBlockSet() );
    ubsp->insert( ubp );
    commands->insert( ubsp );

    gmtl::Vec3f lightVec( 0.5f, .7f, 1.f );
    gmtl::normalize( lightVec );
    jagDraw::UniformSetPtr usp( new jagDraw::UniformSet() );
    usp->insert( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ecLightDir", lightVec ) ) );
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
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    return( true );
}

bool NodesDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

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

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
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
