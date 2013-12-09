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

#ifndef __JAG_DRAW_COMMAND_MAP_H__
#define __JAG_DRAW_COMMAND_MAP_H__ 1

#include <jag/draw/ObjectID.h>
#include <jag/draw/Command.h>
#include <jag/draw/CommandNode.h>
#include <jag/base/LogMacros.h>
#include <jag/draw/Error.h>

#include <jag/base/ptr.h>
#include <jag/base/UserDataOwner.h>

#include <boost/foreach.hpp>

#include <vector>
#include <map>
#include <set>
#include <bitset>


namespace jag {
namespace draw {


/** \addtogroup jagDrawDataStore Geometry and OpenGL Command Data Storage */
/*@{*/

/** \class CommandMap CommandMap.h <jag/draw/CommandMap.h>
*/
class CommandMap : public ObjectIDOwner, public jag::base::UserDataOwner
{
public:
    CommandMap()
    {}
    CommandMap( const CommandMap& rhs )
      : ObjectIDOwner( rhs ),
        _data( rhs._data ),
        _bits( rhs._bits ),
        _overrideBits( rhs._overrideBits ),
        _protectBits( rhs._protectBits )
    {}
    ~CommandMap()
    {}

    void insert( const DrawablePrepPtr drawablePrep, bool override=false, bool protect=false )
    {
        if( drawablePrep->getCommandType() >= Command::MaxCommandType )
        {
            JAG3D_CRITICAL_STATIC( "jag.draw.commandmap", "Unsupported command type." );
            exit( 1 );
        }

        const Command::CommandType type( drawablePrep->getCommandType() );
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

        dirtyParents();
    }

    DrawablePrepPtr& operator[]( const Command::CommandType type )
    {
        return( _data[ type ] );
    }

    typedef std::map< Command::CommandType, DrawablePrepPtr > CommandMapType;

    DrawablePrepPtr operator[]( const Command::CommandType type ) const
    {
        CommandMapType::const_iterator it( _data.find( type ) );
        if( it == _data.end() )
            return( DrawablePrepPtr() );
        else
        {
            return( it->second );
        }
    }

    virtual void setMaxContexts( const unsigned int numContexts )
    {
        BOOST_FOREACH( jag::draw::CommandMap::CommandMapType::value_type dpPair, _data )
        {
            jag::draw::ObjectIDPtr objID( boost::dynamic_pointer_cast< jag::draw::ObjectID >( dpPair.second ) );
            if( objID != NULL )
                objID->setMaxContexts( numContexts );
            else
            {
                jag::draw::ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< jag::draw::ObjectIDOwner >( dpPair.second ) );
                if( objIDOwner != NULL )
                    objIDOwner->setMaxContexts( numContexts );
            }
        }
    }
    virtual void deleteID( const jag::draw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( jag::draw::CommandMap::CommandMapType::value_type dpPair, _data )
        {
            jag::draw::ObjectIDPtr objID( boost::dynamic_pointer_cast< jag::draw::ObjectID >( dpPair.second ) );
            if( objID != NULL )
                objID->deleteID( contextID );
            else
            {
                jag::draw::ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< jag::draw::ObjectIDOwner >( dpPair.second ) );
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
        virtual void operator()( DrawablePrepPtr ) = 0;
    };

    void foreach( Callback& callback )
    {
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            callback( dataPair.second );
        }
    }

    /** \brief Return combination of commands in *this and \rhs.
    \details Returns a new CommandMap equal to *this + \c rhs. Commands
    in \c rhs take precedence over commands of the same type in *this,
    obeying override and protect bits.

    Used by jagSG during scene graph traversal to combine child commands
    with parent commands for new entries on the CommandMap stack. */
    CommandMap operator+( const CommandMap& rhs ) const
    {
        std::set< Command::CommandType > local;
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            local.insert( dataPair.first );
        }
        BOOST_FOREACH( CommandMapType::value_type dataPair, rhs._data )
        {
            local.insert( dataPair.first );
        }

