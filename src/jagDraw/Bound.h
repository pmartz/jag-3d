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

#ifndef __JAGDRAW_BOUND_H__
#define __JAGDRAW_BOUND_H__ 1


#include <jagBase/ptr.h>
#include <gmtl/AABox.h>
#include <gmtl/Sphere.h>
#include <gmtl/Containment.h>
#include <gmtl/VecOps.h>


namespace jagDraw {


/** \class Bound Bound.h <jagDraw/Bound.h>
\brief TBD
\details TBD
*/
class Bound : public SHARED_FROM_THIS(Bound)
{
public:
    Bound()
    {}
    Bound( const Bound& rhs )
    {}
    ~Bound()
    {}

    virtual gmtl::AABoxd asAABox() const = 0;
    virtual gmtl::Sphered asSphere() const = 0;

    virtual bool contains( const gmtl::Point3f& point )
    {
        return( contains( gmtl::Point3d( point[0], point[1], point[2] ) ) );
    }
    virtual bool contains( const gmtl::Point3d& point ) = 0;

    virtual void expand( const gmtl::Point3f& point )
    {
        expand( gmtl::Point3d( point[0], point[1], point[2] ) );
    }
    virtual void expand( const gmtl::Point3d& point ) = 0;

    virtual void setEmpty( const bool empty=true ) = 0;
    virtual bool getEmpty() const = 0;
};

typedef jagBase::ptr< jagDraw::Bound >::shared_ptr BoundPtr;


/** \class BoundAABox Bound.h <jagDraw/Bound.h>
\brief TBD
\details TBD
*/
class BoundAABox : public Bound
{
public:
    BoundAABox()
    {}
    BoundAABox( const BoundAABox& rhs )
      : Bound( rhs ),
        _bound( rhs._bound )
    {}
    ~BoundAABox()
    {}

    virtual gmtl::AABoxd asAABox() const
    {
        return( _bound );
    }
    virtual gmtl::Sphered asSphere() const
    {
        const gmtl::Vec3d extent( _bound.mMax - _bound.mMin );
        const gmtl::Point3d& c( extent * .5 + _bound.mMin );
        const double r( gmtl::length( extent ) * .5 );
        return( gmtl::Sphered( c, r ) );
    }

    virtual bool contains( const gmtl::Point3d& point )
    {
        return( false );
    }

    virtual void expand( const gmtl::Point3d& point )
    {
        gmtl::extendVolume( _bound, point );
    }

    virtual void setEmpty( const bool empty=true )
    {
        _bound.setEmpty( empty );
    }
    virtual bool getEmpty() const
    {
        return( _bound.isEmpty() );
    }

protected:
    gmtl::AABoxd _bound;
};


/** \class BoundSphere Bound.h <jagDraw/Bound.h>
\brief TBD
\details TBD
*/
template< typename T >
class BoundSphere : public Bound
{
public:
    BoundSphere()
    {}
    BoundSphere( const BoundSphere& rhs )
      : Bound( rhs ),
        _bound( rhs._bound )
    {}
    ~BoundSphere()
    {}

    virtual gmtl::AABoxd asAABox() const
    {
        gmtl::Point3d& c( _bound.getCenter() );
        double r( _bound.getRadius );
        if( r == 0. )
        {
            return( gmtl::AABoxd( c, c ) );
        }
        else
        {
            const gmtl::Point3d rad( r, r, r );
            return( gmtl::AABoxd( c-rad, c+rad ) );
        }
    }
    virtual gmtl::Sphered asSphere() const
    {
        return( _bound );
    }

    virtual bool contains( const gmtl::Point3d& point )
    {
        return( false );
    }

    virtual void expand( const gmtl::Point3d& point )
    {
        gmtl::extendVolume( _bound, point );
    }

    virtual void setEmpty( const bool empty=true )
    {
        //_bound.setEmpty( empty );
    }
    virtual bool getEmpty() const
    {
        return( false );//_bound.isEmpty() );
    }

protected:
    gmtl::Sphered _bound;
};


// jagDraw
}


// __JAGDRAW_BOUND_H__
#endif
