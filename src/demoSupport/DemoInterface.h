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

#ifndef __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__
#define __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__ 1


#include <jagDraw/DrawInfo.h>
#include <jagSG/CollectionVisitor.h>
#include <gmtl/gmtl.h>

#include <boost/program_options/options_description.hpp>
#include <boost/foreach.hpp>


/** \class DemoInterface DemoInterface.h DemoInterface.h
\brief Facilitates building examples, tests, and demos with any
windowing system API, e.g., freeglut, Qt, etc.
*/
class DemoInterface
{
public:
    DemoInterface( const std::string& logName )
      : _logName( logName ),
        _startupCalled( false ),
        _continuousRedraw( false )
    {}
    virtual ~DemoInterface() {}

    static DemoInterface* create( boost::program_options::options_description& desc );

    /** Called before any contexts are created. */
    virtual bool startup( const unsigned int numContexts )
    {
        _startupCalled = true;

        _drawInfo._data.resize( numContexts );
        unsigned int id( 0 );
        BOOST_FOREACH( jagDraw::DrawInfo& drawInfo, _drawInfo._data )
        {
            drawInfo._id = id++;
        }

        return( true );
    }
    bool getStartupCalled() { return( _startupCalled ); }

    bool getContinuousRedraw() { return( _continuousRedraw ); }

    /** Called after a context has been created. */
    virtual bool init() = 0;
    /** Called to render a frame. */
    virtual bool frame( const gmtl::Matrix44d& view=gmtl::MAT_IDENTITY44D, const gmtl::Matrix44d& proj=gmtl::MAT_IDENTITY44D ) = 0;
    /** Called on window resize. */
    virtual void reshape( const int w, const int h ) {}
    /** Called prior to exit. */
    virtual bool shutdown() = 0;

    jagDraw::DrawInfo& getDrawInfo( const jagDraw::jagDrawContextID contextID )
    {
        return( _drawInfo._data[ contextID ] );
    }
    jagSG::CollectionVisitor& getCollectionVisitor()
    {
        return( _collectionVisitor );
    }

protected:
    std::string _logName;
    bool _startupCalled;

    bool _continuousRedraw;

    jagDraw::PerContextDrawInfo _drawInfo;
    jagSG::CollectionVisitor _collectionVisitor;
};


// __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__
#endif
