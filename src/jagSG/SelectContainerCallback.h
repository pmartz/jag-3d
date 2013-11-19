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

#ifndef __JAGSG_SELECT_CONTAINER_CALLBACK_H__
#define __JAGSG_SELECT_CONTAINER_CALLBACK_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>



namespace jagSG {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class SelectContainerCallback SelectContainerCallback.h <jagSG/SelectContainerCallback.h>
\brief Change the draw graph NodeContainer that the CollectionVisitor stores Drawables in.
\details TBD
*/
class JAGSG_EXPORT SelectContainerCallback : protected jagBase::LogBase,
        public jagSG::Node::Callback
{
public:
    SelectContainerCallback( const std::string& logName=std::string( "" ) );
    SelectContainerCallback( const unsigned int index, const std::string& logName=std::string( "" ) );
    SelectContainerCallback( const SelectContainerCallback& rhs );
    ~SelectContainerCallback();

    void setContainer( const unsigned int index );
    unsigned int getContainer() const;

    /** \brief TBD
    \details TBD */
    virtual bool operator()( jagSG::VisitorBase* visitor, jagSG::Node::CallbackInfo* info );

protected:
    unsigned int _index;
};

typedef jagBase::ptr< SelectContainerCallback >::shared_ptr SelectContainerCallbackPtr;


/*@}*/


// jagSG
}


// __JAGSG_SELECT_CONTAINER_CALLBACK_H__
#endif
