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

#ifndef __JAG_DRAW_COMMAND_MAP_PTR_H__
#define __JAG_DRAW_COMMAND_MAP_PTR_H__ 1

#include <jag/base/ptr.h>
#include <vector>
#include <map>


namespace jag {
namespace draw {


/** \addtogroup jagDrawDataStore Geometry and OpenGL Command Data Storage */
/*@{*/


class CommandMap;
typedef jag::base::ptr< jag::draw::CommandMap >::shared_ptr CommandMapPtr;
typedef std::vector< jag::draw::CommandMapPtr > CommandMapVec;
typedef std::vector< jag::draw::CommandMap > CommandMapSimpleVec;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_COMMAND_MAP_PTR_H__
#endif
