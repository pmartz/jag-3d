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

#include <jagBase/ptr.h>

#include <boost/thread/mutex.hpp>

#include <vector>


namespace jagDraw {


struct DrawInfo;

class DrawablePrep;
typedef jagBase::ptr< jagDraw::DrawablePrep >::shared_ptr DrawablePrepPtr;


/** \class DrawablePrep DrawablePrep.h <jagDraw/DrawablePrep.h>
\brief
\details
*/
class DrawablePrep
{
public:
    // TBD temp, eventually have std::map< boost::any, boost::any >.
    std::string _nametbd;

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


    DrawablePrep( const CommandType type )
      : _type( type ),
        _uniqueID( UniqueID::instance()->generate( type ) )
    {}
    DrawablePrep( const DrawablePrep& rhs )
      : _nametbd( rhs._nametbd ),
        _type( rhs._type ),
        _uniqueID( UniqueID::instance()->generate( rhs._type ) )
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
    virtual DrawablePrepPtr combine( DrawablePrepPtr rhs )
    {
        return( rhs );
    }

    bool operator== ( const DrawablePrep& rhs ) const
    {
        return( _uniqueID == rhs._uniqueID );
    }
    bool operator!= ( const DrawablePrep& rhs ) const
    {
        return( _uniqueID != rhs._uniqueID );
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
            const unsigned int typeUInt( (int)type );
            if( typeUInt >= _ids.size() )
            {
                while( typeUInt >= _ids.size() )
                {
                    _ids.resize( _ids.size()+1 );
                    _ids[ _ids.size()-1 ] = 0;
                }
            }
            unsigned int id( _ids[ typeUInt ] );
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


// jagDraw
}


// __JAGDRAW_DRAWABLE_PREP_H__
#endif
