/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGDRAW_BOUND_OWNER_H__
#define __JAGDRAW_BOUND_OWNER_H__ 1


#include <jag/base/ptr.h>
#include <jag/base/Profile.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Bound.h>
#include <jag/base/Notifier.h>


namespace jagDraw {


/** \addtogroup jagDrawBound Bounding Volume Management */
/*@{*/

/** \class BoundOwner BoundOwner.h <jagDraw/BoundOwner.h>
\brief Common base class for objects that own bounding volumes.
\details Currently the two derived classes are jagDraw::Drawable and
jagSG::Node.

\specBegin BoundOwner

BoundOwner supports the following functionality:
\li Storage for (or a pointer to) a bounding volume (jagDraw::Bound).
\li Bounding volume initialization.
\li Bounding volume access.
\li Bounding volume computation.
\li A flag to indicate the bound has been computed.

Each BoundOwner instance stores the following information:
\li A bound volume or pointer to same.
\li A Boolean indication of the bound computation status.
\li An initial / least bound volume (or pointer to same).
\li A computation callback for custom bounds computation.

The bounding volume is retrieved by a call to getBound(), which
takes a CommandMap as a paramater. The CommandMap contains
VertexArrayObject, which jagDraw::Drawables iterate over to
compute their bounding volume.

If the initial bound (see setInitialBound()) is non-NULL,
the bounding volume is always guaranteed to be at
least isometric to the initial bound.

The bounding volume will be undefined if there is no
initial bounding bolume, no compute bounds callback, and
a BoundOwner is accessed by CommandMaps containing
different VertexArrayObjects. This could happen, for example,
during traversal of a multiparented scene graph.

The returned bounding volume will be empty if there is no
initial bounding volume, no copmpute bounds callback, and
there are no subordinate BoundOwner objects that compute a
non-empty bound. This could happen, for example, if a
scene graph subgraph contains no Drawables, or Drawables
with no DrawCommands. It could also happen if the CommandMap
paremeter to computeBounds() contains no vertex data in
the VertexArrayObject.

\specTableBegin
\specThread{Unsafe}
\specGL{None}
\specDepend{CommandMap\, Bound}
\specUsage{TBD}
\specViolations{None}
\specTableEnd

See member functions for additional specification requirements.
\specEnd

*/
class BoundOwner
{
public:
    /** Constructor */
    BoundOwner()
        : _dirty( true ),
        _computeBoundCallback( NULL )
    {
    }
    /** Copy constructor */
    BoundOwner( const BoundOwner& rhs )
      : _bound( rhs._bound ),
        _dirty( rhs._dirty ),
        _initialBound( rhs._initialBound ),
        _computeBoundCallback( rhs._computeBoundCallback )
    {
    }
    /** Destructor */
    ~BoundOwner()
    {
    }


    /** \brief Create a new uninitialized bound.
    \details This function is called by getBound() if \c _initialBound is NULL.
    Typically, this function returns a BoundAABoxPtr for subclass jagDraw::Drawable,
    and a BoundSphere for subclass jagSG::Node.

    \specFuncBegin
    \specTableBegin
    \specThread{Thread Safe}
    \specTableEnd
    \specFuncEnd
    */
    virtual BoundPtr newBound() = 0;

    /** \brief Returns a bounding volume.
    \details 
    \param commands The CommandMap required for bound computation.

    \specFuncBegin

    If _dirty is true, this function performs the following operations:
    - If _bound is NULL, this function allocates a new bound.
      If _initialBound is not NULL, it's cloned to create this new bound.
    - If _computeBoundCallback is non-NULL, this function executes the callback.
      Otherwise, this function calls computeBound().
    - _dirty is set to false.

    Regardless of whether _dirty was initially true or false, this function
    returns the value of _bound.

    \specTableBegin
    \specThread{Unsafe}
    \specDepend{CommandMap}
    \specTableEnd
    \specFuncEnd
    */
    virtual const BoundPtr& getBound( const jagDraw::CommandMap& commands )
    {
        JAG3D_PROFILE( "BoundOwner::getBound" );

        if( _dirty )
        {
            if( _bound == NULL )
            {
                if( _initialBound == NULL )
                    _bound = newBound();
                else
                    _bound = _initialBound->clone();
            }
            else
            {
                _bound->setEmpty();
            }
            if( _computeBoundCallback != NULL )
                (*_computeBoundCallback)( _bound, commands, this );
            else
                computeBound( _bound, commands );
            _dirty = false;
        }

        return( _bound );
    }

