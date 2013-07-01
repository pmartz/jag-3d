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


#include <jagDraw/Bound.h>
#include <jagBase/ptr.h>

#include <boost/foreach.hpp>

#include <map>


namespace jagDraw {


// forward:
class VertexArrayObject;

/** \struct BoundInfo BoundOwner.h <jagDraw/BoundOwner.h>
\brief A bounding volume with associated data.
\details Allows a bounding volume and dirty flag per VertexArrayObject.
See BoundMap.
\details TBD */
struct BoundInfo {
    BoundInfo()
        : _dirty( true )
    {}
    BoundInfo( const BoundInfo& rhs )
        : _dirty( rhs._dirty ),
        _bound( rhs._bound )
    {}
    bool _dirty;
    jagDraw::BoundPtr _bound;
};
typedef std::map< const jagDraw::VertexArrayObject*, BoundInfo > BoundMap;



/** \class BoundOwner BoundOwner.h <jagDraw/BoundOwner.h>
\brief Common base class for objects that own bounding volumes.
\details TBD
*/
class BoundOwner
{
public:
    BoundOwner()
    {
    }
    BoundOwner( const BoundOwner& rhs )
      : _bounds( rhs._bounds ),
        _initialBound( rhs._initialBound ),
        _computeBoundCallback( rhs._computeBoundCallback )
    {
    }
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
    \param  The VertexArrayObject required for bound computation.

    \specFuncBegin

    This function creates a jagDraw::BoundInfo entry in the Drawable::_bounds map,
    if it doesn't already exist.
    If BoundInfo::_dirty is true, this function performs the following operations:
    - If BoundInfo::_bound is NULL, this function allocates a new bound.
      If _initialBound is not NULL, it's cloned to create this new bound.
    - If Drawable::_computeBoundCallback() is non-NULL, this function executes the callback.
      Otherwise, this function calls computeBound().
    - BoundInfo::_dirty is set to false.

    Regardless of whether BoundInfo::_dirty was initially true or false, this function
    returns the value of BoundInfo::_bound.

    If Drawable::_computeBoundCallback == NULL and the bound is <em>uncomputable</em>,
    the return value will always be uninitialized. If Drawable::_computeBoundCallback
    is non-NULL, Drawable::_computeBoundCallback is entirely responsible for determining
    the return value.

    \specTableBegin
    \specDepend{VertexArrayObject}
    \specTableEnd
    \specFuncEnd
    */
    virtual BoundPtr getBound( const VertexArrayObject* vao )
    {
        boost::mutex::scoped_lock lock( _mutex );

        BoundInfo& boundInfo( _bounds[ vao ] );
        if( boundInfo._dirty )
        {
            if( boundInfo._bound == NULL )
            {
                if( _initialBound == NULL )
                    boundInfo._bound = newBound();
                else
                    boundInfo._bound = _initialBound->clone();
            }
            else
            {
                boundInfo._bound->setEmpty();
            }
            if( _computeBoundCallback != NULL )
                (*_computeBoundCallback)( boundInfo._bound, vao );
            else
                computeBound( boundInfo._bound, vao );
            boundInfo._dirty = false;
        }

        return( boundInfo._bound );
    }

    /** \brief TBD
    \details TBD */
    void setInitialBound( BoundPtr initialBound )
    {
        _initialBound = initialBound;
        setAllBoundsDirty();
    }
    /** \brief TBD
    \details TBD */
    BoundPtr getInitialBound() const
    {
        return( _initialBound );
    }

    /** \brief TBD
    \details TBD

    \specFuncBegin
    \specTableBegin
    \specDepend{VertexArrayObjectPtr}
    \specTableEnd
    \specFuncEnd
    */
    void setBoundDirty( const VertexArrayObject* vao, const bool dirty=true )
    {
        boost::mutex::scoped_lock lock( _mutex );
        _bounds[ vao ]._dirty = dirty;
    }
    void setAllBoundsDirty()
    {
        boost::mutex::scoped_lock lock( _mutex );
        BOOST_FOREACH( BoundMap::value_type& mapElement, _bounds )
        {
            mapElement.second._dirty = true;
        }
    }
    /** \brief TBD
    \details TBD

    \specFuncBegin
    \specTableBegin
    \specDepend{VertexArrayObjectPtr}
    \specTableEnd
    \specFuncEnd
    */
    bool getBoundDirty( const VertexArrayObject* vao ) const
    {
        boost::mutex::scoped_lock lock( _mutex );

        BoundMap::const_iterator it( _bounds.find( vao ) );
        if( it != _bounds.end() )
            return( it->second._dirty );
        else
            return( true );
    }

    /** \brief Computes a bounding volume.
    \details
    
    \specFuncBegin

    Computes the bounding volume based on Drawable::_drawCommands,
    Drawable::_computeBoundCallback, and the specified VertexArrayObject
    \c vao.

    If any of the following conditions are true, the bound is
    <em>uncomputable</em>:
    \li \c vao is NULL.
    \li \c vao does not contain a non-NULL BufferObjectPtr marked as VertexArrayObject::Vertex.
    \li \c vao does not contain a non-NULL VertexAttribPtr marked as VertexArrayObject::Vertex.
    \li Drawable::_drawCommands.size() == 0.

    \specTableBegin
    \specDepend{Bound\, VertexArrayObject

        Jag3D uses VertexAttribContainer to compute the bound\, but this is not a
        JAG specification requirement. }
    \specTableEnd
    \specFuncEnd
    */
    virtual void computeBound( BoundPtr bound, const VertexArrayObject* vao ) = 0;

    struct ComputeBoundCallback {
        /**

        \specFuncBegin
        \specTableBegin
        \specDepend{VertexArrayObject}
        \specTableEnd
        \specFuncEnd
        */
        virtual void operator()( BoundPtr _bound, const VertexArrayObject* vao ) = 0;
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

protected:
    /** Default value: For each element in _bounds, BoundInfo::_dirty
    is initially true. The first call to getBound() initializes the
    entry further. If \c _initialBound is NULL, a new
    jagDraw::BoundAABox is allocated; otherwise, \c _initialBOund
    is cloned. BoundInfo::_bound is expanded around all applicable
    vertex data, and BoundInfo::_dirty is set to false. */
    BoundMap _bounds;
    /** Default value: NULL */
    BoundPtr _initialBound;
    /** \brief Lock around _bounds BoundInfo map. */
    mutable boost::mutex _mutex;

    /** Default value: _computeBoundCallback = ComputeBoundCallbackPtr() */
    ComputeBoundCallbackPtr _computeBoundCallback;
};


// jagDraw
}


// __JAGDRAW_BOUND_OWNER_H__
#endif
