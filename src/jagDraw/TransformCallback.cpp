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


TransformCallback::TransformCallback()
    : Node::Callback(),
    _transform()
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
    if( drawInfo._transform == node.getTransform() )
        return( true );
    drawInfo._transform = node.getTransform();

    _transform.setModel( node.getTransform() );
    {
        gmtl::Matrix44f mvpMat;
        gmtl::convert( mvpMat, _transform.getModelViewProj() );
        jagDraw::UniformPtr& uniform( drawInfo.getOrCreateUniform( "jagModelViewProjMatrix" ) );
        uniform->setType( GL_FLOAT_MAT4 );
        uniform->set( mvpMat );
        uniform->executeWithoutMap( drawInfo );
    }
    {
        gmtl::Matrix33f mvitMat;
        gmtl::convert( mvitMat, _transform.getModelViewInvTrans() );
        jagDraw::UniformPtr& uniform( drawInfo.getOrCreateUniform( "jagModelViewInvTransMatrix" ) );
        uniform->setType( GL_FLOAT_MAT3 );
        uniform->set( mvitMat );
        uniform->executeWithoutMap( drawInfo );
    }
    _transform.setDirty( 0 );

    return( true );
}


// jagDraw
}
