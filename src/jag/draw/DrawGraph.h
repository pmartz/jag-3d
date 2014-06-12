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

#ifndef __JAG_DRAW_DRAW_GRAPH_H__
#define __JAG_DRAW_DRAW_GRAPH_H__ 1

#include <jag/draw/Export.h>
#include <jag/draw/DrawNodeContainer.h>
#include <jag/draw/ObjectID.h>
#include <jag/draw/TransformCallback.h>
#include <jag/base/LogBase.h>
#include <jag/base/MultiCallback.h>
#include <jag/base/ptr.h>

#include <vector>


namespace jag {
namespace draw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class DrawGraph DrawGraph.h <jag/draw/DrawGraph.h>
\brief TBD
\details std::vector of draw graph Node objects. */
class JAGDRAW_EXPORT DrawGraph : public jag::draw::DrawNodeContainerSimpleVec,
        protected jag::base::LogBase, public ObjectIDOwner
{
public:
    DrawGraph( const std::string& logName=std::string( "" ) );
    DrawGraph( const DrawGraph& rhs );
    ~DrawGraph();

    DrawGraph& operator=( const DrawGraph& rhs );


    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( DrawGraph& /* drawGraph */,
            DrawInfo& /* drawInfo */ )
        {
            return( false );
        }
    };
    typedef jag::base::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    typedef jag::base::MultiCallback< CallbackPtr > CallbackVec;
    /** \brief TBD
    \details TBD */
    CallbackVec& getCallbacks();


    /** \brief TBD
    \details TBD */
    void setTransformCallback( TransformCallbackPtr transformCallback );
    /** \brief TBD
    \details TBD */
    TransformCallbackPtr getTransformCallback();
    /** \brief TBD
    \details TBD */
    void setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );


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
    virtual void deleteID( const jag::draw::jagDrawContextID contextID );

protected:
    CallbackVec _callbacks;

    TransformCallbackPtr _transformCallback;
};

typedef jag::base::ptr< jag::draw::DrawGraph >::shared_ptr DrawGraphPtr;
typedef std::vector< DrawGraphPtr > DrawGraphVec;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_DRAW_GRAPH_H__
#endif
