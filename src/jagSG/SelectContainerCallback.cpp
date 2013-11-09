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

#include <jagSG/SelectContainerCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>


namespace jagSG {


SelectContainerCallback::SelectContainerCallback( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jagSG::Node::Callback(),
    _index( 0 )
{
}
SelectContainerCallback::SelectContainerCallback( const unsigned int index, const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jagSG::Node::Callback(),
    _index( index )
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
