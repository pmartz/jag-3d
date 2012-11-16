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

#include <jagDraw/ObjectID.h>
#include <jagDraw/DrawablePrep.h>
#include <jagBase/LogMacros.h>

#include <jagBase/ptr.h>
#include <boost/foreach.hpp>

#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <deque>


namespace jagDraw {


class CommandMap : public ObjectIDOwner
{
public:
    CommandMap( const std::string& name=std::string("") )
      : _name( name )
    {}
    CommandMap( const CommandMap& rhs )
      : _name( rhs._name ),
        _data( rhs._data ),
        _bits( rhs._bits ),
        _overrideBits( rhs._overrideBits ),
        _protectBits( rhs._protectBits )
    {}
    ~CommandMap()
    {}

    void insert( const DrawablePrepPtr drawablePrep, bool override=false, bool protect=false )
    {
        if( ( drawablePrep->getCommandType() == Texture_t ) ||
            ( drawablePrep->getCommandType() == Uniform_t ) ||
            ( drawablePrep->getCommandType() == UniformBlock_t ) )
        {
            JAG3D_CRITICAL_STATIC( "jag.draw.commandmap", "Unsupported command type." );
            exit( 1 );
        }

        const CommandType type( drawablePrep->getCommandType() );
        _bits.set( type );

        _data[ type ] = drawablePrep;

        if( override )
            _overrideBits.set( type );
        else
            _overrideBits.reset( type );

        if( protect )
            _protectBits.set( type );
        else
            _protectBits.reset( type );
    }

    DrawablePrepPtr& operator[]( const CommandType type )
    {
        return( _data[ type ] );
    }

    typedef std::map< CommandType, DrawablePrepPtr > CommandMapType;

    DrawablePrepPtr operator[]( const CommandType type ) const
    {
        CommandMapType::const_iterator it( _data.find( type ) );
        if( it == _data.end() )
            return( DrawablePrepPtr( (DrawablePrep*)NULL ) );
        else
        {
            return( it->second );
        }
    }

