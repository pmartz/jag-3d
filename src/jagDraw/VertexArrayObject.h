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


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

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
    VertexArrayObject& operator=( const VertexArrayObject& rhs );

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const { return( VertexArrayObjectPtr( new VertexArrayObject( *this ) ) ); }

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
    bool isSameKind( const VertexArrayObject& rhs ) const;

    /** \brief Appends VAO \c rhs onto the end of this VAO instance.
    \details Used by jagUtil::BufferAggregationVisitor.
    \return The previous size of the contained buffer object(s). */
    size_t combine( const VertexArrayObject& rhs );

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


/*@}*/


// jagDraw
}


// __JAGDRAW_VERTEX_ARRAY_OBJECT_H__
#endif
