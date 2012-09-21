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
#include <jagDisk/ReadWrite.h>
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

    virtual bool startup( const unsigned int numContexts );
    virtual bool init();
    virtual bool frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj );
    virtual void reshape( const int w, const int h );
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    gmtl::Matrix44f computeProjection( float aspect );
    void makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal );
    gmtl::Matrix33f computeNormalMatrix( const gmtl::Matrix44f& in );

    jagDraw::DrawableList _drawList;

    gmtl::Matrix44f _proj;
    osg::BoundingSphere _bs;

    jagDraw::PerContextData< jagDraw::UniformPtr > _viewProjUniform;
    jagDraw::PerContextData< jagDraw::UniformPtr > _normalUniform;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    return( new JagLoadDemo );
}

bool JagLoadDemo::startup( const unsigned int numContexts )
{
    //std::string fileName( "GRINDER_WHEEL.PRT.ive" );
    //std::string fileName( "M55339.ASM.ive" );
    //std::string fileName( "USMC23_4019.ASM.ive" );
    //std::string fileName( "02-1100.ive" );

    //std::string fileName( "fountain.osg" );
    //std::string fileName( "glider.osg" );
    //std::string fileName( "cow.osg" );
    //std::string fileName( "dumptruck.osg" );
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
        _drawList = osg2JagConverter.getJagDrawableList();
    }
    if( _drawList.size() == 0 )
    {
        JAG3D_FATAL_STATIC( _logName, "No Drawables from OSG conversion." );
        return( false );
    }

    jagDraw::DrawablePtr firstDrawable( _drawList[ 0 ] );

    jagDraw::ShaderPtr vs( (jagDraw::Shader*) jagDisk::read( "C:\\Projects\\JAG\\jag3d\\data\\jagload.vert" ) );
    jagDraw::ShaderPtr fs( (jagDraw::Shader*) jagDisk::read( "C:\\Projects\\JAG\\jag3d\\data\\jagload.frag" ) );

    jagDraw::ShaderProgramPtr prog;
    prog = jagDraw::ShaderProgramPtr( new jagDraw::Program );
    prog->attachShader( vs );
    prog->attachShader( fs );

    firstDrawable->insertDrawablePrep( prog );

    // Test uniform blocks
    jagDraw::UniformBlockPtr ubp( jagDraw::UniformBlockPtr(
        new jagDraw::UniformBlock( "blockTest" ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ambientScene", .2f ) ) );
    ubp->addUniform( jagDraw::UniformPtr(
        new jagDraw::Uniform( "diffuseMat", gmtl::Point3f( 0.f, .7f, 0.9f ) ) ) );
    firstDrawable->insertDrawablePrep( ubp );

    gmtl::Vec3f lightVec( 0.5, .7, 1. );
    gmtl::normalize( lightVec );
    firstDrawable->insertDrawablePrep( jagDraw::UniformPtr(
        new jagDraw::Uniform( "ecLightDir", lightVec ) ) );

    _proj = computeProjection( 1. );


    // Tell all Jag objects how many contexts to expect.
    BOOST_FOREACH( const jagDraw::DrawableList::value_type& dp, _drawList )
    {
        dp->setMaxContexts( numContexts );
    }
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

    // Display information on the type of context we created.
    string msg = string( "GL_VERSION: " );
    msg.append( (char*)(glGetString( GL_VERSION )) );
    JAG3D_INFO_STATIC( _logName, msg );

    return( true );
}

bool JagLoadDemo::frame( const gmtl::Matrix44f& view, const gmtl::Matrix44f& proj )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current Program
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

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
        const gmtl::Matrix44f viewProj( _proj * viewMat );
        _viewProjUniform[ drawInfo._id ]->set( viewProj );
        _normalUniform[ drawInfo._id ]->set( normalMat );
    }
    (*_viewProjUniform[ drawInfo._id ])( drawInfo );
    (*_normalUniform[ drawInfo._id ])( drawInfo );

    // Render all Drawables.
    BOOST_FOREACH( const jagDraw::DrawableList::value_type& dp, _drawList )
    {
        (*(dp))( drawInfo );
    }

    glFlush ();

    JAG3D_ERROR_CHECK( "jagload display()" );

    return( true );
}

void JagLoadDemo::reshape( const int w, const int h )
{
    _proj = computeProjection( (float)w/(float)h );
}

gmtl::Matrix44f JagLoadDemo::computeProjection( float aspect )
{
    if( !( _bs.valid() ) )
        return( gmtl::MAT_IDENTITY44F );

    gmtl::Matrix44f proj;
    float zNear = 3.5 * _bs.radius();
    float zFar = 5.75 * _bs.radius();
    gmtl::setPerspective< float >( proj, 30., aspect, zNear, zFar );

    return( proj );
}

void JagLoadDemo::makeViewMatrices( gmtl::Matrix44f& view, gmtl::Matrix33f& normal )
{
    const osg::Vec3 osgEye( _bs.center() + ( osg::Vec3( 0., -4., 1.5 ) * _bs.radius() ) );
    const gmtl::Point3f eye( osgEye[ 0 ], osgEye[ 1 ], osgEye[ 2 ] );
    const gmtl::Point3f center( _bs.center()[ 0 ], _bs.center()[ 1 ], _bs.center()[ 2 ] );
    const gmtl::Vec3f up( 0.f, 0.f, 1.f );

    gmtl::setLookAt( view, eye, center, up );
    normal = computeNormalMatrix( view );
}

gmtl::Matrix33f JagLoadDemo::computeNormalMatrix( const gmtl::Matrix44f& in )
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
