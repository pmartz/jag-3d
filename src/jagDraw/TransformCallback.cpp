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

#include <jagDraw/TransformCallback.h>
#include <jagDraw/DrawInfo.h>


namespace jagDraw {


TransformCallback::TransformCallback( const jagBase::TransformD& transform )
    : Node::Callback(),
    _transform( transform )
{
}
TransformCallback::TransformCallback( const TransformCallback& rhs )
    : Node::Callback( rhs ),
    _transform( rhs._transform )
{
}
TransformCallback::~TransformCallback()
{
}


bool TransformCallback::operator()( jagDraw::Node& node, jagDraw::DrawInfo& drawInfo )
{
    jagDraw::UniformSet us;

    _transform.setModel( node.getTransform() );
    {
        gmtl::Matrix44f mvpMat;
        gmtl::convert( mvpMat, _transform.getModelViewProj() );
        jagDraw::UniformPtr modelViewProj( new jagDraw::Uniform( "jagModelViewProjMatrix", mvpMat ) );
        us.insert( modelViewProj );
    }
    {
        gmtl::Matrix33f mvitMat;
        gmtl::convert( mvitMat, _transform.getModelViewInvTrans() );
        jagDraw::UniformPtr modelViewInvTrans( new jagDraw::Uniform( "jagModelViewInvTransMatrix", mvitMat ) );
        us.insert( modelViewInvTrans );
    }
    _transform.setDirty( 0 );

    us.execute( drawInfo );

    return( true );
}


// jagDraw
}
