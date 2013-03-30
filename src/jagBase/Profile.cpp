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
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <sstream>



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

void ProfileManager::dumpAll( const bool reset )
{
    ProfileDump dumper;
    dumper.visit( _root );

    if( reset )
        _root->reset();
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
    _totalTime = boost::posix_time::time_duration();
    _lastStart = _lastEnd = boost::posix_time::ptime();

    BOOST_FOREACH( ProfileNodePtr child, _children )
    {
        child->reset();
    }
}



ProfileVisitor::ProfileVisitor()
{
}
ProfileVisitor::~ProfileVisitor()
{
}

void ProfileVisitor::traverse( ProfileNodePtr node )
{
    if( node != NULL )
    {
        BOOST_FOREACH( ProfileNodePtr child, node->_children )
        {
            visit( child );
        }
    }
}


ProfileDump::ProfileDump()
    : ProfileVisitor(),
      jagBase::LogBase( "jag.prof" ),
      _depth( 0 )
{
}
ProfileDump::~ProfileDump()
{
}

#define AS_DBL_MS(__t) \
    ( (double)( __t.total_microseconds() ) * .001 )

void ProfileDump::visit( ProfileNodePtr node )
{
    const double myTime( AS_DBL_MS(node->_totalTime) );
    if( JAG3D_LOG_INFO )
    {
        Poco::LogStream logstream( _logger );

        std::ostringstream indent;
        for( unsigned int idx=0; idx<_depth; ++idx )
            indent << ". ";

        const std::streamsize oldPrec( logstream.precision( 3 ) );

        logstream.information() << indent.str() << "------------" << std::endl;
        logstream.information() << indent.str() << node->_name << ": " << myTime << "ms" << std::endl;

        if( node->_children.size() > 0 )
        {
            double runningTotal( 0. );
            unsigned int idx;
            for( idx=0; idx< node->_children.size(); ++idx )
            {
                ProfileNodePtr child( node->_children[ idx ] );
                const double childTime( AS_DBL_MS(child->_totalTime) );
                const double percent( ( myTime > 0.) ? childTime / myTime * 100. : 0. );
                runningTotal += childTime;
                logstream.information()<< indent.str() << idx << " - " <<
                    child->_name << ": " <<
                    childTime << " (" <<
                    percent << "%)" << std::endl;
            }
            const double unprofiled( myTime - runningTotal );
            const double percent( ( myTime > 0.) ? unprofiled / myTime * 100. : 0. );
            logstream.information()<< indent.str() <<
                "Unprofiled: " <<
                unprofiled << " (" <<
                percent << "%)" << std::endl;
        }
        
        logstream.precision( oldPrec );
    }

    ++_depth;
    traverse( node );
    --_depth;
}




// JAG3D_ENABLE_PROFILING
#endif


// jagBase
}
