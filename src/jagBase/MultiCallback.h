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
