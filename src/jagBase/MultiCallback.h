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
