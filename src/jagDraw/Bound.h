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

#ifndef __JAGDRAW_BOUND_H__
#define __JAGDRAW_BOUND_H__ 1


#include <jag/base/ptr.h>
#include <jag/base/gmtlSupport.h>
#include <jag/base/LogMacros.h>
#include <gmtl/AABox.h>
#include <gmtl/Sphere.h>
#include <gmtl/Containment.h>
#include <gmtl/VecOps.h>

#include <map>


namespace jagDraw {


// Forward
class Bound;
typedef jag::base::ptr< Bound >::shared_ptr BoundPtr;


/** \addtogroup jagDrawBound Bounding Volume Management */
/*@{*/

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
      : SHARED_FROM_THIS( Bound )(),
      _type( type )
    {}
    Bound( const Bound& rhs )
      : _type( rhs._type )
    {}
    ~Bound()
    {}

    virtual gmtl::AABoxd asAABox() const = 0;
    virtual gmtl::Sphered asSphere() const = 0;
    virtual jagDraw::BoundPtr clone() const = 0;

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

    virtual void transform( const gmtl::Matrix44d& matrix ) = 0;

    virtual void setEmpty( const bool empty=true ) = 0;
    virtual bool getEmpty() const = 0;

    virtual void setCenter( const gmtl::Point3d& center ) = 0;
    virtual gmtl::Point3d getCenter() const = 0;

    virtual double getRadius() const = 0;

protected:
    BaseType _type;
};



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
    BoundAABox( const gmtl::AABoxd& box )
      : Bound( Sphere_t ),
        _bound( box )
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
    virtual BoundPtr clone() const
    {
        return( BoundPtr( new BoundAABox( *this ) ) );
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

    virtual void transform( const gmtl::Matrix44d& matrix )
    {
        JAG3D_CRITICAL_STATIC( "jag.draw.bound", "BoundAABox::transform() is not implemented." );
        // TBD Currently there is no way to transform a box.
        /*
        gmtl::AABoxd newBox( gmtl::transform( matrix, _bound ) );
        _bound = newBox;
        */
    }

    virtual void setEmpty( const bool empty=true )
    {
        _bound.setEmpty( empty );
        if( empty )
        {
            _bound.mMin = _bound.mMax = gmtl::Point3d( 0., 0., 0. );
        }
    }
    virtual bool getEmpty() const
    {
        return( _bound.isEmpty() );
    }

    virtual void setCenter( const gmtl::Point3d& center )
    {
        gmtl::Vec3d delta( center - getCenter() );
        _bound.setMin( _bound.getMin() + delta );
        _bound.setMax( _bound.getMax() + delta );
    }
    virtual gmtl::Point3d getCenter() const
    {
        const gmtl::Point3d center( ( _bound.getMax() - _bound.getMin() ) * 0.5 + _bound.getMin() );
        return( center );
    }

    virtual double getRadius() const
    {
        gmtl::Vec3d vec( _bound.getMax() - _bound.getMin() );
        const double radius( gmtl::length< double >( vec ) * 0.5 );
        return( radius );
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
    BoundSphere( const gmtl::Sphered& sphere )
      : Bound( Sphere_t ),
        _bound( sphere )
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
    virtual BoundPtr clone() const
    {
        return( BoundPtr( new BoundSphere( *this ) ) );
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

    virtual void transform( const gmtl::Matrix44d& matrix )
    {
        gmtl::Sphered newSphere( gmtl::transform( matrix, _bound ) );
        _bound = newSphere;
    }

    virtual void setEmpty( const bool empty=true )
    {
        _bound.setInitialized( !empty );
        if( empty )
        {
            _bound.mCenter.set( 0., 0., 0. );
            _bound.mRadius = 0.;
        }
    }
    virtual bool getEmpty() const
    {
        return( !( _bound.isInitialized() ) );
    }

    virtual void setCenter( const gmtl::Point3d& center )
    {
        _bound.setCenter( center );
    }
    virtual gmtl::Point3d getCenter() const
    {
        return( _bound.getCenter() );
    }

    virtual double getRadius() const
    {
        return( _bound.getRadius() );
    }

protected:
    gmtl::Sphered _bound;
};


/*@}*/


// jagDraw
}


// __JAGDRAW_BOUND_H__
#endif
