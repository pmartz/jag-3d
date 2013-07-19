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
#include <jagDraw/ObjectID.h>
#include <jagDraw/VertexArrayCommand.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;

class VertexArrayObject;
typedef jagBase::ptr< jagDraw::VertexArrayObject >::shared_ptr VertexArrayObjectPtr;

/** \class VertexArrayObject VertexArrayObject.h <jagDraw/VertexArrayObject.h>
\brief A context-safe wrapper for OpenGL vertex array objects.
\details \gl{section 2.10}.
*/
class JAGDRAW_EXPORT VertexArrayObject : public DrawablePrep,
            public ObjectID
{
public:
    VertexArrayObject();
    VertexArrayObject( const VertexArrayObject& rhs );
    virtual ~VertexArrayObject();

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() { return( VertexArrayObjectPtr( new VertexArrayObject( *this ) ) ); }

    /** \brief TBD
    \details TBD */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    \details TBD
    */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    /** \brief Tell the VertexArrayObject how many contexts to expect.
    \details Resizes the _ids variable, then iterates over all attached
    BufferObjects, invoking setMaxContexts on each.
    */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}


    /** \brief TBD
    \details TBD
    */
    typedef enum {
        Unspecified,
        Vertex,
        Normal,
        TexCoord
    } UsageHint;

    /** \brief TBD
    \details TBD */
    void addVertexArrayCommand( VertexArrayCommandPtr vacp, const UsageHint& usage=Unspecified );
    /** \brief TBD
    \details TBD */
    VertexArrayCommandPtr getVertexArrayCommand( const VertexArrayCommand::Type& type, const UsageHint& usage ) const;

    /** \brief TBD
    \details TBD */
    VertexArrayCommandVec& getVertexArrayCommandList();
    /** \brief TBD
    \details TBD */
    const VertexArrayCommandVec& getVertexArrayCommandList() const;


    /** \name VertexArrayObject Aggregation
    \details Aggregating multiple VertexArrayObjects together can
    increase performance by reducing bind calls. See
    jagUtil::BufferAggregationVisitor.
    */
    /*@{*/

    /** \brief Determines if two VAOs are identical in format.
    \details Used by jagUtil::BufferAggregationVisitor. If two VAOs
    are identical in format, they can be aggregated, thus reducing
    draw traversal VAO bind calls. */
    bool isSameKind( const VertexArrayObject& rhs );

    /** \brief Appends VAO \c rhs onto the end of this VAO instance.
    \details Used by jagUtil::BufferAggregationVisitor. */
    VertexArrayObject& combine( const VertexArrayObject& rhs );

    /*@}*/

protected:
    void internalInit( const unsigned int contextID );

    PerContextGLboolean _initialized;

    VertexArrayCommandVec _commands;

    /** Stores the buffer object and vertex attribute that contain
    and specify vertex data (tagged with usage "Vertex"). */
    VertexArrayCommandVec _vertices;
};

typedef std::vector< VertexArrayObjectPtr > VertexArrayObjectVec;


// jagDraw
}


// __JAGDRAW_VERTEX_ARRAY_OBJECT_H__
#endif
