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

#include <jagDraw/DrawNode.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>
#include <jagBase/ptr.h>

#include <boost/foreach.hpp>


namespace jagDraw {


DrawNode::DrawNode( CommandMapPtr commands )
  : jagBase::LogBase( "jag.draw.dgrp" ),
    ObjectIDOwner(),
    _commands( commands )
{
}
DrawNode::DrawNode( const DrawNode& rhs )
  : jagBase::LogBase( "jag.draw.dgrp" ),
    ObjectIDOwner( rhs ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables )
{
}
DrawNode::~DrawNode()
{
}


void DrawNode::operator()( DrawInfo& drawInfo )
{
    CommandMap delta( drawInfo._current << (*_commands) );

    typedef std::map< CommandType, DrawablePrepPtr > MyMapTBD;
    BOOST_FOREACH( MyMapTBD::value_type dpPair, delta._data )
    {
        ( *(dpPair.second) )( drawInfo );
    }

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        (*drawable)( drawInfo );
    }

    JAG3D_ERROR_CHECK( "DrawNode::operator()" );
}

void DrawNode::setMaxContexts( const unsigned int numContexts )
{
    typedef std::map< CommandType, DrawablePrepPtr > MyMapTBD;
    BOOST_FOREACH( MyMapTBD::value_type dpPair, _commands->_data )
    {
        ObjectIDPtr objID( boost::dynamic_pointer_cast< ObjectID >( dpPair.second ) );
        if( objID != NULL )
            objID->setMaxContexts( numContexts );
        else
        {
            ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< ObjectIDOwner >( dpPair.second ) );
            if( objIDOwner != NULL )
                objIDOwner->setMaxContexts( numContexts );
        }
    }

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->setMaxContexts( numContexts );
    }
}


// jagDraw
}
