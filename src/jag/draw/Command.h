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

#ifndef __JAG_DRAW_COMMAND_H__
#define __JAG_DRAW_COMMAND_H__ 1

#include <jag/base/UserDataOwner.h>
#include <jag/base/ptr.h>

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <map>


namespace jag {
namespace draw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

struct DrawInfo;

class Command;
typedef jag::base::ptr< jag::draw::Command >::shared_ptr CommandPtr;


/** \class Command Command.h <jag/draw/Command.h>
\brief
\details
*/
class Command : public jag::base::UserDataOwner
{
public:
    typedef enum {
        Program_t,
        BufferObjectSet_t, // TBD not yet implemented
        VertexArrayObject_t,
        TextureSet_t,
        SamplerSet_t,
        UniformSet_t,
        UniformBlockSet_t,
        Framebuffer_t,

        MaxCommandType,

        BufferObject_t,
        Texture_t,
        Sampler_t,
        Uniform_t,
        UniformBlock_t
    } CommandType;
    typedef std::vector< CommandType > CommandTypeVec;


    Command( const CommandType type, const bool generateUniqueID=true )
      : _type( type ),
        _uniqueID( generateUniqueID ? UniqueID::instance()->generate( type ) : 0 )
    {}
    Command( const Command& rhs, const bool generateUniqueID=true )
      : _type( rhs._type ),
        _uniqueID( generateUniqueID ? UniqueID::instance()->generate( rhs._type ) : 0 )
    {}
    virtual ~Command() {}


    /** \brief TBD
    \details TBD */
    CommandType getCommandType() const { return( _type ); }
    /** \brief TBD
    \details TBD */
    static std::string getCommandTypeString( const CommandType& commandType )
    {
        switch( commandType )
        {
            case Program_t: return( "Program" );
            case VertexArrayObject_t: return( "VertexArrayObject" );
            case BufferObjectSet_t: return( "BufferObjectSet" );
            case TextureSet_t: return( "TextureSet" );
            case SamplerSet_t: return( "SamplerSet" );
            case UniformSet_t: return( "UniformSet" );
            case UniformBlockSet_t: return( "UniformBlockSet" );
            case Framebuffer_t: return( "Framebuffer" );
            case BufferObject_t: return( "BufferObject" );
            case Texture_t: return( "Texture" );
            case Sampler_t: return( "Sampler" );
            case Uniform_t: return( "Uniform" );
            case UniformBlock_t: return( "UniformBlock" );
            default: return( "ERROR" ); break;
        }
    }


    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const = 0;

    /** \brief Activate the Command prior to calling execute().
    \details Command objects usually do not require activation, so this is a no-op.
    However, Texture uses activate() to set the active texture. */
    virtual void activate( DrawInfo&, const unsigned int unit ) {}

    /** \brief TBD
    \details TBD */
    virtual void execute( DrawInfo& ) = 0;

    /** \brief TBD
    \details TBD */
    virtual CommandPtr combine( CommandPtr rhs ) const
    {
        return( rhs );
    }

    /** \brief TBD
    \details TBD */
    virtual bool operator== ( const Command& rhs ) const
    {
        return( _uniqueID == rhs._uniqueID );
    }
    bool operator!= ( const Command& rhs ) const
    {
        return( !( operator==( rhs ) ) );
    }

    virtual bool operator< ( const Command& rhs ) const
    {
        return( _uniqueID < rhs._uniqueID );
    }
    virtual bool operator> ( const Command& rhs ) const
    {
        return( _uniqueID > rhs._uniqueID );
    }

protected:
    CommandType _type;
    unsigned int _uniqueID;


    class UniqueID
    {
    public:
        static UniqueID* instance()
        {
            static UniqueID* uniqueID( new UniqueID );
            return( uniqueID );
        }

