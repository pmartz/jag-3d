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

#ifndef __JAG_BASE_PTR_H__
#define __JAG_BASE_PTR_H__ 1


#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>


namespace jag {
namespace base {


#define SHARED_FROM_THIS(_t) boost::enable_shared_from_this<_t>

    
/** \class ptr ptr.h <jag/base/ptr.h>
\brief Defines the \c ptr template struct, supporting either Boost or TR1.
*/
template <typename T>
struct ptr
{
    typedef boost::shared_ptr< T > shared_ptr;
    typedef boost::shared_array< T > shared_array_ptr;
};


// namespace jag::base::
}
}


// __JAG_BASE_PTR_H__
#endif
