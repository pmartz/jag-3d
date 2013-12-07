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
#include <jag/draw/PerContextData.h>
#include <jag/base/Transform.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <jag/disk/ReadWrite.h>
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

protected:
    gmtl::Matrix44d computeProjection( double aspect );
    gmtl::Matrix44d computeView();

    jag::draw::DrawNodeContainer _drawNodes;

    typedef jag::draw::PerContextData< gmtl::Matrix44d > PerContextMatrix44d;
    PerContextMatrix44d _proj;
    osg::BoundingSphere _bs;

    jag::draw::PerContextData< jag::draw::UniformPtr > _viewProjUniform;
    jag::draw::PerContextData< jag::draw::UniformPtr > _normalUniform;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioDebug, jag::base::Log::Console );
    jag::base::Log::instance()->setPriority( jag::base::Log::PrioWarning, "jag.draw.ctx" );

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

    jag::draw::DrawNodePtr& firstDrawNode( _drawNodes[ 0 ] );

    jag::draw::ShaderPtr vs( DemoInterface::readShaderUtil( "jagload.vert" ) );
    jag::draw::ShaderPtr fs( DemoInterface::readShaderUtil( "jagload.frag" ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jag::draw::CommandMapPtr commands( firstDrawNode->getCommandMap() );
    if( commands == NULL )
    {
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
        firstDrawNode->setCommandMap( commands );
    }
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

    gmtl::Vec3f lightVec( 0.5, .7, 1. );
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
    _drawNodes.setMaxContexts( numContexts );
    for( unsigned int idx=0; idx<numContexts; ++idx )
    {
        _viewProjUniform._data.push_back( jag::draw::UniformPtr(
            new jag::draw::Uniform( "viewProjectionMatrix", gmtl::MAT_IDENTITY44F ) ) );
        _normalUniform._data.push_back( jag::draw::UniformPtr(
            new jag::draw::Uniform( "normalMatrix", gmtl::MAT_IDENTITY33F ) ) );
    }


    return( true );
}

bool JagLoadDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    return( true );
}

bool JagLoadDemo::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    // Systems such as VRJ will pass view and projection matrices.
    jag::base::TransformD transformInfo;
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

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
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
