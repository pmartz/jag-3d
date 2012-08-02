
#ifndef __JAGBASE_GMTL_SUPPORT_H__
#define __JAGBASE_GMTL_SUPPORT_H__ 1

#include <gmtl/gmtl.h>


namespace gmtl {


template <typename T>
inline Matrix<T, 4,4>& setLookAt( Matrix<T, 4,4>& result,
    const Point<T, 3>& eye, const Point<T, 3>& center, const Vec<T, 3>& up )
{
    Vec<T, 3> f( center-eye ); normalize( f );
    Vec<T, 3> s; cross( s, f, up ); normalize( s );
    Vec<T, 3> u; cross( u, s, f ); normalize( u );

    Matrix<T, 4,4> orient;
    zero( orient );
    orient(0,0) = s[0]; orient(0,1) = s[1]; orient(0,2) = s[2];
    orient(1,0) = u[0]; orient(1,1) = u[1]; orient(1,2) = u[2];
    orient(2,0) = -f[0]; orient(2,1) = -f[1]; orient(2,2) = -f[2];
    orient(3,3) = T(1.);

    result = orient * makeTrans< Matrix<T, 4,4> >( -eye );
    result.mState = Matrix<T, 4,4>::AFFINE;
    return( result );
}


// gmtl
}


// __JAGBASE_GMTL_SUPPORT_H__
#endif
