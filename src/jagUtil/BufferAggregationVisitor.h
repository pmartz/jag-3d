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

#ifndef __JAGUTIL_BUFFER_AGGREGATION_VISITOR_H__
#define __JAGUTIL_BUFFER_AGGREGATION_VISITOR_H__ 1

#include <jagUtil/Export.h>
#include <jagSG/Visitor.h>
#include <jagSG/Node.h>
#include <jagDraw/VertexArrayObject.h>

#include <set>
#include <map>


namespace jagUtil
{


/** \class BufferAggregationVisitor BufferAggregationVisitor.h <jagUtil/BufferAggregationVisitor.h>
\brief Combines vertex array and element array buffers.
\details

\specBegin
\specTableBegin
\specLog{jag.sg.visit.bufagg}
\specTableEnd
\specEnd
*/
class JAGUTIL_EXPORT BufferAggregationVisitor : public jagSG::VisitorBase
{
public:
    BufferAggregationVisitor( jagSG::NodePtr node, const std::string& logName=std::string( "" ) );
    BufferAggregationVisitor( const BufferAggregationVisitor& rhs );
    ~BufferAggregationVisitor();

    void reset();

    virtual void visit( jagSG::Node& node );

    /** \brief Optionally combine element buffer objects.
    \details By default, both array buffers and element index buffers
    are combined. Pass 'false' to this function to disable combining
    element index buffers but still combine array buffers. */
    void setCombineElementBuffers( const bool combine=true )
    {
        _combineElementBuffers = combine;
    }

protected:
    void offsetDrawElements( jagDraw::DrawElementsBase* deBase, const size_t offset );
    void combineElementBuffer( jagDraw::DrawElementsBase* deBase );
    void handleDrawable( jagDraw::DrawablePtr draw, const jagDraw::VertexArrayObject* vaop );

    jagDraw::VertexArrayObjectPtr _vaop;
    jagDraw::BufferObjectPtr _elements;
    bool _combineElementBuffers;

    typedef std::set< jagSG::Node* > NodeSet;
    NodeSet _nodeSet;
    typedef std::set< jagDraw::BufferObject* > BufferObjectSet;
    BufferObjectSet _elementBuffers;

    typedef std::map< jagDraw::VertexArrayObject*, size_t > OffsetMap;
    OffsetMap _offsetMap;
};


// jagUtil
}


// __JAGUTIL_BUFFER_AGGREGATION_VISITOR_H__
#endif
