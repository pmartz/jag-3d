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