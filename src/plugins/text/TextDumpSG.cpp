/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

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

    const bool verbose( _logger.getLevel() < Poco::Message::PRIO_NOTICE );

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