        CommandMap result;
        BOOST_FOREACH( const Command::CommandType& type, local )
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
            {
                const DrawablePrepPtr& left( ( *_data.find( type ) ).second );
                const DrawablePrepPtr& right( ( *rhs._data.find( type ) ).second );
                if( ( left != right ) &&
                    ( *left != *right ) )
                    result[ type ] = left->combine( right );
                else
                    result[ type ] = left;
                result._bits.set( type );
                break;
            }
            }
        }

        return( result );
    }

    /* \brief Fold command from \c rhs into *this, returning the delta.
    \details This operator does two things.  
    \li applies \c rhs to *this;
    \li returns only the delta.

    This is used by the draw graph to determine which commands needs to
    be executed (only the delta), and to track current commands in the
    DrawInfo object.

    Currently, override and protect bits are honored, allowing these
    features in the draw traversal. However, this might be removed at
    a future date for performance reasons. */
    CommandMap operator<<( CommandMap& rhs )
    {
        std::set< Command::CommandType > local;
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            local.insert( dataPair.first );
        }

        BOOST_FOREACH( CommandMapType::value_type dataPair, rhs._data )
        {
            local.insert( dataPair.first );
        }

        CommandMap result;
        BOOST_FOREACH( const Command::CommandType& type, local )
        {
            switch( ( rhs._bits[ type ] << 1 ) | (int)( _bits[ type ] ) )
            {
            case 3: // both have it
            {
                DrawablePrepPtr& left( _data[ type ] );
                DrawablePrepPtr& right( rhs._data[ type ] );
                if( ( left != right ) &&
                    ( *left != *right ) )
                {
                    left = left->combine( right );
                    //insert( right, rhs._overrideBits[ type ] );
                    result.insert( right, rhs._overrideBits[ type ] );
                }
                break;
            }
            case 2: // rhs has it
            {
                DrawablePrepPtr command( rhs._data[ type ] );
                insert( command );
                result.insert( command );
                break;
            }
            case 0:
            case 1: // lhs has it
                break;
            }
        }
        return( result );
    }
    void executeDelta( CommandMap& rhs, DrawInfo& drawInfo )
    {
        std::set< Command::CommandType > local;
        BOOST_FOREACH( CommandMapType::value_type dataPair, _data )
        {
            local.insert( dataPair.first );
        }

        BOOST_FOREACH( CommandMapType::value_type dataPair, rhs._data )
        {
            local.insert( dataPair.first );
        }

        BOOST_FOREACH( const Command::CommandType& type, local )
        {
            switch( ( rhs._bits[ type ] << 1 ) | (int)( _bits[ type ] ) )
            {
            case 0:
            case 1: // lhs has it
                break;
            case 2: // rhs has it
                if( _overrideBits.test( type ) == false || rhs._protectBits.test( type ) == true  )
                {
                    DrawablePrepPtr command( rhs._data[ type ] );
                    insert( command, rhs._overrideBits[ type ] );
                    command->execute( drawInfo );
                }
                break; 

            case 3: // both have it
                DrawablePrepPtr& left( _data[ type ] );
                DrawablePrepPtr& right( rhs._data[ type ] );
                if( ( left != right ) &&
                    ( *left != *right ) )
                {
                    if( _overrideBits.test( type ) == false || rhs._protectBits.test( type ) == true ) 
                    {
                        left = left->combine( right );
                        //insert( right, rhs._overrideBits[ type ] );
                        //result.insert( right, rhs._overrideBits[ type ] );
                        left->execute( drawInfo );
                    }
                }
                break;
            }
        }
    }

    bool contains( Command::CommandType type ) const
    {
        return( _bits.test( type ) );
    }
    void clear( Command::CommandType type )
    {
        if( contains( type ) )
        {
            _bits.flip( type );
            _data.erase( type );
        }
        dirtyParents();
    }
    bool empty() const
    {
        return( _data.empty() );
    }

    DrawablePrepPtr getData( Command::CommandType type ) const
    { 
        CommandMapType::const_iterator p( _data.find( type ) );
        if( p == _data.end() )
            return( DrawablePrepPtr() );
        return( p->second );
    }

    bool operator==( const CommandMap& rhs ) const
    {
        if( ( _bits != rhs._bits ) ||
            ( _overrideBits != rhs._overrideBits ) ||
            ( _protectBits != rhs._protectBits ) ||
            ( _data.size() != rhs._data.size() ) )
            return( false );

        CommandMapType::const_iterator leftIt( _data.begin() );
        CommandMapType::const_iterator rightIt( rhs._data.begin() );
        while( leftIt != _data.end() )
        {
            if( *( leftIt->second ) != *( rightIt->second ) )
                return( false );
            ++leftIt; ++rightIt;
        }

        return( true );
    }
    bool operator!=( const CommandMap& rhs ) const
    {
        return( !( operator==( rhs ) ) );
    }

    void addParent( const CommandNodePtr& parent )
    {
        _nodes.insert( parent );
    }
    void removeParent( const CommandNodePtr& parent )
    {
        CommandNodeSet::iterator it( _nodes.find( parent ) );
        if( it != _nodes.end() )
            _nodes.erase( it );
    }
    void dirtyParents()
    {
        BOOST_FOREACH( CommandNodeSet::value_type parentNode, _nodes )
        {
            parentNode->setDirty();
        }
    }


    CommandMapType _data;
    std::bitset< Command::MaxCommandType > _bits;
    std::bitset< Command::MaxCommandType > _overrideBits;
    std::bitset< Command::MaxCommandType > _protectBits;

    CommandNodeSet _nodes;
};




class CommandMapSorter
{
public:
    CommandMapSorter()
    {}
    CommandMapSorter( const Command::CommandTypeVec& priorityVec ):
        _priorityVec( priorityVec )
    {}
    CommandMapSorter( const CommandMapSorter& rhs )
        : _priorityVec( rhs._priorityVec )
    {}
    ~CommandMapSorter()
    {}

    bool operator()( const CommandMap& lhs, const CommandMap& rhs ) const
    {
        BOOST_FOREACH( const Command::CommandType& type, _priorityVec )
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
                    const DrawablePrepPtr& a( lhs._data.find( type )->second );
                    const DrawablePrepPtr& b( rhs._data.find( type )->second );
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
    Command::CommandTypeVec _priorityVec;
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_COMMAND_MAP_H__
#endif
