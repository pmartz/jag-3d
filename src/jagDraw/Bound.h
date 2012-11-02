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
#include <jagBase/gmtlSupport.h>
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
    typedef enum {
        Box_t,
        Sphere_t
    } BaseType;
    BaseType getType() const { return( _type ); }


    Bound( const BaseType type )
      : _type( type )
    {}
    Bound( const Bound& rhs )
      : _type( rhs._type )
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
    virtual void expand( const Bound& rhs ) = 0;

    virtual void setEmpty( const bool empty=true ) = 0;
    virtual bool getEmpty() const = 0;

protected:
    BaseType _type;
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
      : Bound( Box_t )
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
        if( !( _bound.isInitialized() ) )
            return( gmtl::Sphered() );

        const gmtl::Vec3d extent( _bound.mMax - _bound.mMin );
        const gmtl::Point3d& c( extent * .5 + _bound.mMin );
        const double r( gmtl::length( extent ) * .5 );
        return( gmtl::Sphered( c, r ) );
    }

    virtual bool contains( const gmtl::Point3d& point )
    {
        return( gmtl::isInVolume( _bound, point ) );
    }

    virtual void expand( const gmtl::Point3d& point )
    {
        gmtl::extendVolume( _bound, point );
    }
    virtual void expand( const Bound& rhs )
    {
        if( rhs.getType() == Sphere_t )
        {
            //gmtl::extendVolume( _bound, rhs.asSphere() );
        }
        else
            gmtl::extendVolume( _bound, rhs.asAABox() );
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
class BoundSphere : public Bound
{
public:
    BoundSphere()
      : Bound( Sphere_t )
    {}
    BoundSphere( const BoundSphere& rhs )
      : Bound( rhs ),
        _bound( rhs._bound )
    {}
    ~BoundSphere()
    {}

    virtual gmtl::AABoxd asAABox() const
    {
        if( !( _bound.isInitialized() ) )
            return( gmtl::AABoxd() );

        const gmtl::Point3d& c( _bound.getCenter() );
        const double r( _bound.getRadius() );
        if( r == 0. )
        {
            return( gmtl::AABoxd( c, c ) );
        }
        else
        {
            gmtl::AABoxd box;
            gmtl::makeVolume( box, _bound );
            return( box );
        }
    }
    virtual gmtl::Sphered asSphere() const
    {
        return( _bound );
    }

    virtual bool contains( const gmtl::Point3d& point )
    {
        return( gmtl::isInVolume( _bound, point ) );
    }

    virtual void expand( const gmtl::Point3d& point )
    {
        gmtl::extendVolume( _bound, point );
    }
    virtual void expand( const Bound& rhs )
    {
        if( rhs.getType() == Sphere_t )
            gmtl::extendVolume( _bound, rhs.asSphere() );
        else
            gmtl::extendVolume( _bound, rhs.asAABox() );
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