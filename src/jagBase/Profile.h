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

#ifndef __JAGBASE_PROFILE_H__
#define __JAGBASE_PROFILE_H__ 1


#include <jagBase/Export.h>
#include <jagBase/ptr.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>



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

    void startProfile( const char* name );
    void stopProfile();

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
struct ProfileNode : public SHARED_FROM_THIS(ProfileNode)
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
    boost::posix_time::ptime _totalTime;
    boost::posix_time::ptime _lastStart, _lastEnd;
};




#define	JAG3D_PROFILE( name ) jagBase::ProfileSample __profile( name )

// JAG3D_ENABLE_PROFILING
#else

#define	JAG3D_PROFILE( name )

// JAG3D_ENABLE_PROFILING
#endif



/*@}*/


// jagBase
}


// __JAGBASE_PROFILE_H__
#endif
