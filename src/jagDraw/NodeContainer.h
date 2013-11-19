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

#ifndef __JAGDRAW_NODE_CONTAINER_H__
#define __JAGDRAW_NODE_CONTAINER_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/Node.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/LogBase.h>
#include <jagBase/MultiCallback.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class NodeContainer NodeContainer.h <jagDraw/NodeContainer.h>
\brief TBD
\details std::vector of draw graph Node objects. */
class JAGDRAW_EXPORT NodeContainer : public jagDraw::DrawNodeVec,
        protected jagBase::LogBase, public ObjectIDOwner
{
public:
    NodeContainer( const std::string& logName=std::string( "" ) );
    NodeContainer( const NodeContainer& rhs );
    ~NodeContainer();

    /** \brief Reset the NodeContainer.
    \details If reset is enabled, clear the vector of draw Nodes.
    Otherwise, do nothing. */
    void reset();
    /** \brief Enable or disable NodeContainer reset.
    \details By default, reset is enabled. */
    void setResetEnable( const bool enable );
    /** \brief Get the current reset state. */
    bool getResetEnable() const;

    NodeContainer& operator=( const NodeContainer& rhs );


    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( NodeContainer& /* nodeContainer */,
            DrawInfo& /* drawInfo */ )
        {
            return( false );
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > CallbackVec;
    /** \brief TBD
    \details TBD */
    CallbackVec& getCallbacks();


    /** \brief If all callbacks pass, execute all Nodes.
    \details If all callbacks return true, call internalExecute().
    Otherwise do nothing and return.
    
    This is an override of the ObjectIDOwner base class. */
    virtual void execute( DrawInfo& drawInfo );
    /** \brief Call execute() on all Nodes.
    \details TBD */
    virtual void internalExecute( DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    CallbackVec _callbacks;

    /** Default value: true. */
    bool _resetEnable;
};

typedef jagBase::ptr< jagDraw::NodeContainer >::shared_ptr NodeContainerPtr;
typedef std::vector< NodeContainerPtr > NodeContainerVec;
typedef std::vector< NodeContainer > NodeContainerSimpleVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_NODE_CONTAINER_H__
#endif
