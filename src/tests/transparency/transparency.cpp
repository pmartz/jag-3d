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
#include <jag/sg/Common.h>
#include <jagUtil/Shapes.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/Profile.h>
#include <jag/base/Version.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <jag/mx/MxCore.h>

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <string>
#include <sstream>


using namespace std;
namespace bpo = boost::program_options;


class Transparency : public DemoInterface
{
public:
    Transparency()
      : DemoInterface( "jag.ex.trans" ),
        _fileName( "cow.osg" ),
        _lastTime()
    {
        setContinuousRedraw();
    }
    virtual ~Transparency() {}

    virtual bool parseOptions( boost::program_options::variables_map& vm );

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    virtual void reshape( const int w, const int h );

protected:
    gmtl::Matrix44d computeView( jag::mx::MxCorePtr mxCore, const double angleRad );

    std::string _fileName;

    jag::sg::NodePtr _root;

    boost::chrono::high_resolution_clock::time_point _lastTime;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    desc.add_options()
        ( "file,f", bpo::value< std::string >(), "Model to load. Default: cow.osg" );

    return( new Transparency );
}

bool Transparency::parseOptions( bpo::variables_map& vm )
{
    if( vm.count( "file" ) > 0 )
        _fileName = vm[ "file" ].as< std::string >();
    return( true );
}


jag::sg::NodePtr createPlanesSubgraph( jag::draw::BoundPtr bound )
{
    jag::sg::NodePtr planeRoot = jag::sg::NodePtr( new jag::sg::Node() );

    // Instruct collection visitor to copy data store references
    // into a specific NodeContainer.
    jag::sg::SelectContainerCallbackPtr sccp( new jag::sg::SelectContainerCallback( 1 ) );
    planeRoot->getCollectionCallbacks().push_back( sccp );

    // Create container to store the plane vertex / normal / texcoord data.
    jagUtil::VNTCVec data;

    // Create the first plane and its data
    float radius( (float)( bound->getRadius() ) );
    gmtl::Point3f corner( radius, -radius, radius );
    gmtl::Vec3f uVec( 0.f, 0.f, -2.f*radius );
    gmtl::Vec3f vVec( 0.f, 2.f*radius, 0.f );
    jag::draw::DrawablePtr plane0( jagUtil::makePlane(
        data, corner, uVec, vVec ) );
    planeRoot->addDrawable( plane0 );

    // Create second plane and its data.
    corner = gmtl::Point3f( -radius, -radius, -radius );
    uVec = gmtl::Vec3f( 0.f, 0.f, 2.f*radius );
    jag::draw::DrawablePtr plane1( jagUtil::makePlane(
        data, corner, uVec, vVec, 4, 4 ) );
    // Must add as separate child node for proper depth sorting.
    jag::sg::NodePtr planeChild( jag::sg::NodePtr( new jag::sg::Node() ) );
    planeChild->addDrawable( plane1 );
    planeRoot->addChild( planeChild );

    // Put the data in an array buffer object, and add it to
    // a VertexArrayObject.
    jag::draw::CommandMapPtr commands( planeRoot->getCommandMap() );
    if( commands == NULL )
    {
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
        planeRoot->setCommandMap( commands );
    }
    commands->insert( jagUtil::createVertexArrayObject( data ) );

    return( planeRoot );
}

struct BlendCallback : public jag::draw::DrawNodeContainer::Callback
{
    virtual bool operator()( jag::draw::DrawNodeContainer& nodeContainer,
        jag::draw::DrawInfo& drawInfo )
    {
        glEnable( GL_BLEND );
        nodeContainer.internalExecute( drawInfo );
        glDisable( GL_BLEND );
        return( false );
    }
};


bool Transparency::startup( const unsigned int numContexts )
{
    DemoInterface::startup( numContexts );

    JAG3D_INFO_STATIC( _logName, _fileName );

    if( _fileName.empty() )
    {
        JAG3D_FATAL_STATIC( _logName, "Specify '--file <fileName>' on command line." );
        return( false );
    }


    // Prepare the draw graph.
    jag::draw::DrawGraphPtr drawGraphTemplate( new jag::draw::DrawGraph() );
    drawGraphTemplate->resize( 2 );
    (*drawGraphTemplate)[ 1 ].getCallbacks().push_back(
        jag::draw::DrawNodeContainer::CallbackPtr( new BlendCallback() ) );
    getCollectionVisitor().setDrawGraphTemplate( drawGraphTemplate );


    // Prepare the scene graph.
    _root = jag::sg::NodePtr( new jag::sg::Node() );

    jag::sg::NodePtr model( DemoInterface::readSceneGraphNodeUtil( _fileName ) );
    _root->addChild( model );

    _root->addChild( createPlanesSubgraph( model->getBound() ) );


    jag::draw::ShaderPtr vs( DemoInterface::readShaderUtil( "transparency.vert" ) );
    jag::draw::ShaderPtr fs( DemoInterface::readShaderUtil( "transparency.frag" ) );

    jag::draw::ProgramPtr prog;
    prog = jag::draw::ProgramPtr( new jag::draw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    jag::draw::CommandMapPtr commands( _root->getCommandMap() );
    if( commands == NULL )
    {
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
        _root->setCommandMap( commands );
    }
    commands->insert( prog );

    // Test uniform blocks
    jag::draw::UniformBlockPtr ubp( jag::draw::UniformBlockPtr(
        new jag::draw::UniformBlock( "blockTest" ) ) );
    ubp->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ambientScene", .2f ) ) );
    ubp->addUniform( jag::draw::UniformPtr(
        new jag::draw::Uniform( "diffuseMat", gmtl::Point3f( 0.f, .7f, 0.9f ) ) ) );
    jag::draw::UniformBlockSetPtr ubsp( jag::draw::UniformBlockSetPtr(
        new jag::draw::UniformBlockSet() ) );
    ubsp->insert( ubp );
    commands->insert( ubsp );

    gmtl::Vec3f lightVec( .5f, .7f, 1.f );
    gmtl::normalize( lightVec );
    jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr(
        new jag::draw::UniformSet() ) );
    usp->insert( jag::draw::UniformPtr(
        new jag::draw::Uniform( "ecLightDir", lightVec ) ) );
    commands->insert( usp );


    // We keep a different aspect ratio per context (to support different
    // window sizes). Initialize them all to a reasonable default.
    for( unsigned int idx( 0 ); idx<numContexts; ++idx )
    {
        jag::mx::MxCorePtr mxCore( new jag::mx::MxCore() );
        mxCore->setAspect( 1. );
        mxCore->setFovy( 30. );
        _mxCore._data.push_back( mxCore );
    }


    // Tell all Jag3D objects how many contexts to expect.
    _root->setMaxContexts( numContexts );


    return( true );
}

