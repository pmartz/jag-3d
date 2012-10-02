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
#include <jagDraw/ObjectID.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/DrawCommand.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class Drawable Drawable.h <jagDraw/Drawable.h>
\brief A grouping of objects for rendering geometry.
\details Drawable stores two separate lists of jag objects:
\li DrawablePrep objects. These are objects like Program, Uniform, Texture, and VertexArrayObjects.
\li DrawCommand objects. These are commands for rendering, such as glDrawArrays, glDrawElements, and others.

Drawable::operator()( DrawInfo& ) executes the two groups of commands in the order
above, and each command within a given group is executed sequentially.

Note that BufferObject can't be added directly to a Drawable. In a typical use case, buffer objects
bound to GL_ARRAY_BUFFER are attached to a VertexArrayObject, and buffer objects bound to GL_ELEMENT_BUFFER
are attached to DrawElements commands.

jagDraw module client code (such as an application or loader plugin) creates a
Drawable and adds any required commands.

Both command lists can be accessed and modified directly with
getDrawablePrepList() and getDrawCommandList().
*/
class JAGDRAW_EXPORT Drawable : protected jagBase::LogBase, public ObjectIDOwner
{
public:
    Drawable();
    Drawable( const Drawable& rhs );
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
    \details Drawable iterates over all other attached Jag3D objects and
    passes \c numContexts to their setMaxContexts() member function.
    */
    virtual void setMaxContexts( const unsigned int numContexts );


    /** \brief Add a drawable prep command, such as a uniform or a shader program.
    \details All drawable prep commands are executed by the Drawale in order, and prior to
    issuing any of the vertex array commands (see addVertexArrayCommand()). */
    void addDrawablePrep( DrawablePrepPtr dpp );
    void insertDrawablePrep( DrawablePrepPtr dpp, unsigned int pos=0 );

    /** \brief TBD
    \details TBD
    */
    DrawablePrepVec& getDrawablePrepList();
    /** \overload */
    const DrawablePrepVec& getDrawablePrepList() const;


    /** \brief TBD
    \details TBD
    */
    void addDrawCommand( DrawCommandPtr dcp );

    /** \brief TBD
    \details TBD
    */
    DrawCommandVec& getDrawCommandList();
    /** \overload */
    const DrawCommandVec& getDrawCommandList() const;

protected:
    DrawablePrepVec _drawablePrep;
    DrawCommandVec _drawCommands;
};

typedef jagBase::ptr< jagDraw::Drawable >::shared_ptr DrawablePtr;
typedef std::vector< DrawablePtr > DrawableVec;


// jagDraw
}


// __JAGDRAW_DRAWABLE_H__
#endif
