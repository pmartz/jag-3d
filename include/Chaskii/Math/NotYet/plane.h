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
#ifndef CHASKII_MATH_PLANE_DEF
#define CHASKII_MATH_PLANE_DEF 1

#include <Chaskii/Math/vec3.h>

namespace iiMath {

template <class T>
class planeT
{
    public:
        T a, b, c, d, l;

        planeT():
            a(0),
            b(0),
            c(1),
            d(0)
        {}

        planeT(const planeT &p ):
            a(p.a),
            b(p.b),
            c(p.c),
            d(p.d),
            l(p.l) {}

        planeT( vec3T<T> pt, vec3T<T> normal )
        {
            set( pt, normal );
        }

        planeT( T _a, T _b, T _c, T _d )
        {
            a = _a;
            b = _b;
            c = _c;
            d = _d;
        }

        void set( T _a, T _b, T _c, T _d )
        {
            a = _a;
            b = _b;
            c = _c;
            d = _d;
        }

        void set( vec3T<T> pt, vec3T<T> normal )
        {
            a = normal[0];
            b = normal[1];
            c = normal[2];
            d = -((a * pt[0]) +
                (b * pt[1]) +
                (c * pt[2])) ;
            l = normal.length();
        }

        void getEquation( double &_a, double &_b, double &_c, double &_d ) const
        {
            _a = a;
            _b = b;
            _c = c;
            _d = d;
        }

        double distanceFrom( const vec3T<T> &pt ) const
        {
            return ((a*pt.x())+(b*pt.y())+(c*pt.z())+d)/l;
        }

        T operator[](unsigned int index) const
        {
            switch( index )
            {
                case 0: return a; break;
                case 1: return b; break;
                case 2: return c; break;
                case 4: return d; break;
                //case 5: return l; break;
                default: return 0;
            }
        }

        bool operator ==  (const planeT &p ) const
        {
            if( a != p.a ) return false;
            if( b != p.b ) return false;
            if( c != p.c ) return false;
            if( d != p.d ) return false;
            return true;
        }

        bool operator !=  (const planeT &p ) const
        {
            if( a != p.a ) return true;
            if( b != p.b ) return true;
            if( c != p.c ) return true;
            if( d != p.d ) return true;
            return false;
        }

        int numcomponents() { return 4; }
        size_t typesize() { return sizeof(T); }
};

typedef planeT<float> planef;
typedef planeT<double> planed;

typedef planed plane;

}

#endif
