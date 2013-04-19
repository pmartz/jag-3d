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

#ifndef __JAGDRAW_NODE_CONTAINER_H__
#define __JAGDRAW_NODE_CONTAINER_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/Node.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/LogBase.h>
#include <jagBase/MultiCallback.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class NodeContainer NodeContainer.h <jagDraw/NodeContainer.h>
\brief TBD
\details std::vector of draw graph Node objects. */
class JAGDRAW_EXPORT NodeContainer : public jagDraw::DrawNodeSimpleVec,
        protected jagBase::LogBase, public ObjectIDOwner
{
public:
    NodeContainer();
    NodeContainer( const NodeContainer& rhs );
    ~NodeContainer();

    NodeContainer& operator=( const NodeContainer& rhs );


    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( NodeContainer& /* nodeContainer */,
            DrawInfo& /* drawInfo */ )
        {
            return( false );
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > CallbackVec;
    /** \brief TBD
    \details TBD */
    CallbackVec& getCallbacks();


    /** \brief If all callbacks pass, execute all Nodes.
    \details If all callbacks return true, call executeDrawNodes().
    Otherwise do nothing and return.
    
    This is an override of the ObjectIDOwner base class. */
    virtual void execute( DrawInfo& drawInfo );
    /** \brief Call execute() on all Nodes.
    \details TBD */
    virtual void executeDrawNodes( DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    CallbackVec _callbacks;
};

typedef jagBase::ptr< jagDraw::NodeContainer >::shared_ptr NodeContainerPtr;
typedef std::vector< NodeContainerPtr > NodeContainerVec;

typedef std::vector< NodeContainer > DrawGraph; // or NodeContainerSimpleVec
typedef jagBase::ptr< jagDraw::DrawGraph >::shared_ptr DrawGraphPtr;


#if 0
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
#endif


// jagDraw
}


// __JAGDRAW_NODE_CONTAINER_H__
#endif
