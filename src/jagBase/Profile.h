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

#ifndef __JAGBASE_PROFILE_H__
#define __JAGBASE_PROFILE_H__ 1


#include <jagBase/Export.h>
#include <jagBase/Config.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>
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



namespace jagBase
{


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
typedef jagBase::ptr< jagBase::ProfileNode >::shared_ptr ProfileNodePtr;
typedef std::vector< ProfileNodePtr > ProfileNodeVec;


/** \class ProfileManager Profile.h <jagBase/Profile.h>
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


/** \class ProfileManager Profile.h <jagBase/Profile.h>
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



/** \struct ProfileNode Profile.h <jagBase/Profile.h>
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



/** \struct ProfileVisitor Profile.h <jagBase/Profile.h>
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


/** \struct ProfileDump Profile.h <jagBase/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileDump : public ProfileVisitor, protected jagBase::LogBase
{
public:
    ProfileDump( const std::string& logName=std::string( "" ) );
    ~ProfileDump();

    virtual void visit( ProfileNodePtr node );

protected:
    int _depth;
};



#define    JAG3D_PROFILE( name ) jagBase::ProfileSample __profile( name )

// JAG3D_ENABLE_PROFILING
#else

#define    JAG3D_PROFILE( name )

// JAG3D_ENABLE_PROFILING
#endif



/*@}*/


// jagBase
}


// __JAGBASE_PROFILE_H__
#endif
