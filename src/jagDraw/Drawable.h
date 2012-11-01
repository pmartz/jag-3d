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
\brief A collection of DrawCommands for rendering geometry.
\details Drawable stores a list of DrawCommand objects. These are commands for
endering, such as glDrawArrays, glDrawElements, and others.

Drawable::execute(DrawInfo&) executes the DrawCommands sequentially. Access
the list directly with getDrawCommandVec().

Note that Drawables contain only DrawCommands, and not the traditional vertex,
normal, and texture coordinate information. Such information is stored in a
VertexArrayObject, located in CommandMap. One VertexArrayObject can contain the
geometric data for several Drawables, which reduces bind calls.

jagDraw module client code (such as an application or loader plugin) creates a
Drawable and adds any required commands.
*/
class JAGDRAW_EXPORT Drawable : protected jagBase::LogBase, public ObjectIDOwner
{
public:
    Drawable();
    Drawable( const Drawable& rhs );
    ~Drawable();
    

    /** \brief TBD
    \details TBD
    */
    virtual void execute( DrawInfo& drawInfo );


    /** \name Bound computation
    \details TBD */
    /*@{*/

    /** \brief Compute (if necessary) and return the bounding volume.
    \details TBD
    \param commands The CommandMap, containing information necessary to compute
    the bound, such as VertexArrayObjects. */
    virtual BoundPtr getBound( const CommandMap& commands );

    /** \brief TBD
    \details TBD */
    void setBoundDirty( const CommandMap& commands, const bool dirty=true );
    /** \brief TBD
    \details TBD */
    bool getBoundDirty( const CommandMap& commands ) const;

    /** \brief TBD
    \details TBD */
    void computeBounds( BoundPtr _bound, const CommandMap& commands );

    struct ComputeBoundCallback {
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


    /** \name Draw commands
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


    /** \name OpenGL ID management
    \details TBD */
    /*@{*/

    /** \brief Tell the Drawable how many contexts to expect.
    \details Drawable iterates over all other attached Jag3D objects and
    passes \c numContexts to their setMaxContexts() member function.
    
    Override from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the Drawable's objects.
    \details OpenGL object ID cleanup is not yet implemented. TBD.

    Override from ObjectIDOwner. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

    /**@}*/


protected:
    DrawCommandVec _drawCommands;

    BoundPtr _bound;
    bool _boundDirty;

    ComputeBoundCallbackPtr _computeBoundCallback;
};

typedef jagBase::ptr< jagDraw::Drawable >::shared_ptr DrawablePtr;
typedef std::vector< DrawablePtr > DrawableVec;


// jagDraw
}


// __JAGDRAW_DRAWABLE_H__
#endif
