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

#include <jagSG/ExecuteVisitor.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>

#include <sstream>


namespace jagSG {


ExecuteVisitor::ExecuteVisitor( jagDraw::DrawInfo& drawInfo )
  : Visitor( "exec" ),
    _drawInfo( drawInfo )
{
    reset();
}
ExecuteVisitor::ExecuteVisitor( jagDraw::DrawInfo& drawInfo, jagSG::Node& node )
  : Visitor( "exec", node ),
    _drawInfo( drawInfo )
{
    reset();
}
ExecuteVisitor::ExecuteVisitor( const ExecuteVisitor& rhs )
  : Visitor( rhs ),
    _drawInfo( rhs._drawInfo )
{
}
ExecuteVisitor::~ExecuteVisitor()
{
}


void ExecuteVisitor::reset()
{
    JAG3D_TRACE( "reset()" );

    _commandStack.clear();
    _commandStack.push_back( jagDraw::CommandMap() );
}


void ExecuteVisitor::visit( jagSG::Node& node )
{
    JAG3D_TRACE( "visit()" );

    // Push commands on stack.
    jagDraw::CommandMapPtr commands( node.getCommandMap() );
    jagDraw::CommandMap newTop( ( commands != NULL ) ?
        _commandStack.back() + *commands : _commandStack.back() );
    _commandStack.push_back( newTop );

    if( JAG3D_LOG_DEBUG )
    {
        std::ostringstream ostr;
        ostr << "\tCommand stack depth: " << _commandStack.size();
        JAG3D_DEBUG( ostr.str() );
    }

    newTop.execute( _drawInfo );

    // Execute the drawables
    node.execute( _drawInfo );

    Visitor::visit( node );

    _commandStack.pop_back();
}


// jagSG
}
