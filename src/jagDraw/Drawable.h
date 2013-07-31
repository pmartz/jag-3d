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

#ifndef __JAGDRAW_DRAWABLE_H__
#define __JAGDRAW_DRAWABLE_H__ 1

#include <jagDraw/Export.h>
#include <jagBase/LogBase.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Bound.h>
#include <jagDraw/BoundOwner.h>
#include <jagBase/Notifier.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/DrawCommand.h>
#include <jagBase/ptr.h>
#include <jagBase/UserDataOwner.h>

#include <boost/thread/mutex.hpp>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class Drawable Drawable.h <jagDraw/Drawable.h>
\brief A collection of DrawCommand objects for rendering geometry.
\details Stores zero or more DrawCommand objects, and provides an interface for
accessing and computing a bounding volume based on the DrawCommands and
vertex attribute data.

Note that Drawable objects contain only DrawCommands. Vertex information
is stored in a VertexArrayObject (located in CommandMap). One VertexArrayObject
can contain the geometric data for several Drawables, which reduces bind calls.

\specBegin Drawable

Drawable supports the following functionality:
\li Storage for zero or more DrawCommand objects.
\li Bounding volume computation.
\li Object ID management.

Each Drawable instance stores the following information:
\li A vector of shared pointers to DrawCommand objects.
\li Possibly multiple shared pointers to Bound objects.
\li Possibly multiple boolean flags to indicate bound dirty state.
\li A shared pointer to an initial / minimum bounding volume.
\li A shared pointer to a Drawable::ComputeBoundCallback.

The computed bounding volume could be unintialized. See computeBound().

\specTableBegin
\specLog{jag.draw.drawable}
\specThread{Unsafe}
\specGL{None}
\specDepend{DrawCommand\, Bound}
\specUsage{Client code instantiates a Drawable\, then attaches one or more
DrawCommands. Optionally\, client code can also attach a
Drawable::ComputeBoundCallback.

Once the Drawable objects are instantiated and configured\, jagDraw-based
client code adds the Drawable objects to jagDraw::Node objects.

jagSG-based client code adds the Drawable objects to jagSG::Node objects.
The jagSG::CollectionVisitor queries the bounding volume and stores references
to the Drawables in jagDraw::Node objects. }
\specViolations{None}
\specTableEnd

See member functions for additional specification requirements.
\specEnd

*/
class JAGDRAW_EXPORT Drawable : protected jagBase::LogBase,
        public ObjectIDOwner, public BoundOwner,
		public jagBase::UserDataOwner,
        public jagBase::Notifier
{
public:
    Drawable( const std::string& logName=std::string( "" ) );
    Drawable( const Drawable& rhs );
    ~Drawable();
    

    /** \brief TBD
    \details TBD
Drawable::execute(DrawInfo&) executes the DrawCommands sequentially. Access
the list directly with getDrawCommandVec().


    \specFuncBegin
    \specTableBegin
    \specDepend{DrawInfo}
    \specTableEnd
    \specFuncEnd
    */
    virtual void execute( DrawInfo& drawInfo );


    /** \name Draw Commands
    \details TBD */
    /*@{*/

    /** \brief TBD
    \details TBD
    */
    void addDrawCommand( DrawCommandPtr dcp );

    /** \brief TBD
    \details TBD
    */
    DrawCommandVec& getDrawCommandVec();
    /** \overload */
    const DrawCommandVec& getDrawCommandVec() const;

    /**@}*/


    /** \name OpenGL ID Management
    \details TBD */
    /*@{*/

    /** \brief Tell the Drawable how many contexts to expect.
    \details Drawable iterates over all other attached Jag3D objects and
    passes \c numContexts to their setMaxContexts() member function.
    
    Override from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the Drawable's objects.
    \details OpenGL object ID cleanup is not yet implemented. TBD.

    Override from ObjectIDOwner.

    \specFuncBegin
    \specTableBegin
    \specDepend{jagDrawContextID}
    \specTableEnd
    \specFuncEnd
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

    /**@}*/


    /** \name BoundOwner support
    \details Override member functions from the BoundOwner base class. */
    /**@{*/

    /** \brief Return a new uninitialized bound.
    \details Returns a new BoundAABox. */
    BoundPtr newBound();

    /** \brief Compute the Drawable's bounding volume.
    \details Override the base class BoundOwner::computeBound().
    
    \specFuncBegin

    Computes the bounding volume based on Drawable::_drawCommands,
    and the specified VertexArrayObject \c vao.

    If any of the following conditions are true, the bound is
    <em>uncomputable</em>:
    \li \c vao is NULL.
    \li \c vao does not contain a non-NULL BufferObjectPtr marked as VertexArrayObject::Vertex.
    \li \c vao does not contain a non-NULL VertexAttribPtr marked as VertexArrayObject::Vertex.
    \li Drawable::_drawCommands.size() == 0.

    \specTableBegin
    \specThread{Thread Safe}
    \specDepend{Bound\, VertexArrayObject

        Jag3D uses VertexAttribContainer to compute the bound\, but this is not a
        JAG specification requirement. }
    \specTableEnd
    \specFuncEnd
    */
    virtual void computeBound( BoundPtr& bound, const VertexArrayObject* vao, BoundOwner* owner );

    /** \brief Notify owning code of bound dirty state. */
    virtual void setBoundDirty( const VertexArrayObject* vao, const bool dirty=true );
    /** \brief Notify owning code of bound dirty state. */
    virtual void setAllBoundsDirty( const bool dirty=true );

    /**@}*/


protected:
    /** Default value: _drawCommands = jagDraw::DrawCommandVec() */
    DrawCommandVec _drawCommands;
};

typedef jagBase::ptr< jagDraw::Drawable >::shared_ptr DrawablePtr;
typedef std::vector< DrawablePtr > DrawableVec;


// jagDraw
}


// __JAGDRAW_DRAWABLE_H__
#endif
