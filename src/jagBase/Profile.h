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
#include <string>



namespace jagBase
{


/** \addtogroup jagBaseProfile Performance Profiling Utilities
\details Documentation is TBD.
*/
/*@{*/



#ifdef JAG3D_ENABLE_PROFILING

/** \class ProfileManager ProfileManager.h <jagBase/Profile.h>
\brief TBD
\details TBD
*/
class JAGBASE_EXPORT ProfileManager
{
public:
    static ProfileManager* instance();

    void startProfile( const char* name );
    void stopProfile();

protected:
    ProfileManager();
    ~ProfileManager();
    static ProfileManager* _s_instance;
};


/** \class ProfileManager ProfileManager.h <jagBase/Profile.h>
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


#define	JAG3D_PROFILE( name )			CProfileSample __profile( name )

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
