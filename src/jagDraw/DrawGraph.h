/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

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


// jagDraw
}


// __JAGDRAW_DRAW_GRAPH_H__
#endif
