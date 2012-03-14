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

#ifndef __JAGDRAW_DRAWABLE_H__
#define __JAGDRAW_DRAWABLE_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/DrawCommand.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class Drawable Drawable.h <jagDraw/Drawable.h>
\brief
\details
*/
class JAGDRAW_EXPORT Drawable
{
public:
    Drawable();
    ~Drawable();
    

    /** \brief TBD
    \details TBD
    */
    virtual void operator()( DrawInfo& drawInfo );


    /** \brief Add a vertex array command, such as a buffer object, vertex array object, or vertex attribute.
    \details All vertex array commands are executed by the Drawale in order, and prior to
    issuing any of the draw commands (see addDrawCommand()). */
    void addVertexArrayCommand( VertexArrayCommandPtr vacp, const VertexArrayCommand::UsageHint& usageHint=VertexArrayCommand::Unspecified );

    /** \brief TBD
    \details TBD
    */
    VertexArrayCommandList& getVertexArrayCommandList();
    /** \overload */
    const VertexArrayCommandList& getVertexArrayCommandList() const;

    /** \brief TBD
    \details TBD
    */
    void addDrawCommand( DrawCommandPtr dcp );

    /** \brief TBD
    \details TBD
    */
    DrawCommandList& getDrawCommandList();
    /** \overload */
    const DrawCommandList& getDrawCommandList() const;

protected:
    // TBD state(?) commands
    VertexArrayCommandList _vertexArrayCommands;
    DrawCommandList _drawCommands;

    /** Stores the vertex array object, buffer object, and vertex attribute
    that contain and specify vertex data (tagged with usage "Vertex"). */
    VertexArrayCommandList _vertices;
};

typedef jagBase::ptr< jagDraw::Drawable >::shared_ptr DrawablePtr;
typedef std::vector< DrawablePtr > DrawableList;


// jagDraw
}


// __JAGDRAW_DRAWABLE_H__
#endif
