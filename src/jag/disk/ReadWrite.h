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

#ifndef __JAG_DISK_READ_WRITE_H__
#define __JAG_DISK_READ_WRITE_H__ 1


#include <jag/disk/Export.h>
#include <jag/base/Config.h>

#include <boost/any.hpp>
#include <string>


extern "C"
{
    typedef void ( *CPluginFunction )( void );
}


namespace jag {
namespace disk {



/** \struct PluginFunctionProxy ReadWrite.h <jag/disk/ReadWrite.h>
\brief Utility struct for referencing plugins during static link.
\details Creating a variable of this type causes the \c function parameter to be
invoked during construction. In the intended use case (see JAG3D_REFERENCE_PLUGIN),
this ensures the plugin library's static initialization code executes, resulting
in the plugin being included in the link and registered with PluginManager. */
struct PluginFunctionProxy
{
    PluginFunctionProxy( CPluginFunction function ){ (function)(); }
};


/** \def JAG3D_REFERENCE_PLUGIN
\brief Client / application utility macro to ensure a specified plugin 
is referenced during application link.
\details App code should use JAG3D_REFERENCE_PLUGIN for every JAG plugin
it intends to reference. */
#ifdef JAG3D_STATIC

#  define JAG3D_REFERENCE_PLUGIN( _plugin ) \
      extern "C" void jagp_##_plugin( void ); \
      static jag::disk::PluginFunctionProxy proxy_##_plugin( jagp_##_plugin );

#else

#  define JAG3D_REFERENCE_PLUGIN( _plugin )

#endif



// forward
class Options;


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/

/** \brief
\details
*/
JAGDISK_EXPORT boost::any read( const std::string& fileName, const Options* options=NULL );

/** \brief
\details
*/
JAGDISK_EXPORT bool write( const std::string& fileName, const void* data, const Options* options=NULL );

/**@}*/


// namespace jag::disk::
}
}


// __JAG_DISK_READ_WRITE_H__
#endif
