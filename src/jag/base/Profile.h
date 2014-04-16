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

#ifndef __JAG_BASE_PROFILE_H__
#define __JAG_BASE_PROFILE_H__ 1


#include <jag/base/Export.h>
#include <jag/base/Config.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>
#include <string>

//#define USE_POSIX_TIME 1
#define USE_CHRONO 1
#if defined( USE_POSIX_TIME )
#  include <boost/date_time/posix_time/posix_time.hpp>
#elif defined( USE_CHRONO )
#  include <boost/chrono/chrono.hpp>
#  include <boost/chrono/time_point.hpp>
#  include <boost/chrono/duration.hpp>
#endif



namespace jag {
namespace base {


/** \defgroup jagBaseProfile Performance Profiling Utilities
\details The Jag3D profiling system is inspired by and somewhat
based on the Bullet physics profiling system
(http://bullet.googlecode.com).

Note: Although Jag3D's profiling system is relatively efficient,
profiling has an inherent overhead. Profiling should be used during
development, but disabled for maximum performance. To disable all
profiling, set the CMake variable JAG3D_ENABLE_PROFILING to OFF.

To profile (time) a block of code, use the JAG3D_PROFILE macro.
This macros constructs a ProfileSample object that begins timing in
the constructor and ends timing in the destructor.

In addition, ProfileSample adds a ProfileNode to a tree managed by
the singleton ProfileManager. This system of nested nodes supports
timing called functions and code sub-blocks.
*/
/*@{*/



#ifdef JAG3D_ENABLE_PROFILING


// Forward declarations
struct ProfileNode;
typedef jag::base::ptr< jag::base::ProfileNode >::shared_ptr ProfileNodePtr;
typedef std::vector< ProfileNodePtr > ProfileNodeVec;


/** \class ProfileManager Profile.h <jag/base/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileManager
{
public:
    static ProfileManager* instance()
    {
        return( _s_instance );
    }

    void dumpAll( const bool reset=true );

    void startProfile( const char* name );
    void stopProfile();

    ProfileNodePtr getRoot()
    {
        return( _root );
    }

    void reset();

protected:
    ProfileManager();
    ~ProfileManager();
    static ProfileManager* _s_instance;

    ProfileNodePtr _root;
    ProfileNodePtr _current;
};


/** \class ProfileManager Profile.h <jag/base/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileSample
{
public:
    ProfileSample( const char * name )
    {
        ProfileManager::instance()->startProfile( name ); 
    }

    ~ProfileSample( void )                    
    {
        ProfileManager::instance()->stopProfile(); 
    }
};



/** \struct ProfileNode Profile.h <jag/base/Profile.h>
\brief TBD
\details TBD
*/
struct JAGBASE_EXPORT ProfileNode : public SHARED_FROM_THIS(ProfileNode)
{
    ProfileNode( const char* name );
    ~ProfileNode();

    void startCall();
    bool endCall();

    ProfileNodePtr findChild( const char* name );

    void reset();

    std::string _name;

    ProfileNodePtr _parent;
    ProfileNodeVec _children;

    int _totalCalls;
    int _recursiveCount;
#if defined( USE_POSIX_TIME )
    boost::posix_time::time_duration _totalTime;
    boost::posix_time::ptime _lastStart, _lastEnd;
#elif defined( USE_CHRONO )
    boost::chrono::high_resolution_clock::duration _totalTime;
    boost::chrono::high_resolution_clock::time_point _lastStart, _lastEnd;
#endif
};



/** \struct ProfileVisitor Profile.h <jag/base/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileVisitor
{
public:
    ProfileVisitor();
    ~ProfileVisitor();

    virtual void visit( ProfileNodePtr node )
    {
        traverse( node );
    }

    void traverse( ProfileNodePtr node );
};


/** \struct ProfileDump Profile.h <jag/base/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileDump : public ProfileVisitor, protected jag::base::LogBase
{
public:
    ProfileDump( const std::string& logName=std::string( "" ) );
    ~ProfileDump();

    virtual void visit( ProfileNodePtr node );

protected:
    int _depth;
};



#define    JAG3D_PROFILE( name ) jag::base::ProfileSample __profile( name )

// JAG3D_ENABLE_PROFILING
#else

#define    JAG3D_PROFILE( name )

// JAG3D_ENABLE_PROFILING
#endif



/*@}*/


// namespace jag::base::
}
}


// __JAG_BASE_PROFILE_H__
#endif
