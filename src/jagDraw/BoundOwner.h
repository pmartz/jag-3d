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

#ifndef __JAGDRAW_BOUND_OWNER_H__
#define __JAGDRAW_BOUND_OWNER_H__ 1


#include <jagBase/ptr.h>
#include <jagBase/Profile.h>
#include <jagDraw/Bound.h>
#include <jagBase/Notifier.h>
#include <jagDraw/CommandMap.h>

#include <boost/foreach.hpp>

#include <map>


namespace jagDraw {


/** \class BoundOwner BoundOwner.h <jagDraw/BoundOwner.h>
\brief Common base class for objects that own bounding volumes.
\details TBD
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
    and a BoundSphere for subclass jagSG::Node. */
    virtual BoundPtr newBound() = 0;

    /** \brief Returns a bounding volume.
    \details 
    \param  The CommandMap required for bound computation.

    \specFuncBegin

    If _dirty is true, this function performs the following operations:
    - If _bound is NULL, this function allocates a new bound.
      If _initialBound is not NULL, it's cloned to create this new bound.
    - If _computeBoundCallback is non-NULL, this function executes the callback.
      Otherwise, this function calls computeBound().
    - _dirty is set to false.

    Regardless of whether _dirty was initially true or false, this function
    returns the value of _bound.

    If _computeBoundCallback == NULL and the bound is <em>uncomputable</em>,
    the return value will always be uninitialized. If _computeBoundCallback
    is non-NULL, _computeBoundCallback is entirely responsible for determining
    the return value.

    \specTableBegin
    \specThread{Thread Safe}
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
                computeBound( _bound, commands, this );
            _dirty = false;
        }

        return( _bound );
    }

    /** \brief Computes a bounding volume.
    \details
    
    \specFuncBegin

    Computes the bounding volume for the given CommandMap
    \c commands.

    The bound could be <em>uncomputable</em> if the BoundOwner subclass
    doesn't have enough information to compute the bound. Valid reasons
    include, but are not limited to:
    \li \c vao is NULL.
    \li \c vao does not contain a non-NULL BufferObjectPtr marked as VertexArrayObject::Vertex.
    \li \c vao does not contain a non-NULL VertexAttribPtr marked as VertexArrayObject::Vertex.
    See the BoundOwner subclasses (jagDraw::Drawable and jagSG::Node)
    for <em>uncomputable</em> bound reasons.

    \specTableBegin
    \specThread{Thread Safe}
    \specDepend{Bound\, VertexArrayObject}
    \specTableEnd
    \specFuncEnd
    */
    virtual void computeBound( BoundPtr& bound, const jagDraw::CommandMap& commands, BoundOwner* owner ) = 0;


    /** \brief Set an initial bounding volume.
    \details The BoundOwner's bounding volume assumes the Bound::BaseType
    of _initialBound, and is guaranteed to fully enclose _initialBound.
    
    See getBound() for how the _initialBound is used.

    Possible application use cases include assigning a minimum volume
    extent, and specifying use of BoundSphere or BoundAABox. */
    void setInitialBound( const BoundPtr& initialBound )
    {
        _initialBound = initialBound;
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
    that the bound has been dirtied.
    \see setBoundDirtyFlag().

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
        /**

        \specFuncBegin
        \specTableBegin
        \specThread{Thread Safe}
        \specDepend{CommandMap}
        \specTableEnd
        \specFuncEnd
        */
        virtual void operator()( BoundPtr& _bound, const jagDraw::CommandMap& commands, BoundOwner* owner ) = 0;
    };
    typedef jagBase::ptr< ComputeBoundCallback >::shared_ptr ComputeBoundCallbackPtr;

    /** \brief TBD
    \details TBD */
    void setComputeBoundCallback( ComputeBoundCallbackPtr computeBoundCallback )
    {
        _computeBoundCallback = computeBoundCallback;
        setBoundDirty();
    }
    /** \brief TBD
    \details TBD */
    ComputeBoundCallbackPtr getComputeBoundCallback() const
    {
        return( _computeBoundCallback );
    }


    struct BoundDirtyNotifyInfo : jagBase::Notifier::NotifierInfo
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


// jagDraw
}


// __JAGDRAW_BOUND_OWNER_H__
#endif
