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

#ifndef __JAG_UTIL_BUFFER_AGGREGATION_VISITOR_H__
#define __JAG_UTIL_BUFFER_AGGREGATION_VISITOR_H__ 1

#include <jag/util/Export.h>
#include <jag/sg/Visitor.h>
#include <jag/sg/Node.h>
#include <jag/draw/VertexArrayObject.h>

#include <set>
#include <map>


namespace jag {
namespace util {


/** \class BufferAggregationVisitor BufferAggregationVisitor.h <jag/util/BufferAggregationVisitor.h>
\brief Combines vertex array and element array buffers.
\details

\specBegin
\specTableBegin
\specLog{jag.sg.visit.bufagg}
\specTableEnd
\specEnd
*/
class JAGUTIL_EXPORT BufferAggregationVisitor : public jag::sg::VisitorBase
{
public:
    BufferAggregationVisitor( jag::sg::NodePtr node, const std::string& logName=std::string( "" ) );
    BufferAggregationVisitor( const BufferAggregationVisitor& rhs );
    ~BufferAggregationVisitor();

    void reset();

    virtual void visit( jag::sg::Node& node );

    /** \brief Optionally combine element buffer objects.
    \details By default, both array buffers and element index buffers
    are combined. Pass 'false' to this function to disable combining
    element index buffers but still combine array buffers. */
    void setCombineElementBuffers( const bool combine=true )
    {
        _combineElementBuffers = combine;
    }

protected:
    void offsetDrawElements( jag::draw::DrawElementsBase* deBase, const size_t offset );
    void combineElementBuffer( jag::draw::DrawElementsBase* deBase );
    void handleDrawable( jag::draw::DrawablePtr draw, const jag::draw::VertexArrayObject* vaop );

    jag::draw::VertexArrayObjectPtr _vaop;
    jag::draw::BufferObjectPtr _elements;
    bool _combineElementBuffers;

    typedef std::set< jag::sg::Node* > NodeSet;
    NodeSet _nodeSet;
    typedef std::set< jag::draw::BufferObject* > BufferObjectSet;
    BufferObjectSet _elementBuffers;

    typedef std::map< jag::draw::VertexArrayObject*, size_t > OffsetMap;
    OffsetMap _offsetMap;
};


// namespace jag::util::
}
}


// __JAG_UTIL_BUFFER_AGGREGATION_VISITOR_H__
#endif
