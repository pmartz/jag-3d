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


#include <gmtl/gmtl.h>

#include <boost/program_options/options_description.hpp>


/** \class DemoInterface DemoInterface.h DemoInterface.h
\brief Facilitates building examples, tests, and demos with any
windowing system API, e.g., freeglut, Qt, etc.
*/
class DemoInterface
{
public:
    DemoInterface( const std::string& logName )
      : _logName( logName )
    {}
    virtual ~DemoInterface() {}

    static DemoInterface* create( boost::program_options::options_description& desc );

    /** Called before any contexts are created. */
    virtual bool startup( const unsigned int numContexts ) = 0;
    /** Called after a context has been created. */
    virtual bool init() = 0;
    /** Called to render a frame. */
    virtual bool frame( const gmtl::Matrix44f& view=gmtl::MAT_IDENTITY44F, const gmtl::Matrix44f& proj=gmtl::MAT_IDENTITY44F ) = 0;
    /** Called on window resize. */
    virtual void reshape( const int w, const int h ) {}
    /** Called prior to exit. */
    virtual bool shutdown() = 0;

protected:
    std::string _logName;
};


// __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__
#endif
