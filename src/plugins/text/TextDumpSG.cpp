/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
