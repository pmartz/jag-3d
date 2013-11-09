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

#include <jagUtil/ResetBoundsVisitor.h>
#include <jagDraw/Drawable.h>
#include <jagSG/Node.h>

#include <boost/foreach.hpp>



namespace jagUtil
{


ResetBoundsVisitor::ResetBoundsVisitor( jagSG::NodePtr node, const std::string& logName )
    : jagSG::VisitorBase( "resetbnds", logName )
{
    node->accept( *this );
}
ResetBoundsVisitor::ResetBoundsVisitor( const ResetBoundsVisitor& rhs )
    : jagSG::VisitorBase( rhs )
{
}
ResetBoundsVisitor::~ResetBoundsVisitor()
{
}


void ResetBoundsVisitor::visit( jagSG::Node& node )
{
    node.setBoundDirtyFlag();

    for( unsigned int idx = 0; idx < node.getNumDrawables(); ++idx )
    {
        node.getDrawable( idx )->setBoundDirtyFlag();
    }

    node.traverse( *this );
}


// jagUtil
}
