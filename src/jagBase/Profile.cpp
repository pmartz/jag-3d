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

#include <jagBase/Profile.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

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
    startProfile( "jag" );
}
ProfileManager::~ProfileManager()
{
}

void ProfileManager::dumpAll( const bool reset )
{
    stopProfile();

    ProfileDump dumper;
    dumper.visit( _root );

    if( reset )
        _root->reset();

    startProfile( "jag" );
}

void ProfileManager::startProfile( const char* name )
{
    if( name != _current->_name )
        _current = _current->findChild( name );
    _current->startCall();
}
void ProfileManager::stopProfile()
{
    if( _current->endCall() && ( _current->_parent != NULL ) )
        _current = _current->_parent;
}

void ProfileManager::reset()
{
    _root.reset( new ProfileNode( "jag" ) );
    _current = _root;
    startProfile( "jag" );
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
#if defined( USE_POSIX_TIME )
        _lastStart = boost::posix_time::microsec_clock::local_time();
#elif defined( USE_CHRONO )
        _lastStart = boost::chrono::high_resolution_clock::now();
#endif
}
bool ProfileNode::endCall()
{
    if( --_recursiveCount == 0 )
    {
#if defined( USE_POSIX_TIME )
        _lastEnd = boost::posix_time::microsec_clock::local_time();
#elif defined( USE_CHRONO )
        _lastEnd = boost::chrono::high_resolution_clock::now();
#endif
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
#if defined( USE_POSIX_TIME )
    _totalTime = boost::posix_time::time_duration();
    _lastStart = _lastEnd = boost::posix_time::ptime();
#elif defined( USE_CHRONO )
    _totalTime = boost::chrono::microseconds( 0 );
#endif

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


ProfileDump::ProfileDump( const std::string& logName )
    : ProfileVisitor(),
      jagBase::LogBase( logName.empty() ? "jag.prof" : logName ),
      _depth( 0 )
{
}
ProfileDump::~ProfileDump()
{
}


#if defined( USE_POSIX_TIME )
#  define AS_DBL_MS(__t) \
    ( (double)( __t.total_microseconds() ) * .001 )
#elif defined( USE_CHRONO )
#  define AS_DBL_MS(__t) \
    ( (double)( __t.count() ) * .000001 )
#endif

void ProfileDump::visit( ProfileNodePtr node )
{
    const double myTime( AS_DBL_MS(node->_totalTime) );
    if( JAG3D_LOG_INFO )
    {
        Poco::LogStream logstream( _logger );

        std::ostringstream indent;
        for( unsigned int idx=0; idx<_depth; ++idx )
            indent << ". ";

        const std::streamsize oldPrec( logstream.precision( 4 ) );

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
                    childTime << "ms (" <<
                    percent << "%)" << std::endl;
            }
            const double unprofiled( myTime - runningTotal );
            const double percent( ( myTime > 0.) ? unprofiled / myTime * 100. : 0. );
            logstream.information()<< indent.str() <<
                "Unprofiled: " <<
                unprofiled << "ms (" <<
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
