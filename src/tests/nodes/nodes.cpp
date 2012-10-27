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
#include <jagDraw/PerContextData.h>
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
    virtual bool frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj );
    virtual void reshape( const int w, const int h );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    jagSG::NodePtr makeScene( const gmtl::Point3f& offset );

    gmtl::Matrix44f computeProjection( float aspect );
    void makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal );
    gmtl::Matrix33f computeNormalMatrix( const gmtl::Matrix44f& in );

    jagSG::NodePtr _scene;

    typedef jagDraw::PerContextData< gmtl::Matrix44f > PerContextMatrix44f;
    PerContextMatrix44f _proj;
    jagDraw::BoundPtr _bSphere;

    jagDraw::PerContextData< jagDraw::UniformPtr > _viewProjUniform;
    jagDraw::PerContextData< jagDraw::UniformPtr > _normalUniform;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    return( new NodesDemo );
}

jagSG::NodePtr NodesDemo::makeScene( const gmtl::Point3f& offset )
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

    return( node );
}

bool NodesDemo::startup( const unsigned int numContexts )
{
    _scene = makeScene( gmtl::Point3f( 0.f, 1.f, .5f ) );
    _scene->addChild( makeScene( gmtl::Point3f( -1.f, 0.f, 0.f ) ) );
    _scene->addChild( makeScene( gmtl::Point3f( 1.1f, -.25f, 0.f ) ) );


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
            JAG3D_INFO_STATIC( _logName, "Bound: Uninitialized." );
        }
        else
        {
            std::ostringstream ostr;
            ostr << "Bound: " << s.mCenter << " " << s.mRadius;
            JAG3D_INFO_STATIC( _logName, ostr.str() );
        }
    }

    jagDraw::ShaderPtr vs( (jagDraw::Shader*) jagDisk::read( "nodes.vert" ) );
    jagDraw::ShaderPtr fs( (jagDraw::Shader*) jagDisk::read( "nodes.frag" ) );
    if( ( vs == NULL ) || ( fs == NULL ) )
    {
        JAG3D_INFO_STATIC( _logName, "Unable to load shaders. Set JAG3D_DATA_PATH in the environment." );
        return( false );
    }

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
    commands->insert( ubp );

    gmtl::Vec3f lightVec( 0.5f, .7f, 1.f );
    gmtl::normalize( lightVec );
    commands->insert( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ecLightDir", lightVec ) ) );


    // We keep a different project matrix per context (to support different
    // window sizes). Initialize them all to a reasonable default.
    const gmtl::Matrix44f defaultProjMat( computeProjection( 1. ) );
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
        _proj._data.push_back( defaultProjMat );


    // Tell all Jag3D objects how many contexts to expect.
    _scene->setMaxContexts( numContexts );
    for( unsigned int idx=0; idx<numContexts; ++idx )
    {
        _viewProjUniform._data.push_back( jagDraw::UniformPtr(
            new jagDraw::Uniform( "viewProjectionMatrix", gmtl::MAT_IDENTITY44F ) ) );
        _normalUniform._data.push_back( jagDraw::UniformPtr(
            new jagDraw::Uniform( "normalMatrix", gmtl::MAT_IDENTITY33F ) ) );
    }


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

bool NodesDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Systems such as VRJ will pass view and projection matrices.
    if( view.mState != gmtl::Matrix44f::IDENTITY || proj.mState != gmtl::Matrix44f::IDENTITY )
    {
        const gmtl::Matrix44f viewProj( proj * view );
        _viewProjUniform[ drawInfo._id ]->set( viewProj );
        gmtl::Matrix33f normalMat( computeNormalMatrix( view ) );
        _normalUniform[ drawInfo._id ]->set( normalMat );
    }
    else
    {
        gmtl::Matrix44f viewMat;
        gmtl::Matrix33f normalMat;
        makeViewMatrices( viewMat, normalMat );
        const gmtl::Matrix44f viewProj( _proj._data[ contextID ] * viewMat );
        _viewProjUniform[ drawInfo._id ]->set( viewProj );
        _normalUniform[ drawInfo._id ]->set( normalMat );
    }
    _viewProjUniform[ drawInfo._id ]->execute( drawInfo );
    _normalUniform[ drawInfo._id ]->execute( drawInfo );

    // Render all Drawables.
    _scene->execute( drawInfo );

    glFlush();

    JAG3D_ERROR_CHECK( "nodes display()" );

    return( true );
}

void NodesDemo::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    _proj._data[ contextID ] = computeProjection( (float)w/(float)h );
}

gmtl::Matrix44f NodesDemo::computeProjection( float aspect )
{
    const gmtl::Sphered s( _bSphere->asSphere() );

    if( s.getRadius() <= 0.f )
        return( gmtl::MAT_IDENTITY44F );

    gmtl::Matrix44f proj;
    const double zNear = 3.5 * s.getRadius();
    const double zFar = 5.75 * s.getRadius();
    gmtl::setPerspective< float >( proj, 30.f, aspect, (float)zNear, (float)zFar );

    return( proj );
}

void NodesDemo::makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal )
{
    const gmtl::Sphered s( _bSphere->asSphere() );
    const gmtl::Point3d c( s.getCenter() );
    const gmtl::Point3f center( (float)c[0], (float)c[1], (float)c[2] );
    const float radius( (float)s.getRadius() );

    const gmtl::Point3f eye( center + ( gmtl::Point3f( 1.5f, -4.f, 1.5f ) * radius ) );
    const gmtl::Vec3f up( 0.f, 0.f, 1.f );

    gmtl::setLookAt( view, eye, center, up );
    normal = computeNormalMatrix( view );
}

gmtl::Matrix33f NodesDemo::computeNormalMatrix( const gmtl::Matrix44f& in )
{
    gmtl::Matrix33f tempIn;
    for( unsigned int r=0; r<3; r++ )
        for( unsigned int c=0; c<3; c++ )
            tempIn( r, c ) = in( r, c );

#if 0
    // TBD
    // Normals convert to eye coords using the inversetranspose
    // of the upper left 3x3 of the modelview matrix.
    gmtl::Matrix33f invIn, result;
    gmtl::invert( invIn, tempIn );
    gmtl::transpose( result, invIn );
#endif

    return( tempIn );
}
