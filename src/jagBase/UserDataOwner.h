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

#ifndef __JAGBASE_USER_DATA_OWNER_H__
#define __JAGBASE_USER_DATA_OWNER_H__ 1


#include <jagBase/Config.h>
#include <jagBase/ptr.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/NullChannel.h>
#include <Poco/Dynamic/Var.h>
#include <map>
#include <string>

namespace jagBase
{




/** \class UserDataOwner UserDataOwner.h <jagBase/UserDataOwner.h>
\brief Enables storage and retrieval of arbitrary user specified data in Jag3D classes
\details Uses std::map and Poco::Dynamic::var to provide access to arbitrary value stored on a string key. 
Derived classes can store arbitrary user data on string keys that can be used by the specific application without needing to create additional derived classes.
TBD Included are several helper functions for common uses of userData. These may be optimized to use a dedicated field rather than the map if performance demands it.
*/
class UserDataOwner {
public:
	UserDataOwner() {}
	UserDataOwner( const UserDataOwner& rhs )
      : _userData( rhs._userData ) 
    {}
	~UserDataOwner() {}


    typedef std::map<std::string, Poco::Dynamic::Var> UserDataMap;

	/** \brief Get the user data object
	*/
	UserDataMap& getUserData() {
		return _userData;
	}

	/** \brief Set the value of a particular key
	*/
	void setUserDataPair(const std::string& key,const Poco::Dynamic::Var& value) {
		_userData[key] = value;
	}

	/** \brief Get the value associated with a particular key
	*/
	Poco::Dynamic::Var getUserDataValue(const std::string& key) {
		return _userData[key];
	}


    /** \brief Set the "name" key using the standard key naming convention.
    */
    void setUserDataName( const std::string& name )
    {
        _userData[ "__jag3d_Name" ] = name;
    }
    /** \brief Get the "name" key using the standard key naming convention.
    */
    std::string getUserDataName() const
    {
        UserDataMap::const_iterator it( _userData.find( "__jag3d_Name" ) );
        if( it == _userData.end() )
            return( std::string( "" ) );

        const Poco::Dynamic::Var& value( it->second );
        return( value.convert< std::string >() );
    }


protected:
	UserDataMap _userData;
	

};
// jagBase
}


// __JAGBASE_LOG_BASE_H__
#endif
