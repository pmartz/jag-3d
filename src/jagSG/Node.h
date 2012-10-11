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

#ifndef __JAGSG_NODE_H__
#define __JAGSG_NODE_H__ 1

#include <jagSG/Export.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/ContextSupport.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {
    struct DrawInfo;
}

namespace jagSG {


/** \class Node Node.h <jagSG/DrawNode.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT Node : protected jagBase::LogBase, public jagDraw::ObjectIDOwner
{
public:
    Node();
    Node( const Node& rhs );
    ~Node();




    /** \brief TBD
    \details TBD */
    virtual void execute( jagDraw::DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
};

typedef jagBase::ptr< jagSG::Node >::shared_ptr NodePtr;
typedef std::vector< NodePtr > NodeVec;
typedef std::vector< Node > NodeSimpleVec;


// jagSG
}


// __JAGSG_NODE_H__
#endif
