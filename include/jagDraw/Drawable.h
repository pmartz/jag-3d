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
#include <jagBase/LogBase.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/DrawCommand.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class Drawable Drawable.h <jagDraw/Drawable.h>
\brief A grouping of objects for rendering geometry.
\details Drawable stores three separate lists of jag objects:
\li DrawablePrep objects. These are objects like ShaderProgram, Uniform, Texture, etc.
\li VertexArrayCommand objects. These are things like BufferObject, VertexAttrib, and VertexArrayObject.
\li DrawCommand objects. These are commands for rendering, such as glDrawArrays, glDrawElements, and others.

Drawable::operator()( DrawInfo& ) executes the three groups of commands in the order
above, and each command within a given group is executed sequentially.



Note that when adding a BufferObject to the VertexArrayCommands list, the design
intent is that the BufferObject binding be set to GL_ARRAY_BUFFER, but this is not
a strict requirement.

jagDraw module client code (such as an application or loader plugin) creates a
Drawable and adds any required commands. Commands within a given set should be
added in their desired execution order. For example, if the app wants a VertexAttrib
to source its data from a specific VufferObject, the app adds the BufferObject
first, then the VertexAttrib, as follows:
\code
    drawable->addVertexArrayCommand( bufferObj );
    drawable->addVertexArrayCommand( vertexAttrib );
\endcode
At draw time, Drawable::operator()( DrawInfo& ) would bind the buffer object,
then enable the vertex attrib and issue the glVertexAttribPointer() command.

All three command lists can be accessed and modified directly with
getDrawablePrepList(), getVertexArrayCommandList(), and getDrawCommandList().

Note that getVertexArrayCommandList() takes an optional second parameter
to specify a VertexArrayCommand::UsageHint. VertexArrayCommand objects with
UsageHint Vertex are used by getBound() to compute the bounding volume. Other
usage hints are reserved for future use.
*/
class JAGDRAW_EXPORT Drawable : protected jagBase::LogBase
{
public:
    Drawable();
    ~Drawable();
    

    /** \brief TBD
    \details TBD
    */
    virtual void operator()( DrawInfo& drawInfo );


    /** \brief Compute (if necessary) and return the bounding volume.
    \details This function is not yet implemented.
    */
    void getBound();


    /** \brief Tell the Drawable how many contexts to expect.
    \details Drawable iterates over all other attached Jag objects and
    passes \c numContexts to their setMaxContexts() member function.
    */
    void setMaxContexts( const unsigned int numContexts );


    /** \brief Add a drawable prep command, such as a uniform or a shader program.
    \details All drawable prep commands are executed by the Drawale in order, and prior to
    issuing any of the vertex array commands (see addVertexArrayCommand()). */
    void addDrawablePrep( DrawablePrepPtr dpp );

    /** \brief TBD
    \details TBD
    */
    DrawablePrepList& getDrawablePrepList();
    /** \overload */
    const DrawablePrepList& getDrawablePrepList() const;


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
    DrawablePrepList _drawablePrep;
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
