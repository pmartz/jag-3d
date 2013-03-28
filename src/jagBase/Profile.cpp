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

#include <jagBase/Profile.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include <vector>



namespace jagBase
{


#ifdef JAG3D_ENABLE_PROFILING



// TBD Need singleton manager to cleanup/delete singletons.
ProfileManager* ProfileManager::_s_instance( new jagBase::ProfileManager() );


ProfileManager::ProfileManager()
    : _root( new ProfileNode( "jag" ) ),
      _current( _root )
{
}
ProfileManager::~ProfileManager()
{
}

void ProfileManager::startProfile( const char* name )
{
    if( name != _current->_name )
        _current = _current->findChild( name );
    _current->startCall();
}
void ProfileManager::stopProfile()
{
    if( _current->endCall() )
        _current = _current->_parent;
}



ProfileNode::ProfileNode( const char* name )
    : _name( std::string( name ) ),
      _totalCalls( 0 ),
      _recursiveCount( 0 ),
      _totalTime(),
      _lastStart(),
      _lastEnd()

{
}
ProfileNode::~ProfileNode()
{
}

void ProfileNode::startCall()
{
    ++_totalCalls;
    if( _recursiveCount++ == 0 )
        _lastStart = boost::posix_time::microsec_clock::local_time();
}
bool ProfileNode::endCall()
{
    if( --_recursiveCount == 0 )
    {
        _lastEnd = boost::posix_time::microsec_clock::local_time();
        _totalTime += ( _lastEnd - _lastStart );
        return( true );
    }
    else
        return( false );
}

ProfileNodePtr ProfileNode::findChild( const char* name )
{
    std::string target( name );
    BOOST_FOREACH( ProfileNodePtr child, _children )
    {
        if( child->_name == target )
            return( child );
    }

    ProfileNodePtr newChild( new ProfileNode( name ) );
    _children.resize( _children.size() + 1 );
    _children[ _children.size() - 1 ] = newChild;
    newChild->_parent = shared_from_this();
    return( newChild );
}

void ProfileNode::reset()
{
    _totalCalls = 0;
    _recursiveCount = 0;
    _lastStart = _lastEnd = _totalTime = boost::posix_time::ptime();

    BOOST_FOREACH( ProfileNodePtr child, _children )
    {
        child->reset();
    }
}



// JAG3D_ENABLE_PROFILING
#endif


// jagBase
}
