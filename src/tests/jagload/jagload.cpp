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


using namespace std;
namespace bpo = boost::program_options;


class JagLoadDemo : public DemoInterface
{
public:
    JagLoadDemo()
      : DemoInterface( "jag.demo.uniform" )
    {}
    virtual ~JagLoadDemo() {}

    virtual bool startup();
    virtual bool init();
    virtual bool frame();
    virtual bool shutdown()
    {
        return( true );
    }

protected:
    Osg2Jag osg2JagConverter;
};


DemoInterface* DemoInterface::create( bpo::options_description& desc )
{
    return( new JagLoadDemo );
}

bool JagLoadDemo::startup()
{
    jagBase::Log::instance()->setPriority( jagBase::Log::PrioTrace, jagBase::Log::Console );

    std::string fileName( "teapot.osg" );
    JAG3D_NOTICE_STATIC( "jag.demo.jagload", fileName );

    if( fileName.empty() )
    {
        JAG3D_FATAL_STATIC( "jag.demo.jagload", "Specify '--load <fileName>' on command line." );
        return( false );
    }

    osg::ref_ptr< osg::Node> root = osgDB::readNodeFile( fileName );
    if( !root.valid() )
    {
        std::string msg( "Can't load \"" + fileName + "\"." );
        JAG3D_FATAL_STATIC( "jag.demo.jagload", msg );
        return( false );
    }

    root->accept( osg2JagConverter );


    return( true );
}

bool JagLoadDemo::init()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );

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
    glClear( GL_COLOR_BUFFER_BIT );

    // drawInfo stores the contextID (used by many Jag objects to
    // look up their object ID), and the current ShaderProgram
    // (used by vertex attribs and uniforms to look up their locations).
    jagDraw::DrawInfo drawInfo;
    drawInfo._id = jagDraw::ContextSupport::instance()->getActiveContext();

    // Render all Drawables.
    const jagDraw::DrawableList& drawables( osg2JagConverter.getJagDrawableList() );
    BOOST_FOREACH( const jagDraw::DrawableList::value_type& dp, drawables )
    {
        (*(dp))( drawInfo );
    }


    glFlush ();

    JAG3D_ERROR_CHECK( "uniform display()" );

    return( true );
}
