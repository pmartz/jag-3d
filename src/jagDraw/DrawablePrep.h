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


typedef enum {
    Framebuffer_t,
    Program_t,
    Texture_t,
    Uniform_t,
    UniformBlock_t,
    VertexArrayObject_t
} CommandType;

typedef std::vector< CommandType > CommandTypeVec;


/** \class DrawablePrep DrawablePrep.h <jagDraw/DrawablePrep.h>
\brief
\details
*/
class DrawablePrep
{
public:
    DrawablePrep( const CommandType type )
      : _type( type ),
        _uniqueID( UniqueID::instance()->generate( type ) )
    {}
    DrawablePrep( const DrawablePrep& rhs )
      : _type( rhs._type ),
        _uniqueID( UniqueID::instance()->generate( rhs._type ) )
    {}
    ~DrawablePrep() {}

    /** \brief TBD
    \details TBD */
    virtual void operator()( DrawInfo& ) = 0;


    // TBD remove, replace with general purpose user data infrastructure.
    std::string _nametbd;

    CommandType getCommandType() const
    {
        return( _type );
    }
    std::string getCommandTypeString() const
    {
        switch( _type )
        {
            case Framebuffer_t: return( "Framebuffer" );
            case Program_t: return( "Program" );
            case Texture_t: return( "Texture" );
            case Uniform_t: return( "Uniform" );
            case UniformBlock_t: return( "UniformBlock" );
            case VertexArrayObject_t: return( "VertexArrayObject" );
            default: return( "ERROR" ); break;
        }
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
            const int typeInt( (int)type );
            if( typeInt >= _ids.size() )
            {
                while( typeInt >= _ids.size() )
                {
                    _ids.resize( _ids.size()+1 );
                    _ids[ _ids.size()-1 ] = 0;
                }
            }
            unsigned int id( _ids[ typeInt ] );
            ++( _ids[ typeInt ] );
            return( id );
        }

    private:
        UniqueID() {}
        ~UniqueID() {}

        std::vector< unsigned int > _ids;
        boost::mutex _mutex;
    };
};

typedef jagBase::ptr< jagDraw::DrawablePrep >::shared_ptr DrawablePrepPtr;
typedef std::vector< DrawablePrepPtr > DrawablePrepVec;


// jagDraw
}


// __JAGDRAW_DRAWABLE_PREP_H__
#endif
