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
#include <jagSG/CollectionVisitor.h>
#include <jagSG/Node.h>
#include <jagSG/SmallFeatureCallback.h>
#include <jagDisk/ReadWrite.h>
#include <jagBase/Profile.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <string>
#include <sstream>


using namespace std;
namespace bpo = boost::program_options;


class JagModel : public DemoInterface
{
public:
    JagModel()
      : DemoInterface( "jag.ex.jagmodel" ),
        _fileName( "cow.osg" )
    {}
    virtual ~JagModel() {}

    virtual bool parseOptions( boost::program_options::variables_map& vm );

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    gmtl::Matrix44d computeProjection( const double aspect, const double zNear=0.5, const double zFar=400. );
    gmtl::Matrix44d computeView();

    std::string _fileName;

    jagSG::NodePtr _root;

    typedef jagDraw::PerContextData< double > PerContextAspect;
    PerContextAspect _aspect;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    desc.add_options()
        ( "file,f", bpo::value< std::string >(), "Model to load. Default: cow.osg" );

    return( new JagModel );
}

bool JagModel::parseOptions( bpo::variables_map& vm )
{
    if( vm.count( "file" ) > 0 )
        _fileName = vm[ "file" ].as< std::string >();
    return( true );
}

bool JagModel::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    JAG3D_INFO_STATIC( _logName, _fileName );

    if( _fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--file <fileName>' on command line." );
        return( false );
    }


    // Prepare the draw graph.
    jagDraw::DrawGraphPtr drawGraphTemplate( new jagDraw::DrawGraph() );
    drawGraphTemplate->resize( 1 );
    getCollectionVisitor().setDrawGraphTemplate( drawGraphTemplate );


    // Prepare the scene graph.
    jagSG::Node* rawModel( (jagSG::Node*) jagDisk::read( _fileName ) );
    if( rawModel == NULL )
    {
        JAG3D_FATAL_STATIC( _logName, "Can't load \"" + _fileName + "\"." );
        return( false );
    }
    _root = boost::make_shared< jagSG::Node >(
        *rawModel );
        

    jagSG::SmallFeatureDistributionVisitor sfdv;
    _root->accept( sfdv );


    jagDraw::ShaderPtr vs( (jagDraw::Shader*) jagDisk::read( "jagmodel.vert" ) );
    jagDraw::ShaderPtr fs( (jagDraw::Shader*) jagDisk::read( "jagmodel.frag" ) );
    if( ( vs == NULL ) || ( fs == NULL ) )
    {
        JAG3D_INFO_STATIC( _logName, "Unable to load shaders. Set JAG3D_DATA_PATH in the environment." );
        return( false );
    }

    jagDraw::ProgramPtr prog;
    prog = jagDraw::ProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jagDraw::CommandMapPtr commands( _root->getCommandMap() );
    if( commands == NULL )
    {
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap() );
        _root->setCommandMap( commands );
    }
    commands->insert( prog );

    // Test uniform blocks
    jagDraw::UniformBlockPtr ubp( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "blockTest" ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambientScene", .2f ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuseMat", gmtl::Point3f( 0.f, .7f, 0.9f ) ) ) );
    jagDraw::UniformBlockSetPtr ubsp( jagDraw::UniformBlockSetPtr(
        new jagDraw::UniformBlockSet() ) );
    ubsp->insert( ubp );
    commands->insert( ubsp );

    gmtl::Vec3f lightVec( .5f, .7f, 1.f );
    gmtl::normalize( lightVec );
    jagDraw::UniformSetPtr usp( jagDraw::UniformSetPtr(
        new jagDraw::UniformSet() ) );
    usp->insert( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ecLightDir", lightVec ) ) );
    commands->insert( usp );


    // We keep a different aspect ratio per context (to support different
    // window sizes). Initialize them all to a reasonable default.
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
        _aspect._data.push_back( 1. );


    // Tell all Jag3D objects how many contexts to expect.
    _root->setMaxContexts( numContexts );


    return( true );
}

bool JagModel::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Auto-log the OpenGL version string.
    jagDraw::getOpenGLVersionString();

    return( true );
}

// Don't bother until we have something worth sorting.
#define ENABLE_SORT

bool JagModel::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    JAG3D_PROFILE( "frame" );

#ifdef ENABLE_SORT
    jagDraw::DrawablePrep::CommandTypeVec plist;
    plist.push_back( jagDraw::DrawablePrep::UniformSet_t );
#endif

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    jagDraw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jagSG::CollectionVisitor& collect( getCollectionVisitor() );
    collect.reset();

    gmtl::Matrix44d viewMatrix( view );
    gmtl::Matrix44d projMatrix( proj );
    {
        JAG3D_PROFILE( "Collection" );

        // Set view and projection to define the collection frustum.
        // Systems such as VRJ will pass view and projection matrices.
        if( view.mState == gmtl::Matrix44f::IDENTITY || proj.mState != gmtl::Matrix44f::IDENTITY )
        {
            // Not VRJ. Compute a view and projection.
            viewMatrix = computeView();
            projMatrix = computeProjection( _aspect._data[ contextID ] );
        }
        collect.setViewProj( viewMatrix, projMatrix );

        {
            JAG3D_PROFILE( "Collection traverse" );
            // Collect a draw graph.
            _root->accept( collect );
        }
#ifdef ENABLE_SORT
        {
            JAG3D_PROFILE( "Collection sort" );
            jagDraw::DrawGraphPtr drawGraph( collect.getDrawGraph() );
            BOOST_FOREACH( jagDraw::NodeContainer& nc, *drawGraph )
            {
                std::sort( nc.begin(), nc.end(), jagDraw::DrawNodeCommandSorter( plist ) );
            }
        }
#endif
    }

    {
        JAG3D_PROFILE( "Render" );

        // Execute the draw graph.
        jagDraw::DrawGraphPtr drawGraph( collect.getDrawGraph() );

        // Compute projection matrix to use for drawing, based on CollectionVisitor's
        // computed min/max Z values.
        double minZ, maxZ;
        collect.getNearFar( minZ, maxZ );
        projMatrix = computeProjection( _aspect._data[ contextID ], minZ, maxZ );
        drawGraph->setViewProj( viewMatrix, projMatrix );

        drawGraph->execute( drawInfo );
    }

    glFlush();

    JAG3D_ERROR_CHECK( "jagmodel display()" );

    return( true );
}

void JagModel::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    const jagDraw::jagDrawContextID contextID( jagDraw::ContextSupport::instance()->getActiveContext() );
    _aspect._data[ contextID ] = ( double ) w / ( double ) h;
}

gmtl::Matrix44d JagModel::computeProjection( const double aspect, const double zNear, const double zFar )
{
    gmtl::Matrix44d proj;
    gmtl::setPerspective< double >( proj, 30., aspect, zNear, zFar );

    return( proj );
}

gmtl::Matrix44d JagModel::computeView()
{
    const gmtl::Sphered s( _root->getBound()->asSphere() );
    const gmtl::Point3d center( s.getCenter() );
    const double radius( (float)s.getRadius() );

    const gmtl::Point3d eye( center + ( gmtl::Point3d( 1.5, -4., 1.5 ) * radius ) );
    const gmtl::Vec3d up( 0.f, 0.f, 1.f );

    gmtl::Matrix44d view;
    gmtl::setLookAt( view, eye, center, up );
    return( view );
}
