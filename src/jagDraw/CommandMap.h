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

#ifndef __JAGDRAW_COMMAND_MAP_H__
#define __JAGDRAW_COMMAND_MAP_H__ 1

#include <jagDraw/DrawablePrep.h>

#include <vector>
#include <map>
#include <set>
#include <bitset>


namespace jagDraw {


template< class T, class U, size_t S >
class CommandMapTemplate
{
public:
    CommandMapTemplate( const std::string& name=std::string("") )
      : _name( name )
    {}
    CommandMapTemplate( const CommandMapTemplate& rhs )
      : _name( rhs._name ),
        _data( rhs._data ),
        _bits( rhs._bits ),
        _overrideBits( rhs._overrideBits ),
        _protectBits( rhs._protectBits )
    {}
    ~CommandMapTemplate()
    {}

    void insert( const std::pair< T, U >& d, bool override=false, bool protect=false )
    {
        _bits.set( d.first );

        _data[ d.first ] = d.second;

        if( override )
            _overrideBits.set( d.first );
        else
            _overrideBits.reset( d.first );

        if( protect )
            _protectBits.set( d.first );
        else
            _protectBits.reset( d.first );
    }

    void insert( T t, U u, bool override=false, bool protect=false )
    {
        insert( std::pair< T, U >( t, u ), override, protect );
    }

    U& operator[]( const T t )
    {
        return( _data[ t ] );
    }

    struct Callback
    {
        virtual void operator()( U ) const = 0;
    };

    void foreach( const Callback& callback )
    {
        for( typename std::map< T, U >::iterator p = _data.begin(); p != _data.end(); ++p )
        {
            callback( p->second );
        }
    }

    CommandMapTemplate< T, U, S > operator+( const CommandMapTemplate< T, U, S >& rhs ) const
    {
        std::set< T > local;
        for( typename std::map< T, U >::const_iterator p = _data.begin(); p != _data.end(); ++p )
            local.insert( p->first );

        for( typename std::map< T, U >::const_iterator p = rhs._data.begin(); p != rhs._data.end(); ++p )
            local.insert( p->first );

        CommandMapTemplate< T, U, S > ret;
        for( typename std::set< T >::iterator p = local.begin(); p != local.end(); ++p )
        {
            switch( ( rhs._bits[ *p ] << 1 ) | (int)( _bits[ *p ] ) )
            {
            case 0:
                break;
            case 1:
                ret[ *p ] = ( *_data.find( *p ) ).second;
                break;
            case 2:
                ret[ *p ] = ( *rhs._data.find(*p) ).second;
                break;
            case 3:
                ret[ *p ] = ( *rhs._data.find( *p ) ).second;
                break; 
            }
        }

        return( ret );
    }

    /*
        * This operator does two things.  
        * 1) it applies the rhs to the lhs 
        * 2) it returns only the deltas
        */
    CommandMapTemplate< T, U, S > operator<<( CommandMapTemplate< T, U, S >& rhs )
    {
        std::set< T > local;
        for( typename std::map< T, U >::const_iterator p = _data.begin(); p != _data.end(); ++p )
            local.insert( p->first );

        for( typename std::map< T, U >::const_iterator p = rhs._data.begin(); p != rhs._data.end(); ++p )
            local.insert( p->first );

        CommandMapTemplate< T, U, S > ret;
        for( typename std::set< T >::iterator p = local.begin(); p != local.end(); ++p )
        {
            switch( ( rhs._bits[ *p ] << 1 ) | (int)( _bits[ *p ] ) )
            {
                case 0:
                case 1:
                    break;
                case 2: 
                    if( _overrideBits.test( *p ) == false || rhs._protectBits.test( *p ) == true  )
                    {
                        U u = rhs._data[ *p ];
                        insert( *p, u, rhs._overrideBits[ *p ] );
                        ret.insert( *p, u, rhs._overrideBits[ *p ] );
                    }
                    break; 

                case 3: 
                    if( *(_data[ *p ]) != *(rhs._data[ *p ]) )
                    {
                        if( _overrideBits.test( *p ) == false || rhs._protectBits.test( *p ) == true ) 
                        {
                            U u = rhs._data[ *p ];
                            insert( *p, u, rhs._overrideBits[ *p ] );
                            ret.insert( *p, u, rhs._overrideBits[ *p ] );
                        }
                    }
                    break;
            }
        }
        return( ret );
    }

    bool contains( T t )
    {
        return( _bits[ t ].test() );
    }

    U getData( T t ) const
    { 
        typename std::map< T, U >::const_iterator p = _data.find(t);
        if( p == _data.end() )
            return( NULL );
        return( p->second );
    }


    std::string _name;
    std::map<T,U> _data;
    std::bitset<S> _bits;
    std::bitset<S> _overrideBits;
    std::bitset<S> _protectBits;
};

typedef CommandMapTemplate< CommandType, DrawablePrepPtr, 6 > CommandMap;



template< class T >
struct CommandPriorityVec : public std::vector< T >
{
};

template< class T, class U, size_t S >
class CommandMapSorter
{
public:
    CommandMapSorter()
    {}
    CommandMapSorter( const CommandPriorityVec< T >& priorityVec ):
        _priorityVec( priorityVec )
    {}
    CommandMapSorter( const CommandMapSorter& rhs )
        : _priorityVec( rhs._priorityVec )
    {}
    ~CommandMapSorter()
    {}

    bool operator()( const CommandMapTemplate< T, U, S >& lhs, const CommandMapTemplate< T, U, S >& rhs ) const
    {
        for( typename CommandPriorityVec< T >::const_iterator p = _priorityVec.begin(); p != _priorityVec.end(); ++p )
        {
            switch( (int)( lhs._bits[ *p ] ) | ( rhs._bits[ *p ] << 1 ) )
            {
                case 0:
                    continue;
                case 1:
                    return( true );
                case 2:
                    return( false );
                case 3: 
                {
                    const U a( lhs._data.find( *p )->second );
                    const U b( rhs._data.find( *p )->second );
                    if( *a < *b )
                        return( true );
                    if( *a > *b )
                        return( false );
                    break;
                }
            }
        }
        return( false );
    }

protected:
    CommandPriorityVec< T > _priorityVec;
};


// jagDraw
}


// __JAGDRAW_COMMAND_MAP_H__
#endif
