/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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
