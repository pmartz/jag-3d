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

#ifndef __JAGDRAW_VERTEX_ARRAY_OBJECT_H__
#define __JAGDRAW_VERTEX_ARRAY_OBJECT_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class VertexArrayObject VertexArrayObject.h <jagDraw/VertexArrayObject.h>
\brief A context-safe wrapper for OpenGL vertex array objects.
\details \gl{section 2.10}.
*/
class JAGDRAW_EXPORT VertexArrayObject : public DrawablePrep
{
public:
    VertexArrayObject();
    VertexArrayObject( const VertexArrayObject& rhs );

    virtual ~VertexArrayObject();

    virtual void operator()( DrawInfo& drawInfo );

    GLint getId( const unsigned int contextID );

    /** \brief Tell the VertexArrayObject how many contexts to expect.
    \details Resizes the _ids variable, then iterates over all attached
    BufferObjects, invoking setMaxContexts on each.
    */
    void setMaxContexts( const unsigned int numContexts );

    void addVertexArrayCommand( VertexArrayCommandPtr vacp, const VertexArrayCommand::UsageHint& usage=VertexArrayCommand::Unspecified );
    VertexArrayCommandList& getVertexArrayCommandList();
    const VertexArrayCommandList& getVertexArrayCommandList() const;

protected:
    void internalInit( const unsigned int contextID );

    PerContextIDStatus _ids;

    VertexArrayCommandList _commands;

    /** Stores the buffer object and vertex attribute that contain
    and specify vertex data (tagged with usage "Vertex"). */
    VertexArrayCommandList _vertices;
};

typedef jagBase::ptr< jagDraw::VertexArrayObject >::shared_ptr VertexArrayObjectPtr;
typedef std::vector< VertexArrayObjectPtr > VertexArrayObjectList;


// jagDraw
}


// __JAGDRAW_VERTEX_ARRAY_OBJECT_H__
#endif
