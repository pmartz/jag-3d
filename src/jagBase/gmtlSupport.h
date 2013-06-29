
#ifndef __JAGBASE_GMTL_SUPPORT_H__
#define __JAGBASE_GMTL_SUPPORT_H__ 1

#include <gmtl/gmtl.h>


namespace gmtl {


    // This file should be used as a placeholder for any additional GMTL
    // functionality needed by Jag3D, but not yet added to GMTL proper.
    // This file is currently unused.


/**
 * Modifies the existing sphere to tightly enclose itself and the given
 * std::vector of points.
 *
 * @param container  [in,out] the sphere that will be extended
 * @param pts        [in]     the std::vector of points which the sphere should contain
 */
template< class DATA_TYPE >
void extendVolume( Sphere<DATA_TYPE>& container,
                   const std::vector< Point<DATA_TYPE, 3> >& pts )
{
   if (! container.isInitialized())
   {
      makeVolume( container, pts );
      return;
   }

   // Algorithm taken from OpenSceneGraph include/osg/BoundingSphere.
   // The idea is to create a temporary box that contains all the input points
   // and also contains the projection of each of those points through the
   // container center onto the opposite side of the container. Then, simply
   // convert that temporary box to a sphere.

   // Create the temp box.
   AABox<DATA_TYPE> tempBox;

   for( typename std::vector< Point<DATA_TYPE, 3> >::const_iterator itr = pts.begin();
      itr != pts.end(); ++itr )
   {
      // Shorthand constant
      const Point<DATA_TYPE, 3>& p( *itr );

      if( isInVolume( container, p ) )
         // Current point is already contained by the sphere.
         continue;

      // Add current point to temp box.
      extendVolume( tempBox, p );

      // Project point through sphere center onto opposite side of
      // sphere, then add it to the temp box.
      Vec<DATA_TYPE,3> v = p - container.mCenter; // get the direction vector from corner
      normalize( v );
      v *= -container.mRadius; // move the vector in the opposite direction distance radius.
      v += container.mCenter; // move to absolute position.
      extendVolume( tempBox, Point<DATA_TYPE,3>( v ) ); // add it into the new bounding box.
   }

   if (! tempBox.isInitialized() )
      // The input points were already contained by the sphere.
      return;

   // Convert temp box to sphere in two steps:
   // 1. Change sphere center to box center
   container.mCenter = (tempBox.mMax - tempBox.mMin) * DATA_TYPE(0.5) + tempBox.mMin;
   // 2. Change sphere radius to be the distance from box center to box corner.
   container.mRadius = length( gmtl::Vec<DATA_TYPE,3>( tempBox.mMin - container.mCenter ) );
}

/**
 * Modifies the container to tightly enclose itself and the given box.
 *
 * @param container  [in,out] the sphere that will be extended
 * @param box        [in]     the box which container should contain
 */
template< class DATA_TYPE >
void extendVolume( Sphere<DATA_TYPE>& container,
                   const AABox<DATA_TYPE>& box )
{
   // Can't extend by an empty sphere
   if (! box.isInitialized())
   {
      return;
   }

   if (container.isInitialized())
   {
      // Shorthand constants
      const Point<DATA_TYPE, 3>& a( box.mMin );
      const Point<DATA_TYPE, 3>& b( box.mMax );

      // Make a std::vector of all eight corner points and extend the volume
      // around the vector of points.
      typename std::vector< Point<DATA_TYPE, 3> > ptList;
      ptList.push_back( a );
      ptList.push_back( Point<DATA_TYPE, 3>( a[0], a[1], b[2] ) );
      ptList.push_back( Point<DATA_TYPE, 3>( a[0], b[1], a[2] ) );
      ptList.push_back( Point<DATA_TYPE, 3>( a[0], b[1], b[2] ) );
      ptList.push_back( Point<DATA_TYPE, 3>( b[0], a[1], a[2] ) );
      ptList.push_back( Point<DATA_TYPE, 3>( b[0], a[1], b[2] ) );
      ptList.push_back( Point<DATA_TYPE, 3>( b[0], b[1], a[2] ) );
      ptList.push_back( b );

      extendVolume( container, ptList );
   }
   else
   {
      container.mCenter = (box.mMax - box.mMin) * DATA_TYPE(0.5) + box.mMin;
      container.mRadius = length( gmtl::Vec<DATA_TYPE,3>( box.mMin - container.mCenter ) );
      container.mInitialized = true;
   }
}



/** \brief Converts a single precision matrix to a double precision matrix. */
template <unsigned ROWS, unsigned COLS>
void convert( gmtl::Matrix< double, ROWS, COLS >& result, const gmtl::Matrix< float, ROWS, COLS >& rhs )
{
    for (unsigned int x = 0; x < ROWS * COLS; ++x)
        result.mData[x] = (double)rhs.mData[x];
    result.mState = rhs.mState;
}
/** \brief Converts a double precision matrix to a single precision matrix. */
template <unsigned ROWS, unsigned COLS>
void convert( gmtl::Matrix< float, ROWS, COLS >& result, const gmtl::Matrix< double, ROWS, COLS >& rhs )
{
    for (unsigned int x = 0; x < ROWS * COLS; ++x)
        result.mData[x] = (float)rhs.mData[x];
    result.mState = rhs.mState;
}

/** \brief Transform a Sphere by a matrix. */
template< typename T, unsigned ROWS, unsigned COLS >
gmtl::Sphere<T>
transform( const gmtl::Matrix<T,ROWS,COLS>& m, const gmtl::Sphere<T>& src )
{
    gmtlASSERT( COLS >= 3 );

    gmtl::Point<T,3> xdashIn = src.getCenter();
    xdashIn[0] += src.getRadius();
    gmtl::Point<T,3> xdash;
    if( COLS == 3 )
        xdash = m * xdashIn;
    else
    {
        gmtl::Point<T,4> temp;
        temp = m * gmtl::Point<T,4>( xdashIn[0], xdashIn[1], xdashIn[2], 1. );
        xdash.set( temp[0], temp[1], temp[2] );
    }

    gmtl::Point<T,3> ydashIn = src.getCenter();
    ydashIn[1] += src.getRadius();
    gmtl::Point<T,3> ydash;
    if( COLS == 3 )
        ydash = m * ydashIn;
    else
    {
        gmtl::Point<T,4> temp;
        temp = m * gmtl::Point<T,4>( ydashIn[0], ydashIn[1], ydashIn[2], 1. );
        ydash.set( temp[0], temp[1], temp[2] );
    }

    gmtl::Point<T,3> zdashIn = src.getCenter();
    zdashIn[2] += src.getRadius();
    gmtl::Point<T,3> zdash;
    if( COLS == 3 )
        zdash = m * zdashIn;
    else
    {
        gmtl::Point<T,4> temp;
        temp = m * gmtl::Point<T,4>( zdashIn[0], zdashIn[1], zdashIn[2], 1. );
        zdash.set( temp[0], temp[1], temp[2] );
    }

    gmtl::Sphere<T> newSphere;
    if( COLS == 3 )
        newSphere.setCenter( m * src.getCenter() );
    else
    {
        gmtl::Point<T,4> temp;
        temp = m * gmtl::Point<T,4>( src.getCenter()[0], src.getCenter()[1], src.getCenter()[2], 1. );
        newSphere.setCenter( gmtl::Point<T,3>( temp[0], temp[1], temp[2] ) );
    }
    

    xdash -= newSphere.getCenter();
    T len_xdash = gmtl::length( gmtl::Vec<T,3>( xdash ) );

    ydash -= newSphere.getCenter();
    T len_ydash = gmtl::length( gmtl::Vec<T,3>( ydash ) );

    zdash -= newSphere.getCenter();
    T len_zdash = gmtl::length( gmtl::Vec<T,3>( zdash ) );

    newSphere.setRadius( len_xdash );
    if( newSphere.getRadius() < len_ydash )
        newSphere.setRadius( len_ydash );
    if( newSphere.getRadius() < len_zdash )
        newSphere.setRadius( len_zdash );

    newSphere.setInitialized();
    return( newSphere );
}



/** \brief Check for potential containment.
\details Returns true if the sphere is potentially inside the frustum.
Returns false if the sphere is clearly outside the frustum.
*/
template<typename T>
inline bool isPotentiallyContained(const Frustum<T>& f, const Sphere<T>& s)
{
   for ( unsigned int i = 0; i < 6; ++i )
   {
      Vec<T, 3> norm( f.mPlanes[i].mNorm );
      normalize( norm );
      T dist = dot(norm, static_cast< Vec<T, 3> >(s.getCenter())) +
          f.mPlanes[i].mOffset + T(s.getRadius());
      if ( dist  < T(0.) )
      {
         return false;
      }
   }

   return true;
}


// gmtl
}


// __JAGBASE_GMTL_SUPPORT_H__
#endif
