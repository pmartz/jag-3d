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
#ifndef CHASKII_MATH_MATH_DEF
#define CHASKII_MATH_MATH_DEF 1

#include <sys/types.h>

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES //to get rid of annoying Warnings related to M_PI redefinition in Visual Studio
#endif

#include <math.h>
#include <Chaskii/Export.h>


#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif

#ifndef M_PIF // For visual Studio's incessant insistence on a suffix
#  define M_PIF 3.141592653589f
#endif

#ifndef M_PI_2
# define M_PI_2		1.57079632679489661923132169163975144 	/* pi/2 */
#endif

namespace iiMath {

class II_EXPORT Math
{
public:
    template<class _Ty> static int      sign( _Ty x );
    template<class _Ty> static _Ty      _max_( _Ty x, _Ty y );
    template<class _Ty> static _Ty      _min_( _Ty x, _Ty y );
    template<class _Ty> static _Ty      clamp( _Ty x, _Ty min, _Ty max );
    template<class _Ty> static _Ty      abs( _Ty );
    static double                       snapGrid( double  x, double grid );    // snaps x to be a multiple of grid
};

template<class _Ty> inline int Math::sign( _Ty x ) { return (x < (_Ty)0 ? -1 :  1); }

template<class _Ty> inline _Ty Math::abs( _Ty x ) { return x < (_Ty)0 ? -x : x; }

template<class _Ty> inline _Ty Math::_max_( _Ty x, _Ty y ) {
  return ( x > y ) ? x : y;
}

template<class _Ty> inline _Ty Math::_min_( _Ty x, _Ty y ) {
  return ( x < y ) ? x : y;
}

template<class _Ty> inline _Ty Math::clamp( _Ty x, _Ty min, _Ty max ) {
  return ( x < min ) ? min : ( ( x > max ) ? max : x );
}

inline double Math::snapGrid( double x, double grid ) {
  return ( grid == 0 ) ? x : ( floor( ( x + 0.5f * grid ) / grid ) * grid );
}

template <typename T>
inline T deg2rad(const T x)
{ return T(x*(M_PI/180.0)); }

template <typename T>
inline T rad2deg(const T x)
{ return T(x*(180.0/M_PI)); }

template <typename T>
inline T sqr(const T x)
{ return x*x; }

inline float round(float v) { return v>=0.0f?floorf(v+0.5f):ceilf(v-0.5f); }
inline double round(double v) { return v>=0.0?floor(v+0.5):ceil(v-0.5); }

inline bool isPowerOfTwo( unsigned int n )
{
    unsigned int i = 1;
    while( i < n ) i <<= 1;
    if( i == n ) return true; else return false;
}


/* Compute the nearest power of 2 number that is
 * less than or equal to the value passed in.
 */
inline int nearestPower( int value )
{
    int i = 1;

    if (value == 0) return -1;  /* Error! */
    for (;;) {
        if (value == 1) return i;
        else if (value == 3) return i*4;
        value >>= 1; i *= 2;
    }
}

}


#endif
