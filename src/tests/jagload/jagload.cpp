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
#include <jagDraw/PerContextData.h>
#include <jagBase/Transform.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <jagDisk/ReadWrite.h>
#include <boost/program_options/options_description.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include "osg2jag.h"

#include <osgDB/ReadFile>

#include <string>
#include <sstream>

#include <osg/Matrix>
#include <osg/io_utils>


using namespace std;
namespace bpo = boost::program_options;


class JagLoadDemo : public DemoInterface
{
public:
    JagLoadDemo()
      : DemoInterface( "jag.demo.jagload" )
    {}
    virtual ~JagLoadDemo() {}

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    gmtl::Matrix44d computeProjection( double aspect );
    gmtl::Matrix44d computeView();

    jagDraw::NodeContainer _drawNodes;

    typedef jagDraw::PerContextData< gmtl::Matrix44d > PerContextMatrix44d;
    PerContextMatrix44d _proj;
    osg::BoundingSphere _bs;

    jagDraw::PerContextData< jagDraw::UniformPtr > _viewProjUniform;
    jagDraw::PerContextData< jagDraw::UniformPtr > _normalUniform;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioDebug, jagBase::Log::Console );

    return( new JagLoadDemo );
}

bool JagLoadDemo::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    std::string fileName( "teapot.osg" );
    JAG3D_INFO_STATIC( _logName, fileName );

    if( fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--load <fileName>' on command line." );
        return( false );
    }

    {
        osg::ref_ptr< osg::Node> root = osgDB::readNodeFile( fileName );
        if( !root.valid() )
        {
            std::string msg( "Can't load \"" + fileName + "\"." );
            JAG3D_FATAL_STATIC( _logName, msg );
            return( false );
        }
        _bs = root->getBound();

        Osg2Jag osg2JagConverter;
        root->accept( osg2JagConverter );
        _drawNodes = osg2JagConverter.getDrawNodeContainer();
    }
    if( _drawNodes.size() == 0 )
    {
        JAG3D_FATAL_STATIC( _logName, "No DrawNodes from OSG conversion." );
        return( false );
    }

    jagDraw::Node& firstDrawNode( _drawNodes[ 0 ] );

    jagDraw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagload.vert" ) );
    jagDraw::ShaderPtr fs( DemoInterface::readShaderUtil( "jagload.frag" ) );

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( firstDrawNode.getCommandMap() );
    if( commands == NULL )
    {
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap() );
        firstDrawNode.setCommandMap( commands );
    }
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

    gmtl::Vec3f lightVec( 0.5, .7, 1. );
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
    _drawNodes.setMaxContexts( numContexts );
    for( unsigned int idx=0; idx<numContexts; ++idx )
    {
        _viewProjUniform._data.push_back( jagDraw::UniformPtr(
            new jagDraw::Uniform( "viewProjectionMatrix", gmtl::MAT_IDENTITY44F ) ) );
        _normalUniform._data.push_back( jagDraw::UniformPtr(
            new jagDraw::Uniform( "normalMatrix", gmtl::MAT_IDENTITY33F ) ) );
    }


    return( true );
}

bool JagLoadDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    return( true );
}

bool JagLoadDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Systems such as VRJ will pass view and projection matrices.
    jagBase::TransformD transformInfo;
    if( view.mState != gmtl::Matrix44f::IDENTITY || proj.mState != gmtl::Matrix44f::IDENTITY )
    {
        transformInfo.setProj( proj );
        transformInfo.setView( view );
    }
    else
    {
        transformInfo.setProj( _proj._data[ contextID ] );
        transformInfo.setView( computeView() );
    }
    gmtl::Matrix44f tempMat4;
    gmtl::convert( tempMat4, transformInfo.getViewProj() );
    _viewProjUniform[ drawInfo._id ]->set( tempMat4 );
    _viewProjUniform[ drawInfo._id ]->execute( drawInfo );
    gmtl::Matrix33f tempMat3;
    gmtl::convert( tempMat3, transformInfo.getModelViewInvTrans() );
    _normalUniform[ drawInfo._id ]->set( tempMat3 );
    _normalUniform[ drawInfo._id ]->execute( drawInfo );

    // Render all Drawables.
    _drawNodes.execute( drawInfo );

    glFlush();

    JAG3D_ERROR_CHECK( "jagload display()" );

    return( true );
}

void JagLoadDemo::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    _proj._data[ contextID ] = computeProjection( (double)w/(double)h );
}

gmtl::Matrix44d JagLoadDemo::computeProjection( double aspect )
{
    if( !( _bs.valid() ) )
        return( gmtl::MAT_IDENTITY44D );

    gmtl::Matrix44d proj;
    const double zNear = 3.5 * _bs.radius();
    const double zFar = 5.75 * _bs.radius();
    gmtl::setPerspective< double >( proj, 30., aspect, zNear, zFar );

    return( proj );
}

gmtl::Matrix44d JagLoadDemo::computeView()
{
    const osg::Vec3 osgEye( _bs.center() + ( osg::Vec3( 1.5, -4., 1.5 ) * _bs.radius() ) );
    const gmtl::Point3d eye( osgEye[ 0 ], osgEye[ 1 ], osgEye[ 2 ] );
    const gmtl::Point3d center( _bs.center()[ 0 ], _bs.center()[ 1 ], _bs.center()[ 2 ] );
    const gmtl::Vec3d up( 0., 0., 1. );

    gmtl::Matrix44d view;
    gmtl::setLookAt( view, eye, center, up );
    return( view );
}
