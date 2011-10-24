// Copyright

#ifndef __JAGDRAW_EXPORT__
#define __JAGDRAW_EXPORT__ 1


#if defined( _MSC_VER ) || defined( __CYGWIN__ ) || defined( __MINGW32__ ) || defined( __BCPLUSPLUS__ ) || defined( __MWERKS__ )
    #if defined( JAG3D_STATIC )
        #define JAGDRAW_EXPORT
    #elif defined( JAGDRAW_LIBRARY )
        #define JAGDRAW_EXPORT __declspec( dllexport )
    #else
        #define JAGDRAW_EXPORT __declspec( dllimport )
    #endif
#else
    #define JAGDRAW_EXPORT
#endif


// __JAGDRAW_EXPORT__
#endif
