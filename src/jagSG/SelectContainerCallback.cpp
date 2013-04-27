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

#include <jagSG/SelectContainerCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>


namespace jagSG {


SelectContainerCallback::SelectContainerCallback()
  : jagBase::LogBase( "jag.sg.coll.select" ),
    jagSG::Node::Callback(),
    _index( 0 )
{
}
SelectContainerCallback::SelectContainerCallback( const SelectContainerCallback& rhs )
  : jagBase::LogBase( "jag.sg.coll.select" ),
    jagSG::Node::Callback( rhs ),
    _index( rhs._index )
{
}
SelectContainerCallback::~SelectContainerCallback()
{
}


void SelectContainerCallback::setContainer( const unsigned int index )
{
    _index = index;
}
unsigned int SelectContainerCallback::getContainer() const
{
    return( _index );
}

bool SelectContainerCallback::operator()( jagSG::VisitorBase* visitor, jagSG::Node::CallbackInfo* info )
{
    jagSG::CollectionVisitor* cv( static_cast<
        jagSG::CollectionVisitor* >( visitor ) );

    const unsigned int savedIndex( cv->getCurrentNodeContainer() );
    cv->setCurrentNodeContainer( _index );
    cv->collectAndTraverse( *( info->getNode() ) );
    cv->setCurrentNodeContainer( savedIndex );

    // Don't continue traversal, as we already did that.
    return( false );
}


// jagSG
}
