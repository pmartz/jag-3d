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
    void removeVertexArrayCommand( const VertexArrayCommandPtr vacp );
    /** \brief TBD
    \details TBD */
    void removeVertexArrayCommand( const std::string& userDataName );
    /** \brief TBD
    \details TBD */
    VertexArrayCommandPtr getVertexArrayCommand( const VertexArrayCommand::Type& type, const UsageHint& usage ) const;

    /** \brief TBD
    \details TBD */
    VertexArrayCommandVec& getVertexArrayCommandList();
    /** \brief TBD
    \details TBD */
    const VertexArrayCommandVec& getVertexArrayCommandList() const;

    /** \brief TBD
    \details TBD */
    void markAllDirty( const bool dirty=true );
    /** \brief TBD
    \details TBD */
    bool isDirty( const unsigned int contextID ) const;


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

    PerContextBool _dirty;

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
