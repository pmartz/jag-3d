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

#include <boost/any.hpp>
#include <string>


namespace jag {
namespace disk {


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