    virtual void setMaxContexts( const unsigned int numContexts )
    {
        BOOST_FOREACH( jagDraw::CommandMap::CommandMapType::value_type dpPair, _data )
        {
            jagDraw::ObjectIDPtr objID( boost::dynamic_pointer_cast< jagDraw::ObjectID >( dpPair.second ) );
            if( objID != NULL )
                objID->setMaxContexts( numContexts );
            else
            {
                jagDraw::ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< jagDraw::ObjectIDOwner >( dpPair.second ) );
                if( objIDOwner != NULL )
                    objIDOwner->setMaxContexts( numContexts );
            }
        }
    }
    virtual void deleteID( const jagDraw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( jagDraw::CommandMap::CommandMapType::value_type dpPair, _data )
        {
            jagDraw::ObjectIDPtr objID( boost::dynamic_pointer_cast< jagDraw::ObjectID >( dpPair.second ) );
            if( objID != NULL )
                objID->deleteID( contextID );
            else
            {
                jagDraw::ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< jagDraw::ObjectIDOwner >( dpPair.second ) );
                if( objIDOwner != NULL )
                    objIDOwner->deleteID( contextID );
            }
        }
    }


    void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            dataPair.second->execute( drawInfo );
        }
    }

    struct Callback
    {
        virtual void operator()( DrawablePrepPtr ) const = 0;
    };

    void foreach( const Callback& callback )
    {
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            callback( dataPair.second );
        }
    }

    CommandMap operator+( const CommandMap& rhs ) const
    {
        std::set< CommandType > local;
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            local.insert( dataPair.first );
        }
        BOOST_FOREACH( CommandMapType::value_type dataPair, rhs._data )
        {
            local.insert( dataPair.first );
        }

        CommandMap result;
        BOOST_FOREACH( const CommandType& type, local )
        {
            switch( ( rhs._bits[ type ] << 1 ) | (int)( _bits[ type ] ) )
            {
            case 0:
                break;
            case 1: // lhs has it
                result[ type ] = ( *_data.find( type ) ).second;
                result._bits.set( type );
                break;
            case 2: // rhs has it
                result[ type ] = ( *rhs._data.find( type ) ).second;
                result._bits.set( type );
                break;
            case 3: // both have it
                result[ type ]->combine( *rhs._data.find( type )->second );
                result._bits.set( type );
                break;
            }
        }

        return( result );
    }

    /*
        * This operator does two things.  
        * 1) it applies the rhs to the lhs 
        * 2) it returns only the deltas
        */
    CommandMap operator<<( CommandMap& rhs )
    {
        std::set< CommandType > local;
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            local.insert( dataPair.first );
        }

        BOOST_FOREACH( CommandMapType::value_type dataPair, rhs._data )
        {
            local.insert( dataPair.first );
        }

        CommandMap result;
        BOOST_FOREACH( const CommandType& type, local )
        {
            switch( ( rhs._bits[ type ] << 1 ) | (int)( _bits[ type ] ) )
            {
                case 0:
                case 1: // lhs has it
                    break;
                case 2: // rhs has it
                    if( _overrideBits.test( type ) == false || rhs._protectBits.test( type ) == true  )
                    {
                        DrawablePrepPtr drawable( rhs._data[ type ] );
                        insert( drawable, rhs._overrideBits[ type ] );
                        result.insert( drawable, rhs._overrideBits[ type ] );
                    }
                    break; 

                case 3: // both have it
                    if( *(_data[ type ]) != *(rhs._data[ type ]) )
                    {
                        if( _overrideBits.test( type ) == false || rhs._protectBits.test( type ) == true ) 
                        {
                            DrawablePrepPtr drawable( rhs._data[ type ] );
                            insert( drawable, rhs._overrideBits[ type ] );
                            result.insert( drawable, rhs._overrideBits[ type ] );
                        }
                    }
                    break;
            }
        }
        return( result );
    }

    bool contains( CommandType type ) const
    {
        return( _bits.test( type ) );
    }
    void clear( CommandType type )
    {
        if( contains( type ) )
        {
            _bits.flip( type );
            _data[ type ] = DrawablePrepPtr( (DrawablePrep*)NULL );
        }
    }

    DrawablePrepPtr getData( CommandType type ) const
    { 
        CommandMapType::const_iterator p( _data.find( type ) );
        if( p == _data.end() )
            return( DrawablePrepPtr( (DrawablePrep*)NULL ) );
        return( p->second );
    }


    std::string _name;
    CommandMapType _data;
    std::bitset< MaxCommandType > _bits;
    std::bitset< MaxCommandType > _overrideBits;
    std::bitset< MaxCommandType > _protectBits;
};

typedef jagBase::ptr< jagDraw::CommandMap >::shared_ptr CommandMapPtr;
typedef std::vector< jagDraw::CommandMapPtr > CommandMapVec;

typedef std::deque< jagDraw::CommandMap > CommandMapDeque;



template< class CommandType >
struct CommandPriorityVec : public std::vector< CommandType >
{
};

class CommandMapSorter
{
public:
    CommandMapSorter()
    {}
    CommandMapSorter( const CommandPriorityVec< CommandType >& priorityVec ):
        _priorityVec( priorityVec )
    {}
    CommandMapSorter( const CommandMapSorter& rhs )
        : _priorityVec( rhs._priorityVec )
    {}
    ~CommandMapSorter()
    {}

    bool operator()( const CommandMap& lhs, const CommandMap& rhs ) const
    {
        BOOST_FOREACH( const CommandType& type, _priorityVec )
        {
            switch( (int)( lhs._bits[ type ] ) | ( rhs._bits[ type ] << 1 ) )
            {
                case 0:
                    continue;
                case 1: // lhs has it
                    return( true );
                case 2: // rhs has it
                    return( false );
                case 3: // both have it
                {
                    const DrawablePrepPtr a( lhs._data.find( type )->second );
                    const DrawablePrepPtr b( rhs._data.find( type )->second );
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
    CommandPriorityVec< CommandType > _priorityVec;
};


// jagDraw
}


// __JAGDRAW_COMMAND_MAP_H__
#endif
