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

#ifndef __JAGDRAW_DRAWABLE_PREP_H__
#define __JAGDRAW_DRAWABLE_PREP_H__ 1

#include <jagBase/UserDataOwner.h>
#include <jagBase/ptr.h>

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <map>


namespace jagDraw {


struct DrawInfo;

class DrawablePrep;
typedef jagBase::ptr< jagDraw::DrawablePrep >::shared_ptr DrawablePrepPtr;


/** \class DrawablePrep DrawablePrep.h <jagDraw/DrawablePrep.h>
\brief
\details
*/
class DrawablePrep : public jagBase::UserDataOwner
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


    DrawablePrep( const CommandType type, const bool generateUniqueID=true )
      : _type( type ),
        _uniqueID( generateUniqueID ? UniqueID::instance()->generate( type ) : 0 )
    {}
    DrawablePrep( const DrawablePrep& rhs, const bool generateUniqueID=true )
      : _type( rhs._type ),
        _uniqueID( generateUniqueID ? UniqueID::instance()->generate( rhs._type ) : 0 )
    {}
    ~DrawablePrep() {}


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
    virtual DrawablePrepPtr clone() = 0;

    /** \brief Activate the DrawablePrep prior to calling execute().
    \details DrawablePrep objects usually do not require activation, so this is a no-op.
    However, Texture uses activate() to set the active texture. */
    virtual void activate( DrawInfo&, const unsigned int unit ) {}

    /** \brief TBD
    \details TBD */
    virtual void execute( DrawInfo& ) = 0;

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr combine( DrawablePrepPtr rhs ) const
    {
        return( rhs );
    }

    /** \brief TBD
    \details TBD */
    virtual bool operator== ( const DrawablePrep& rhs ) const
    {
        return( _uniqueID == rhs._uniqueID );
    }
    bool operator!= ( const DrawablePrep& rhs ) const
    {
        return( !( operator==( rhs ) ) );
    }

    bool operator< ( const DrawablePrep& rhs ) const
    {
        return( _uniqueID < rhs._uniqueID );
    }
    bool operator> ( const DrawablePrep& rhs ) const
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

typedef std::vector< DrawablePrepPtr > DrawablePrepVec;



/** \class DrawablePrepSet DrawablePrep.h <jagDraw/DrawablePrep.h>
\brief TBD
\details TBD
*/
template< typename KEY, class ELEMENT, class CLASS, class CLASS_PTR >
class DrawablePrepSet : public DrawablePrep, public std::map< KEY, ELEMENT >
{
protected:
    typedef std::map< KEY, ELEMENT > MAP_TYPE;

public:
    DrawablePrepSet( const CommandType type )
        : DrawablePrep( type, false )
    {}
    DrawablePrepSet( const DrawablePrepSet& rhs )
        : DrawablePrep( rhs, false ),
        MAP_TYPE( rhs )
    {}
    ~DrawablePrepSet()
    {}

    /** \brief TBD
    \details Override method from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->execute( drawInfo );
        }
    }

    virtual DrawablePrepPtr combine( DrawablePrepPtr rhs ) const
    {
        // std::map::insert does NOT overwrite, so put rhs in result first,
        // then insert the values held in this.
        CLASS* right( static_cast< CLASS* >( rhs.get() ) );
        CLASS_PTR result( new CLASS( *right ) );
        static_cast< MAP_TYPE* >( (CLASS*)result.get() )->insert( begin(), end() );
        return( result );
    }

    /** \brief TBD
    \details TBD */
    virtual bool operator== ( const DrawablePrep& rhs ) const
    {
        const MAP_TYPE* rSet( dynamic_cast< const MAP_TYPE* >( &rhs ) );

        if( size() != rSet->size() )
            return( false );

        MAP_TYPE::const_iterator leftIt( begin() );
        MAP_TYPE::const_iterator rightIt( rSet->begin() );
        while( leftIt != end() )
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
};


// jagDraw
}


// __JAGDRAW_DRAWABLE_PREP_H__
#endif
