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

#ifndef __JAGMX_EXPORT_H__
#define __JAGMX_EXPORT_H__ 1


#if defined( _MSC_VER ) || defined( __CYGWIN__ ) || defined( __MINGW32__ ) || defined( __BCPLUSPLUS__ ) || defined( __MWERKS__ )
    #if defined( JAG3D_STATIC )
        #define JAGMX_EXPORT
    #elif defined( JAGMX_LIBRARY )
        #define JAGMX_EXPORT __declspec( dllexport )
    #else
        #define JAGMX_EXPORT __declspec( dllimport )
    #endif
#else
    #define JAGMX_EXPORT
#endif


// __JAGMX_EXPORT_H__
#endif
