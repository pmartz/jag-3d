// Copyright

#ifndef __JAGBASE_EXPORT__
#define __JAGBASE_EXPORT__ 1


#if defined( _MSC_VER ) || defined( __CYGWIN__ ) || defined( __MINGW32__ ) || defined( __BCPLUSPLUS__ ) || defined( __MWERKS__ )
    #if defined( JAG3D_STATIC )
        #define JAGBASE_EXPORT
    #elif defined( JAGBASE_LIBRARY )
        #define JAGBASE_EXPORT __declspec( dllexport )
    #else
        #define JAGBASE_EXPORT __declspec( dllimport )
    #endif
#else
    #define JAGBASE_EXPORT
#endif


// __JAGBASE_EXPORT__
#endif