        unsigned int generate( CommandType type )
        {
            boost::mutex::scoped_lock lock( _mutex );
            const unsigned int typeUInt( ( unsigned int )type );
            if( typeUInt >= _ids.size() )
            {
                while( typeUInt >= _ids.size() )
                {
                    _ids.resize( _ids.size()+1 );
                    _ids[ _ids.size()-1 ] = 0;
                }
            }
            const unsigned int id( _ids[ typeUInt ] );
            ++( _ids[ typeUInt ] );
            return( id );
        }

    private:
        UniqueID() {}
        ~UniqueID() {}

        std::vector< unsigned int > _ids;
        boost::mutex _mutex;
    };
};

typedef std::vector< CommandPtr > CommandVec;



/** \class CommandSet Command.h <jag/draw/Command.h>
\brief TBD
\details TBD
*/
template< typename KEY, class ELEMENT, class CLASS, class CLASS_PTR >
class CommandSet : public Command, public std::map< KEY, ELEMENT >
{
protected:
    typedef std::map< KEY, ELEMENT > MAP_TYPE;

public:
    CommandSet( const CommandType type )
        : Command( type, false )
    {}
    CommandSet( const CommandSet& rhs )
        : Command( rhs, false ),
        MAP_TYPE( rhs )
    {}
    ~CommandSet()
    {}

    /** \brief TBD
    \details Override method from Command. */
    virtual void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( const typename MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->execute( drawInfo );
        }
    }

    virtual CommandPtr combine( CommandPtr rhs ) const
    {
        // std::map::insert does NOT overwrite, so put rhs in result first,
        // then insert the values held in this.
        CLASS* right( static_cast< CLASS* >( rhs.get() ) );
        CLASS_PTR result( new CLASS( *right ) );
        static_cast< MAP_TYPE* >( (CLASS*)result.get() )->insert( this->begin(), this->end() );
        return( result );
    }

    /** \brief TBD
    \details TBD */
    virtual bool operator== ( const Command& rhs ) const
    {
        const MAP_TYPE* rSet( dynamic_cast< const MAP_TYPE* >( &rhs ) );

        if( this->size() != rSet->size() )
            return( false );

        typename MAP_TYPE::const_iterator leftIt( this->begin() );
        typename MAP_TYPE::const_iterator rightIt( rSet->begin() );
        while( leftIt != this->end() )
        {
            if( leftIt->first != rightIt->first )
                return( false );
            if( leftIt->second != rightIt->second )
                return( false );
            ++leftIt;
            ++rightIt;
        }

        return( true );
    }

    virtual bool operator< ( const Command& rhs ) const
    {
        const MAP_TYPE* rSet( dynamic_cast< const MAP_TYPE* >( &rhs ) );

        if( this->size() < rSet->size() )
            return( true );
        if( this->size() > rSet->size() )
            return( false );

        typename MAP_TYPE::const_iterator leftIt( this->begin() );
        typename MAP_TYPE::const_iterator rightIt( rSet->begin() );
        while( leftIt != this->end() )
        {
            if( leftIt->first < rightIt->first )
                return( true );
            if( leftIt->first > rightIt->first )
                return( false );
            if( *(leftIt->second) < *(rightIt->second) )
                return( true );
            ++leftIt;
            ++rightIt;
        }

        return( false );
    }
    virtual bool operator> ( const Command& rhs ) const
    {
        const MAP_TYPE* rSet( dynamic_cast< const MAP_TYPE* >( &rhs ) );

        if( this->size() > rSet->size() )
            return( true );
        if( this->size() < rSet->size() )
            return( false );

        typename MAP_TYPE::const_iterator leftIt( this->begin() );
        typename MAP_TYPE::const_iterator rightIt( rSet->begin() );
        while( leftIt != this->end() )
        {
            if( leftIt->first > rightIt->first )
                return( true );
            if( leftIt->first < rightIt->first )
                return( false );
            if( *(leftIt->second) > *(rightIt->second) )
                return( true );
            ++leftIt;
            ++rightIt;
        }

        return( false );
    }
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_COMMAND_H__
#endif
