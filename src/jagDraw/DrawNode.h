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

#ifndef __JAGDRAW_DRAW_NODE_H__
#define __JAGDRAW_DRAW_NODE_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/DrawablePrep.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class DrawNode DrawNode.h <jagDraw/DrawNode.h>
\brief TBD
\details TBD
*/
class JAGDRAW_EXPORT DrawNode : protected jagBase::LogBase, public ObjectIDOwner
{
public:
    DrawNode();
    DrawNode( const DrawNode& rhs );
    ~DrawNode();

    /** \brief TBD
    \details TBD */
    virtual void operator()( DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts ) {}

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}


    /** \brief Add a drawable prep command, such as a uniform or a shader program.
    \details All drawable prep commands are executed by the Drawale in order, and prior to
    issuing any of the vertex array commands (see addVertexArrayCommand()). */
    void addDrawablePrep( DrawablePrepPtr dpp );
    void insertDrawablePrep( DrawablePrepPtr dpp, unsigned int pos=0 );

    /** \brief TBD
    \details TBD */
    DrawablePrepVec& getDrawablePrepVec() { return( _drawablePrep ); }
    /** \overload */
    const DrawablePrepVec& getDrawablePrepVec() const { return( _drawablePrep ); }


    /** \brief Add a drawable.
    \details All drawables are executed after issuing all DrawablePrep commands. */
    void addDrawable( DrawablePtr drawable )
    {
        _drawables.push_back( drawable );
    }

    /** \brief TBD
    \details TBD */
    DrawableVec& getDrawableVec() { return( _drawables ); }
    /** \overload */
    const DrawableVec& getDrawableVec() const { return( _drawables ); }

protected:
    DrawablePrepVec _drawablePrep;
    DrawableVec _drawables;
};

typedef jagBase::ptr< jagDraw::DrawNode >::shared_ptr DrawNodePtr;
typedef std::vector< DrawNodePtr > DrawNodeVec;


// jagDraw
}


// __JAGDRAW_DRAW_NODE_H__
#endif
