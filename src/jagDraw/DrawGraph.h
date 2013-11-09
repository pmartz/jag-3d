/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGDRAW_DRAW_GRAPH_H__
#define __JAGDRAW_DRAW_GRAPH_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/TransformCallback.h>
#include <jagBase/LogBase.h>
#include <jagBase/MultiCallback.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class DrawGraph DrawGraph.h <jagDraw/DrawGraph.h>
\brief TBD
\details std::vector of draw graph Node objects. */
class JAGDRAW_EXPORT DrawGraph : public jagDraw::NodeContainerSimpleVec,
        protected jagBase::LogBase, public ObjectIDOwner
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
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > CallbackVec;
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
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    CallbackVec _callbacks;

    TransformCallbackPtr _transformCallback;
};

typedef jagBase::ptr< jagDraw::DrawGraph >::shared_ptr DrawGraphPtr;
typedef std::vector< DrawGraphPtr > DrawGraphVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_DRAW_GRAPH_H__
#endif
