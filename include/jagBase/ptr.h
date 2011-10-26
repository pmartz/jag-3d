// Copyright

#ifndef __JAGBASE_PLATFORM_POINTER_H__
#define __JAGBASE_PLATFORM_POINTER_H__ 1


#ifdef JAG3D_USE_BOOST_POINTERS
#  include <boost/shared_ptr>
#else
#  ifdef WIN32
#    include <memory>
#  else
#    include <tr1/memory>
#  endif
#endif


namespace jagBase {


/** \class ptr PlatformPtr.h <jagBase/PlatformPtr.h>
\brief Defines the \c ptr template struct, supporting either Boost or TR1.
*/
template <typename T>
struct ptr
{
#ifdef JAG3D_USE_BOOST_POINTERS
    typedef boost::shared_ptr< T > shared_ptr;
    typedef boost::shared_array< T > shared_array;
#else
    typedef std::tr1::shared_ptr< T > shared_ptr;
    struct shared_array_ptr : public shared_ptr
    { 
        shared_array_ptr() : shared_ptr()
        {}
        shared_array_ptr( T *t )
          : shared_ptr( t, ptr< T >::__array_deleter() )
        {}
    };

    struct __array_deleter
    {
        void operator()( T *t )
        {
            delete[] t;
        }
    };
#endif
};


// jagBase
}


// __JAGBASE_PLATFORM_POINTER_H__
#endif