bool Transparency::init()
{
    glClearColor( 1.f, 1.f, 1.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Auto-log the version string.
    jag::base::getVersionString();

    // Auto-log the OpenGL version string.
    jag::draw::getOpenGLVersionString();

    return( true );
}

// Don't bother until we have something worth sorting.
#define ENABLE_SORT

bool Transparency::frame( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    if( !getStartupCalled() )
        return( true );

    JAG3D_PROFILE( "frame" );

#ifdef ENABLE_SORT
    jag::draw::Command::CommandTypeVec plist;
    plist.push_back( jag::draw::Command::UniformSet_t );
#endif

    boost::chrono::high_resolution_clock::time_point current( boost::chrono::high_resolution_clock::now() );
    const boost::chrono::high_resolution_clock::duration elapsed( current - _lastTime );
    _lastTime = current;
    const double elapsedSecs( elapsed.count() * 0.000000001 );
    const double rotation = elapsedSecs * gmtl::Math::TWO_PI / 4.; // 2*PI rads (360 degrees) every 4 secs.

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    jag::draw::DrawInfo& drawInfo( getDrawInfo( contextID ) );

    jag::mx::MxCorePtr mxCore( _mxCore._data[ contextID ] );

    jag::sg::CollectionVisitor& collect( getCollectionVisitor() );
    collect.reset();

    jag::draw::DrawGraphPtr drawGraph;
    {
        JAG3D_PROFILE( "Collection" );

        // Set view and projection to define the collection frustum.
        const gmtl::Matrix44d viewMatrix( computeView( mxCore, rotation ) );
        collect.setViewProj( viewMatrix, mxCore->computeProjection( .1, 500. ) );

        {
            JAG3D_PROFILE( "Collection traverse" );
            // Collect a draw graph.
            _root->accept( collect );
        }
        drawGraph = collect.getDrawGraph();

        // Set view and projection to use for drawing. Create projection using
        // the computed near and far planes.
        double minNear, maxFar;
        collect.getNearFar( minNear, maxFar );
        drawGraph->setViewProj( viewMatrix, mxCore->computeProjection( minNear, maxFar ) );

#ifdef ENABLE_SORT
        {
            JAG3D_PROFILE( "Collection sort" );
            {
                // Sort by commands.
                jag::draw::DrawNodeContainer& nc( (*drawGraph)[ 0 ] );
                std::sort( nc.begin(), nc.end(), jag::draw::DrawNodeCommandSorter( plist ) );
            }
            {
                // Sort by descending eye coord z distance.
                jag::draw::DrawNodeContainer& nc( (*drawGraph)[ 1 ] );
                jag::draw::DrawNodeDistanceSorter distanceSorter;
                std::sort( nc.begin(), nc.end(), distanceSorter );
            }
        }
#endif
    }

    {
        JAG3D_PROFILE( "Render" );

        // Execute the draw graph.
        drawGraph->execute( drawInfo );
    }

    glFlush();

    JAG3D_ERROR_CHECK( "jagmodel display()" );

    return( true );
}

void Transparency::reshape( const int w, const int h )
{
    if( !getStartupCalled() )
        return;

    const jag::draw::jagDrawContextID contextID( jag::draw::ContextSupport::instance()->getActiveContext() );
    _mxCore._data[ contextID ]->setAspect( ( double ) w / ( double ) h );
}

gmtl::Matrix44d Transparency::computeView( jag::mx::MxCorePtr mxCore, const double angleRad )
{
    mxCore->rotateOrbit( angleRad, gmtl::Vec3d( 0., 0., 1. ) );
    mxCore->lookAtAndFit( _root->getBound()->asSphere() );
    return( mxCore->getInverseMatrix() );
}
