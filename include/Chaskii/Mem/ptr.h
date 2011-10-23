#ifndef IIMEM_PTR_DEF
#define IIMEM_PTR_DEF

#include <tr1/memory>
#include <Chaskii/Mem/ref_ptr.h>

namespace iiMem {

template <typename T>
struct ptr
{ 
#if 1
    typedef std::tr1::shared_ptr<T> shared_ptr;
    struct shared_array_ptr: public shared_ptr
    { 
        shared_array_ptr(): shared_ptr() {}
        shared_array_ptr(T *t): shared_ptr( t, ptr<T>::__array_deleter()) {}
    };

    struct __array_deleter { void operator()( T *t ) { delete [] t; } };
#else
    typedef ref_ptr<T> shared_ptr;
    typedef ref_array_ptr<T> shared_array_ptr;
#endif
};

}

#endif
