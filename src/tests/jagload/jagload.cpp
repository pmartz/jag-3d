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

#include <jagDraw/DrawCommon.h>
#include <jagBase/Version.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <Poco/Message.h>
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

    virtual bool startup();
    virtual bool init();
    virtual bool frame();
    virtual void reshape( const int w, const int h );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    gmtl::Matrix44f computeProjection( float aspect );
    void makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal );

    jagDraw::DrawableList _drawList;

    gmtl::Matrix44f _proj;
    jagDraw::UniformPtr _viewProjUniform;
    osg::BoundingSphere _bs;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    return( new JagLoadDemo );
}

bool JagLoadDemo::startup()
{
    //std::string fileName( "GRINDER_WHEEL.PRT.ive" );
    std::string fileName( "M55339.ASM.ive" );
    //std::string fileName( "USMC23_4019.ASM.ive" );
    //std::string fileName( "02-1100.ive" );

    //std::string fileName( "fountain.osg" );
    //std::string fileName( "glider.osg" );
    //std::string fileName( "cow.osg" );
    //std::string fileName( "dumptruck.osg" );
    //std::string fileName( "teapot.osg" );
    JAG3D_INFO_STATIC( "jag.demo.jagload", fileName );

    if( fileName.empty() )
    {
        JAG3D_FATAL_STATIC( "jag.demo.jagload", "Specify '--load <fileName>' on command line." );
        return( false );
    }

    {
        osg::ref_ptr< osg::Node> root = osgDB::readNodeFile( fileName );
        if( !root.valid() )
        {
            std::string msg( "Can't load \"" + fileName + "\"." );
            JAG3D_FATAL_STATIC( "jag.demo.jagload", msg );
            return( false );
        }
        _bs = root->getBound();

        Osg2Jag osg2JagConverter;
        root->accept( osg2JagConverter );
        _drawList = osg2JagConverter.getJagDrawableList();
    }
    if( _drawList.size() == 0 )
    {
        JAG3D_FATAL_STATIC( "jag.demo.jagload", "No Drawables from OSG conversion." );
        return( false );
    }

    jagDraw::DrawablePtr firstDrawable( _drawList[ 0 ] );

    const char* vShaderSource =
        "#version 140 \n"
        " \n"
        "uniform mat4 viewProjectionMatrix; \n"
        "uniform mat3 normalMatrix; \n"
        "uniform vec3 ecLightDir; \n"
        " \n"
        "in vec4 vertex; \n"
        "in vec3 normal; \n"
        "out vec4 color; \n"
        " \n"
        "void main() \n"
        "{ \n"
        "    vec3 ecNormal = normalize( normalMatrix * normal ); \n"
        "    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); \n"
        "    color = vec4( vec3(diffuse), 1. ); \n"
        " \n"
        "    gl_Position = viewProjectionMatrix * vertex; \n"
        "} \n";
    jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
    vs->addSourceString( std::string( vShaderSource ) );

    const char* fShaderSource =
        "#version 140 \n"
        " \n"
        "in vec4 color; \n"
        "out vec4 fragData; \n"
        " \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    jagDraw::ShaderProgramPtr prog;
    prog = jagDraw::ShaderProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    firstDrawable->addDrawablePrep( prog );

    gmtl::Vec3f lightVec( 0.5, .7, 1. );
    gmtl::normalize( lightVec );
    firstDrawable->addDrawablePrep( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ecLightDir", lightVec ) ) );

    _proj = computeProjection( 1. );

    gmtl::Matrix44f viewMat;
    gmtl::Matrix33f normalMat;
    makeViewMatrices( viewMat, normalMat );
    const gmtl::Matrix44f viewProj( _proj * viewMat );
    _viewProjUniform = jagDraw::UniformPtr(
        new jagDraw::Uniform( "viewProjectionMatrix", viewProj ) );
    firstDrawable->addDrawablePrep( _viewProjUniform );

    firstDrawable->addDrawablePrep( jagDraw::UniformPtr( new jagDraw::Uniform(
        "normalMatrix", normalMat ) ) );


    // Tell all Jag objects how many contexts to expect.
    const unsigned int numContexts( jagDraw::ContextSupport::instance()->getNumRegisteredContexts() );
    BOOST_FOREACH( const jagDraw::DrawableList::value_type& dp, _drawList )
    {
        dp->setMaxContexts( numContexts );
    }


    return( true );
}

bool JagLoadDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    glEnable( GL_DEPTH_TEST );

    // Auto-log the version string.
    jagBase::getVersionString();

    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    JAG3D_INFO_STATIC( _logName, msg );

    return( true );
}

bool JagLoadDemo::frame()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current Program
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

    // Render all Drawables.
    BOOST_FOREACH( const jagDraw::DrawableList::value_type& dp, _drawList )
    {
        (*(dp))( drawInfo );
    }

    glFlush ();

    JAG3D_ERROR_CHECK( "uniform display()" );

    return( true );
}

void JagLoadDemo::reshape( const int w, const int h )
{
    if( _viewProjUniform == NULL )
        return;

    _proj = computeProjection( (float)w/(float)h );

    gmtl::Matrix44f viewMat;
    gmtl::Matrix33f normalMat;
    makeViewMatrices( viewMat, normalMat );
    const gmtl::Matrix44f viewProj( _proj * viewMat );
    _viewProjUniform->set( viewProj );
}

gmtl::Matrix44f JagLoadDemo::computeProjection( float aspect )
{
    gmtl::Matrix44f proj;
    float zNear = 3.5 * _bs.radius();
    float zFar = 5.75 * _bs.radius();
    gmtl::setPerspective< float >( proj, 30., aspect, zNear, zFar );

    return( proj );
}

void JagLoadDemo::makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal )
{
    osg::Matrix m( osg::Matrix::lookAt( _bs.center() + ( osg::Vec3( 0., -4., 1.5 ) * _bs.radius() ),
        _bs.center(), osg::Vec3( 0., 0., 1. ) ) );
//    osg::Matrix m( osg::Matrix::lookAt( _bs.center() + ( osg::Vec3( -.5, -4., 0. ) * _bs.radius() ),
//        _bs.center(), osg::Vec3( -1., 0., 0. ) ) );

    gmtl::Matrix44f v;
    unsigned int r, c;
    for( r=0; r<4; r++ )
        for( c=0; c<4; c++ )
            v( r, c ) = m( c, r );
    view = v;

    normal( 0, 0 ) = v( 0, 0 );  normal( 0, 1 ) = v( 0, 1 );  normal( 0, 2 ) = v( 0, 2 );
    normal( 1, 0 ) = v( 1, 0 );  normal( 1, 1 ) = v( 1, 1 );  normal( 1, 2 ) = v( 1, 2 );
    normal( 2, 0 ) = v( 2, 0 );  normal( 2, 1 ) = v( 2, 1 );  normal( 2, 2 ) = v( 2, 2 );
}
