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

#ifndef __JAGBASE_USER_DATA_OWNER_H__
#define __JAGBASE_USER_DATA_OWNER_H__ 1

#include <Poco/Foundation.h>
#if( POCO_OS == POCO_OS_MAC_OS_X )
// Include this first to work around OSX gcc 4.2 build issue
// Put isinf(), isnan() in global namespace, which Poco assumes. 
#include <cmath>
using std::isinf;
using std::isnan;
#endif

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

    /** \brief Is the UserData present?
    */
    bool hasUserData( const std::string& key )
    {
        return( _userData.find( key ) != _userData.end() );
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

    void setNodeMask( const bool nodeMask )
    {
        _userData[ "__jag3d_NodeMask"  ] = nodeMask;  
    }

    bool getNodeMask() const
    {
        UserDataMap::const_iterator it( _userData.find( "__jag3d_NodeMask" ) );
        if( it == _userData.end() )
            return( true );

        const Poco::Dynamic::Var& value( it->second );
        return( value.convert< bool >() );
    }

protected:
    UserDataMap _userData;
    

};
// jagBase
}


// __JAGBASE_LOG_BASE_H__
#endif
