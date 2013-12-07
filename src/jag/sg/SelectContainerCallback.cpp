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

#include <jag/sg/SelectContainerCallback.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/sg/CollectionVisitor.h>


namespace jag {
namespace sg {


SelectContainerCallback::SelectContainerCallback( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jag::sg::Node::Callback(),
    _index( 0 )
{
}
SelectContainerCallback::SelectContainerCallback( const unsigned int index, const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.select" : logName ),
    jag::sg::Node::Callback(),
    _index( index )
{
}
SelectContainerCallback::SelectContainerCallback( const SelectContainerCallback& rhs )
  : jag::base::LogBase( "jag.sg.coll.select" ),
    jag::sg::Node::Callback( rhs ),
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

bool SelectContainerCallback::operator()( jag::sg::VisitorBase* visitor, jag::sg::Node::CallbackInfo* info )
{
    jag::sg::CollectionVisitor* cv( static_cast<
        jag::sg::CollectionVisitor* >( visitor ) );

    const unsigned int savedIndex( cv->getCurrentNodeContainer() );
    cv->setCurrentNodeContainer( _index );
    cv->collectAndTraverse( *( info->getNode() ) );
    cv->setCurrentNodeContainer( savedIndex );

    // Don't continue traversal, as we already did that.
    return( false );
}


// namespace jag::sg::
}
}
