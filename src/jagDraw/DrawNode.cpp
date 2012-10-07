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
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


DrawNode::DrawNode()
  : jagBase::LogBase( "jag.draw.dgrp" ),
    ObjectIDOwner()
{
}
DrawNode::DrawNode( const DrawNode& rhs )
  : jagBase::LogBase( "jag.draw.dgrp" ),
    ObjectIDOwner( rhs ),
    _drawablePrep( rhs._drawablePrep ),
    _drawables( rhs._drawables )
{
}
DrawNode::~DrawNode()
{
}


void DrawNode::operator()( DrawInfo& drawInfo )
{
    BOOST_FOREACH( DrawablePrepPtr dpp, _drawablePrep )
    {
        (*dpp)( drawInfo );
    }

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        (*drawable)( drawInfo );
    }

    JAG3D_ERROR_CHECK( "DrawableGroup::operator()" );
}


void DrawNode::addDrawablePrep( DrawablePrepPtr dpp )
{
    _drawablePrep.push_back( dpp );
}
void DrawNode::insertDrawablePrep( DrawablePrepPtr dpp, unsigned int pos )
{
    if( pos >= _drawablePrep.size() )
    {
        _drawablePrep.resize( pos+1 );
    }
    else
    {
        _drawablePrep.resize( _drawablePrep.size()+1 );
        size_t idx;
        for( idx = _drawablePrep.size()-1; idx>pos; idx-- )
            _drawablePrep[ idx ] = _drawablePrep[ idx-1 ];
    }
    _drawablePrep[ pos ] = dpp;
}


// jagDraw
}
