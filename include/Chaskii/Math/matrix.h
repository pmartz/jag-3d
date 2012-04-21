/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_MATH_MATRIX_DEF
#define CHASKII_MATH_MATRIX_DEF 1

#include <gmtl/gmtl.h>

namespace iiMath {

typedef gmtl::Matrix44f matrix4f;
typedef gmtl::Matrix44d matrix4d;
typedef matrix4d matrix4;

typedef gmtl::Matrix33f matrix3f;
typedef gmtl::Matrix33d matrix3d;
typedef matrix3d matrix3;

/*
In gmtl/Generate.h

   inline Matrix<T, 4,4>& setPerspective( Matrix<T, 4, 4>& result,

   inline Matrix<T, 4,4>& setFrustum( Matrix<T, 4, 4>& result,
                                                   T left, T top, T right,
                                                   T bottom, T nr, T fr )
*/

template <typename T>
inline gmtl::Matrix<T,4,4> Frustum( T l, T r, T b, T t, T n, T f )
{
    gmtl::Matrix<T,4,4> m;
    m[0][0] = (2.0f * n)/(r - l);
    m[1][0] = 0.0;
    m[2][0] = 0.0;
    m[3][0] = 0.0;

    m[0][1] = 0.0;
    m[1][1] = (2. * n)/(t-b);
    m[2][1] = 0.0;
    m[3][1] = 0.0;

    m[0][2] = (r + l)/(r-l);
    m[1][2] = (t+b)/(t-b);
    m[2][2] = -(f + n)/(f-n);
    m[3][2] = -1.0;

    m[0][3] = 0.0;
    m[1][3] = 0.0;
    m[2][3] = -(2 * f * n)/(f-n);
    m[3][3] = 0.0;
    return m;
}

inline matrix4f Frustumf( float l, float r, float b, float t, float n, float f )
{
    return Frustum<float>(l,r,b,t,n,f);
}

inline matrix4d Frustumd( double l, double r, double b, double t, double n, double f )
{
    return Frustum<double>(l,r,b,t,n,f);
}

template <typename T>
inline gmtl::Matrix<T,4,4> Perspective( T vfov, T aspectRatio, T nearClip, T farClip )
{
    T vfov_r = vfov*M_PI/180.0;//iiMath::deg2rad<T>(vfov);
    T o = nearClip * tan( vfov_r * 0.5);
    T b = -o;
    T t =  o;
    T l = -o * aspectRatio;
    T r  =  o * aspectRatio;
    return Frustum<T>( l, r, b, t, nearClip, farClip );
}

template <typename T>
inline gmtl::Matrix<T,4,4> Translate( T x, T y, T z )
{
    gmtl::Matrix<T,4,4>m;
    m[0][0] = 1.0;
    m[0][1] = 0.0;
    m[0][2] = 0.0;
    m[0][3] = x;

    m[1][0] = 0.0;
    m[1][1] = 1.0;
    m[1][2] = 0.0;
    m[1][3] = y;

    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
    m[2][3] = z;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;

    return m;
}

inline matrix4f Translatef( float x, float y, float z )
{
    return Translate<float>(x,y,z);
}

inline matrix4d Translated( double x, double y, double z )
{
    return Translate<double>(x,y,z);
}

template <typename T>
inline gmtl::Matrix<T,4,4> Rotate( T angle, T x, T y, T z )
{
    gmtl::AxisAngle<T> aa( angle, x, y, z );
    return gmtl::make< gmtl::Matrix<T,4,4> >( aa );
}

inline matrix4f Rotatef( float angle, float x, float y, float z )
{
    return Rotate<float>(angle,x,y,z);
}

inline matrix4d Rotated( double angle, double x, double y, double z )
{
    return Rotate<double>(angle,x,y,z);
}
   

template <typename T>
inline gmtl::Matrix<T,4,4> Scale( T scale )
{
    gmtl::Matrix<T,4,4> m;
    gmtl::setScale( m, scale );
    return m;
}

inline matrix4f Scalef( float scale )
{
    return Scale<float>(scale);
}

inline matrix4d Scaled( double scale )
{
    return Scale<double>(scale);
}

/*
template <typename T>
inline gmtl::Vec<T,3> operator *( gmtl::Matrix<T,4,4> m , gmtl::Vec<T, 3> v  )
{
    gmtl::Vec<T, 3> r;
    gmtl::xform( r, m, v );
    return r;
}
*/




}

#endif
