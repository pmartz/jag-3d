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

#ifndef __JAGUTIL_RESET_BOUNDS_VISITOR_H__
#define __JAGUTIL_RESET_BOUNDS_VISITOR_H__ 1

#include <jagUtil/Export.h>
#include <jagSG/Visitor.h>
#include <jagSG/Node.h>


namespace jagUtil
{


/** \class BufferAggregationVisitor BufferAggregationVisitor.h <jagUtil/BufferAggregationVisitor.h>
\brief Combines vertex array and element array buffers.
\details
*/
class JAGUTIL_EXPORT ResetBoundsVisitor : public jagSG::VisitorBase
{
public:
    ResetBoundsVisitor( jagSG::NodePtr node, const std::string& logName=std::string( "" ) );
    ResetBoundsVisitor( const ResetBoundsVisitor& rhs );
    ~ResetBoundsVisitor();

    virtual void visit( jagSG::Node& node );

protected:
};


// jagUtil
}


// __JAGUTIL_RESET_BOUNDS_VISITOR_H__
#endif
