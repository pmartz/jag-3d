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

#include <jag/draw/CommandNode.h>
#include <jag/draw/CommandMap.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>

#include <boost/foreach.hpp>


namespace jag {
namespace draw {


CommandNode::CommandNode( const std::string& logName )
    : jag::base::LogBase( logName.empty() ? "jag.draw.cg.node" : logName ),
    jag::base::UserDataOwner(),
    _parent( NULL ),
    _dirty( true )
{
}
CommandNode::CommandNode( CommandNode* parent, const CommandMapPtr& commands, const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.cg.node" : logName ),
  jag::base::UserDataOwner(),
  _parent( parent ),
  _dirty( true ),
  _commands( commands )
{
}
CommandNode::CommandNode( const CommandNode& rhs )
  : jag::base::LogBase( rhs ),
    jag::base::UserDataOwner( rhs ),
    _parent( rhs._parent ),
    _children( rhs._children ),
    _dirty( true ),
    _commands( rhs._commands ),
    _accumulation( rhs._accumulation )
{
}
CommandNode::~CommandNode()
{
}

CommandNode* CommandNode::getParent()
{
    return( _parent );
}

CommandNode* CommandNode::findOrCreateChild( CommandMapPtr commands )
{
    ChildMap::const_iterator it( _children.find( commands.get() ) );
    if( it != _children.end() )
    {
        // TBD if dirty, or if parent dirty, accumulate
        return( it->second.get() );
    }
    
    CommandNodePtr newChild;
    newChild.reset( new CommandNode( this, commands ) );
    commands->addParent( newChild );
    _children[ commands.get() ] = newChild;

    return( newChild.get() );
}

void CommandNode::setDirty( const bool dirty )
{
    _dirty = dirty;

    if( _dirty )
    {
        // Dirty all children.
        BOOST_FOREACH( ChildMap::value_type dataPair, _children )
            dataPair.second->setDirty( true );
    }
}
bool CommandNode::getDirty() const
{
    return( _dirty );
}

void CommandNode::accumulate()
{
    if( _parent == NULL )
    {
        if( _accumulation == NULL )
            _accumulation = _commands;
        else if( _accumulation != _commands )
            *_accumulation = *_commands;
    }
    else
    {
        if( _accumulation == NULL )
            _accumulation.reset( new jag::draw::CommandMap() );
        const CommandMapPtr& parentCommands( _parent->getAccumulation() );
        if( parentCommands == NULL )
            *_accumulation = *_commands;
        else
            *_accumulation = *( parentCommands ) + *_commands;
    }
    _dirty = false;
}

CommandMapPtr CommandNode::getAccumulation()
{
    if( _dirty )
        accumulate();
    return( _accumulation );
}


// namespace jag::draw::
}
}
