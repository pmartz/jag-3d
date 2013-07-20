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
#include <jagBase/MultiCallback.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>
#include <jagBase/UserDataOwner.h>
#include <vector>


namespace jagDraw {


/** \class Node Node.h <jagDraw/Node.h>
\brief TBD
\details jagDraw::Node associates a CommandMap with a collection of Drawables.
The CommandMap must not be NULL. */
class JAGDRAW_EXPORT Node : protected jagBase::LogBase, public ObjectIDOwner, public jagBase::UserDataOwner
{
public:
    Node( CommandMapPtr commands=CommandMapPtr( (CommandMap*)NULL ), const std::string& logName=std::string( "" ) );
    Node( const std::string& logName );
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

    /** \brief Set the distance (absolute eye coordinate z value).
    \details Set by jagSG::CollectionVisitor during collection.
    Can be used to sort Nodes by depth. */
    void setDistance( const double distance )
    {
        _distance = distance;
    }
    /** \brief Get the disance (absolute eye coordinate z value). */
    double getDistance() const
    {
        return( _distance );
    }


    /** \brief Assignment operator.
    \details Required by std::sort(). */
    Node& operator=( const Node& rhs )
    {
        _matrix = rhs._matrix;
        _commands = rhs._commands;
        _drawables = rhs._drawables;
        _distance = rhs._distance;
        _executeCallbacks = rhs._executeCallbacks;
        return( *this );
    }


    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( jagDraw::Node& /* node */, jagDraw::DrawInfo& /* drawInfo */ )
        {
            return( false );
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;


    /** \brief Transform
    \details TBD */
    void setTransform( const gmtl::Matrix44d& matrix );
    /** \brief TBD
    \details TBD */
    gmtl::Matrix44d& getTransform();
    /** \overload */
    const gmtl::Matrix44d& getTransform() const;


    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > ExecuteCallbacks;
    /** \brief TBD
    \details TBD */
    ExecuteCallbacks& getExecuteCallbacks();


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
    gmtl::Matrix44d _matrix;
    CommandMapPtr _commands;
    DrawableVec _drawables;

    double _distance;

    ExecuteCallbacks _executeCallbacks;
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


class DrawNodeDistanceSorter
{
public:
    typedef enum {
        Ascending,
        Descending
    } SortOrder;

    DrawNodeDistanceSorter( const SortOrder order=Descending )
        : _order( order )
    {}
    DrawNodeDistanceSorter( const DrawNodeDistanceSorter& rhs )
        : _order( rhs._order )
    {}
    ~DrawNodeDistanceSorter()
    {}

    bool operator()( const Node& lhs, const Node& rhs ) const
    {
        const double lDist( lhs.getDistance() );
        const double rDist( rhs.getDistance() );
        if( lDist < rDist )
            return( _order == Ascending );
        else if( lDist > rDist )
            return( _order != Ascending );
        return( false );
    }

protected:
    SortOrder _order;
};


// jagDraw
}


// __JAGDRAW_NODE_H__
#endif
