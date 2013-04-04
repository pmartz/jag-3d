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

#ifndef __JAGDRAW_NODE_H__
#define __JAGDRAW_NODE_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Drawable.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class Node Node.h <jagDraw/Node.h>
\brief TBD
\details jagDraw::Node associates a CommandMap with a collection of Drawables.
The CommandMap must not be NULL. */
class JAGDRAW_EXPORT Node : protected jagBase::LogBase, public ObjectIDOwner
{
public:
    Node( CommandMapPtr commands=CommandMapPtr( (CommandMap*)NULL ));
    Node( const Node& rhs );
    ~Node();


    /** \brief Add a drawable prep command, such as a uniform or a shader program.
    \details All drawable prep commands are executed by the Drawale in order, and prior to
    issuing any of the vertex array commands (see addVertexArrayCommand()). */
    void setCommandMap( CommandMapPtr commands )
    {
        _commands = commands;
    }
    /** \brief TBD
    \details TBD */
    const CommandMapPtr getCommandMap() const
    {
        return( _commands );
    }


    /** \brief Add a drawable.
    \details All drawables are executed after issuing all commands in \c _commands. */
    void addDrawable( DrawablePtr drawable )
    {
        _drawables.push_back( drawable );
    }
    /** \brief TBD
    \details TBD */
    DrawableVec& getDrawableVec()
    {
        return( _drawables );
    }
    /** \overload */
    const DrawableVec& getDrawableVec() const
    {
        return( _drawables );
    }
    /** \brief TBD
    \details TBD */
    unsigned int getNumDrawables() const
    {
        return( (unsigned int) _drawables.size() );
    }


    /** \brief Assignment operator.
    \details Required by std::sort(). */
    Node& operator=( const Node& rhs )
    {
        _commands = rhs._commands;
        _drawables = rhs._drawables;
        return( *this );
    }


    /** \brief TBD
    \details TBD */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    CommandMapPtr _commands;
    DrawableVec _drawables;
};

typedef jagBase::ptr< jagDraw::Node >::shared_ptr DrawNodePtr;
typedef std::vector< DrawNodePtr > DrawNodeVec;
typedef std::vector< Node > DrawNodeSimpleVec;


class DrawNodeCommandSorter
{
public:
    DrawNodeCommandSorter()
    {}
    DrawNodeCommandSorter( const DrawablePrep::CommandTypeVec& priorityVec ):
        _priorityVec( priorityVec )
    {}
    DrawNodeCommandSorter( const DrawNodeCommandSorter& rhs )
        : _priorityVec( rhs._priorityVec )
    {}
    ~DrawNodeCommandSorter()
    {}

    bool operator()( const Node& lhs, const Node& rhs ) const
    {
        const CommandMapPtr lhsCommands( lhs.getCommandMap() );
        const CommandMapPtr rhsCommands( rhs.getCommandMap() );

        for( DrawablePrep::CommandTypeVec::const_iterator typeIter = _priorityVec.begin(); typeIter != _priorityVec.end(); ++typeIter )
        {
            switch( (int)( lhsCommands->_bits[ *typeIter ] ) | ( rhsCommands->_bits[ *typeIter ] << 1 ) )
            {
                case 0:
                    continue;
                case 1:
                    return( true );
                case 2:
                    return( false );
                case 3: 
                {
                    const DrawablePrepPtr a( lhsCommands->_data.find( *typeIter )->second );
                    const DrawablePrepPtr b( rhsCommands->_data.find( *typeIter )->second );
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
    DrawablePrep::CommandTypeVec _priorityVec;
};


// jagDraw
}


// __JAGDRAW_NODE_H__
#endif