    /** \brief Computes a bounding volume.
    \details
    
    \specFuncBegin

    Computes the bounding volume for the given CommandMap
    \c commands.

    The bound could be empty if the BoundOwner subclass doesn't
    have enough information to compute the bound. See the
    BoundOwner subclasses (jagDraw::Drawable and jagSG::Node)
    for <em>uncomputable</em> bound reasons.

    \specTableBegin
    \specThread{Thread Safe}
    \specDepend{Bound\, CommandMap}
    \specTableEnd
    \specFuncEnd
    */
    virtual void computeBound( BoundPtr& bound, const jagDraw::CommandMap& commands ) = 0;


    /** \brief Set an initial bounding volume.
    \details The BoundOwner's bounding volume assumes the Bound::BaseType
    of _initialBound, and is guaranteed to be at least isometric to
    _initialBound.
    
    See getBound() for how the _initialBound is used.

    Possible application use cases include assigning a minimum volume
    extent, and specifying use of BoundSphere or BoundAABox. */
    void setInitialBound( const BoundPtr& initialBound )
    {
        _initialBound = initialBound;
        _bound = NULL;
        setBoundDirty();
    }
    /** \brief Return the _initialBound.
    \details TBD */
    BoundPtr& getInitialBound()
    {
        return( _initialBound );
    }
    /** \overload */
    const BoundPtr& getInitialBound() const
    {
        return( _initialBound );
    }

    /** \brief Mark a bound as dirty.
    \details Set the _dirty flag to \c dirty.

    Typically, classes derived from BoundOwner (jagDraw::Drawable and jagSG::Node)
    override this function to notify observer classes (parent jagSG::Node objects)
    that the bound has been dirtied. See setBoundDirtyFlag(), which simply sets
    the dirty flag without notifying observers.

    \specFuncBegin
    \specTableBegin
    \specThread{Thread Safe}
    \specTableEnd
    \specFuncEnd
    */
    virtual void setBoundDirty( const bool dirty=true )
    {
        _dirty = dirty;
    }
    /** \brief Mark a bound as dirty.
    \details Set the _dirty flag to \c dirty.

    This is not a virtual function and can't be overridden by derived classes.
    Use this function to mark a bound as dirty without notifying any observers. */
    void setBoundDirtyFlag( const bool dirty=true )
    {
        _dirty = dirty;
    }
    /** \brief Return the dirty state for a specific Bound.
    \details TBD

    \specFuncBegin
    \specTableBegin
    \specThread{Thread Safe}
    \specTableEnd
    \specFuncEnd
    */
    bool getBoundDirty() const
    {
        return( _dirty );
    }


    /** \struct ComputeBoundCallback BoundOwner.h <jagDraw/BoundOwner.h>
    \brief Custom bound computation support.
    \details TBD */
    struct ComputeBoundCallback {
        /** \brief Function call operator for computing bounds.

        \specFuncBegin
        \specTableBegin
        \specThread{Thread Safe}
        \specDepend{CommandMap}
        \specTableEnd
        \specFuncEnd
        */
        virtual void operator()( BoundPtr& _bound, const jagDraw::CommandMap& commands, BoundOwner* owner ) = 0;
    };
    typedef jag::base::ptr< ComputeBoundCallback >::shared_ptr ComputeBoundCallbackPtr;

    /** \brief Set the compute bounds callback and mark bounds as dirty.
    \details By marking bounds dirty, all observers could be notified.
    See setBoundDirty() */
    void setComputeBoundCallback( ComputeBoundCallbackPtr computeBoundCallback )
    {
        _computeBoundCallback = computeBoundCallback;
        setBoundDirty();
    }
    /** \brief Get the compute bounds callback.
    \details TBD */
    ComputeBoundCallbackPtr getComputeBoundCallback() const
    {
        return( _computeBoundCallback );
    }


    /** TBD jagDraw does not depend on jagSG. Consequently,
    jagDraw::Drawable needs a callback mechanism to notify owning
    jagSG::Node objects that a bound has changed. TBD consider
    moving to the Poco notification system. */
    struct BoundDirtyNotifyInfo : jag::base::Notifier::NotifierInfo
    {
        BoundDirtyNotifyInfo()
        {}
    };

protected:
    /** Default value: _bound = BoundPtr() */
    BoundPtr _bound;
    /** Default value: true */
    bool _dirty;

    /** Default value: _initialBound = BoundPtr() */
    BoundPtr _initialBound;

    /** Default value: _computeBoundCallback = ComputeBoundCallbackPtr() */
    ComputeBoundCallbackPtr _computeBoundCallback;
};


/*@}*/


// jagDraw
}


// __JAGDRAW_BOUND_OWNER_H__
#endif
