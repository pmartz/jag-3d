/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
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

#include <jagSG/SelectContainerCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>


namespace jagSG {


SelectContainerCallback::SelectContainerCallback( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jagSG::Node::Callback(),
    _index( 0 )
{
}
SelectContainerCallback::SelectContainerCallback( const unsigned int index, const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jagSG::Node::Callback(),
    _index( index )
{
}
SelectContainerCallback::SelectContainerCallback( const SelectContainerCallback& rhs )
  : jag::base::LogBase( "jag.sg.coll.select" ),
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
