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
#include <jagDraw/CommandMap.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/DrawCommand.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class Drawable Drawable.h <jagDraw/Drawable.h>
\brief A collection of DrawCommand objects for rendering geometry.
\details Stores zero or more DrawCommand objects, and provides an interface for
accessing and computing a bounding volume based on the DrawCommands and
vertex attribute data.

Note that Drawable objects contain only DrawCommands. Vertex information
is stored in a VertexArrayObject, located in CommandMap. One VertexArrayObject
can contain the geometric data for several Drawables, which reduces bind calls.

\specBegin

Drawable supports the following functionality:
\li Storage for zero or more DrawCommand objects.
\li Bounding volume computation.
\li Object ID management.

Each Drawable instance stores the following information:
\li A vector of shared pointers to DrawCommand objects.
\li A shared pointer to a Bound object.
\li A boolean flag to indicate the bound dirty state.
\li A shared pointer to a Drawable::ComputeBoundCallback.

The computed bounding volume could be unintialized. See computeBounds().

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
The jagSG::CullVisitor queries the bounding volume and stores references
to the Drawables in jagDraw::Node objects. }
\specViolations{None}
\specTableEnd

See member functions for additional specification requirements.

\specIssue{The fact that Drawable objects don't contain vertex data impacts
bound computation. A Drawable could be shared by multiple jagDraw::Node
objects\, producing different bound values depending on which
VertexArrayObject is associates with the jagDraw::Node CommandMap.}

\specEnd

*/
class JAGDRAW_EXPORT Drawable : protected jagBase::LogBase, public ObjectIDOwner
{
public:
    Drawable();
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


    /** \name Bound Query and Computation
    \details 
    */
    /*@{*/

    /** \brief Returns a bounding volume.
    \details 
    \param commands The CommandMap, containing information necessary to compute
    the bound, such as VertexArrayObjects.

    \specFuncBegin

    If Drawable::_boundDirty is true, this function performs the following operations:
    - If Drawable::_bound is NULL, this function allocates a new bound and stores it in Drawable::_bound.
    - If Drawable::_computeBoundCallback() is non-NULL, this function executes the callback.
      Otherwise, this function calls computeBounds().
    - Drawable::_boundDirty is set to false by calling setBoundDirty().

    Regardless of whether Drawable::_boundDirty was initially true or false, this function
    returns the value of Drawable::_bound.

    If Drawable::_computeBoundCallback == NULL and the bound is <em>uncomputable</em>,
    the return value will always be uninitialized. If Drawable::_computeBoundCallback
    is non-NULL, Drawable::_computeBoundCallback is entirely responsible for determining
    the return value.

    \specTableBegin
    \specDepend{CommandMap}
    \specTableEnd
    \specFuncEnd
    */
    virtual BoundPtr getBound( const CommandMap& commands );

    /** \brief TBD
    \details TBD

    \specFuncBegin
    \specTableBegin
    \specDepend{CommandMap}
    \specTableEnd
    \specFuncEnd
    */
    void setBoundDirty( const CommandMap& commands, const bool dirty=true );
    /** \brief TBD
    \details TBD

    \specFuncBegin
    \specTableBegin
    \specDepend{CommandMap}
    \specTableEnd
    \specFuncEnd
    */
    bool getBoundDirty( const CommandMap& commands ) const;

    /** \brief Computes a bounding volume.
    \details
    
    \specFuncBegin

    Computes the bounding volume based on Drawable::_drawCommands,
    Drawable::_computeBoundCallback, and the VertexArrayObject stored in
    \c commands.

    If any of the following conditions are true, the bound is

    <em>uncomputable</em>:
    \li \c commands does not contain a VertexArrayObject.
    \li The VertexArrayObject does not contain a non-NULL BufferObjectPtr marked as VertexArrayObject::Vertex.
    \li The VertexArrayObject does not contain a non-NULL VertexAttribPtr marked as VertexArrayObject::Vertex.
    \li Drawable::_drawCommands.size() == 0.

    \specTableBegin
    \specDepend{Bound\, CommandMap

        Jag3D uses VertexAttribContainer to compute the bound\, but this is not a
        JAG specification requirement. }
    \specTableEnd
    \specFuncEnd
    */
    void computeBounds( BoundPtr _bound, const CommandMap& commands );

    struct ComputeBoundCallback {
        /**

        \specFuncBegin
        \specTableBegin
        \specDepend{CommandMap}
        \specTableEnd
        \specFuncEnd
        */
        virtual void operator()( BoundPtr _bound, const CommandMap& commands ) = 0;
    };
    typedef jagBase::ptr< ComputeBoundCallback >::shared_ptr ComputeBoundCallbackPtr;

    /** \brief TBD
    \details TBD */
    void setComputeBoundCallback( ComputeBoundCallbackPtr computeBoundCallback )
    {
        _computeBoundCallback = computeBoundCallback;
    }
    /** \brief TBD
    \details TBD */
    ComputeBoundCallbackPtr getComputeBoundCallback() const
    {
        return( _computeBoundCallback );
    }

    /**@}*/


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


protected:
    /** Default value: _drawCommands = jagDraw::DrawCommandVec() */
    DrawCommandVec _drawCommands;

    /** Default value: _bound = jagDraw::BoundPtr() */
    BoundPtr _bound;
    /** Default value: _boundDirty = true */
    bool _boundDirty;

    /** Default value: _computeBoundCallback = ComputeBoundCallbackPtr() */
    ComputeBoundCallbackPtr _computeBoundCallback;
};

typedef jagBase::ptr< jagDraw::Drawable >::shared_ptr DrawablePtr;
typedef std::vector< DrawablePtr > DrawableVec;


// jagDraw
}


// __JAGDRAW_DRAWABLE_H__
#endif
