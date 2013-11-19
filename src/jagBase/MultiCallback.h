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

#ifndef __JAGBASE_MULTI_CALLACK_H__
#define __JAGBASE_MULTI_CALLACK_H__ 1


#include <jagBase/Export.h>
#include <vector>


namespace jagBase
{

    
/** \class MultiCallback MultiCallback.h <jagBase/MultiCallback.h>
\brief Template class for storing a vector of callbacks.
\details TBD.
*/
template< class CB_TYPE >
class MultiCallback : public std::vector< CB_TYPE >
{
public:
    typedef std::vector< CB_TYPE > DataVec;

    bool contains( const CB_TYPE& cb )
    {
        for( DataVec::const_iterator it = begin(); it != end(); ++it )
        {
            if( *it == cb )
                return( true );
        }
        return( false );
    }

    void addUnique( const CB_TYPE& cb )
    {
        if( contains( cb ) )
            return;
        push_back( cb );
    }

    void remove( const CB_TYPE& cb )
    {
        for( DataVec::const_iterator it = begin(); it != end(); ++it )
        {
            if( *it == cb )
            {
                erase( it );
                break;
            }
        }
    }
};


// jagBase
}


// __JAGBASE_MULTI_CALLACK_H__
#endif
