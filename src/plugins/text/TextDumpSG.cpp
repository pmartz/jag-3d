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

#include "TextDumpSG.h"
#include <jagBase/LogMacros.h>
#include <Poco/Message.h>

#include <fstream>


TextDumpSG::TextDumpSG( std::ostream& oStr )
  : Visitor( "text" ),
  _oStr( oStr ),
  _indent( 0 )
{
}
TextDumpSG::~TextDumpSG()
{
}

void TextDumpSG::visit( jagSG::Node& node )
{
    std::string indentStr;
    for( int idx = 0; idx < _indent; ++idx )
        indentStr += "  ";

    const bool verbose( _logger.getLevel() < Poco::Message::Priority::PRIO_NOTICE );

    if( verbose )
    {
        _oStr << indentStr << "----------------" << std::endl;
        _oStr << indentStr << "Node \"" << node.getUserDataName() << "\"" << std::endl;
        if( node.getNumDrawables() > 0 )
            _oStr << indentStr << "- Num drawables: " << node.getNumDrawables() << std::endl;
        if( node.getNumChildren() > 0 )
            _oStr << indentStr << "- Num children: " << node.getNumChildren() << std::endl;
        if( node.getTransform().mState != gmtl::Matrix44d::IDENTITY )
            _oStr << indentStr << "- Transformed" << std::endl;
        if( node.getCommandMap() != NULL )
            _oStr << indentStr << "- Commands" << std::endl;
    }
    else {
        _oStr << indentStr << "Node \"" << node.getUserDataName() << "\", " <<
            node.getNumDrawables() << " drawables, " <<
            node.getNumChildren() << " children." <<
            ( ( node.getTransform().mState != gmtl::Matrix44d::IDENTITY ) ? " T" : "" ) <<
            ( ( node.getCommandMap() != NULL ) ? " CM" : "" ) <<
            std::endl;
    }

    ++_indent;
    Visitor::visit( node );
    --_indent;
}
