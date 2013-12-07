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

#include <jagUtil/ResetBoundsVisitor.h>
#include <jag/draw/Drawable.h>
#include <jag/sg/Node.h>

#include <boost/foreach.hpp>



namespace jagUtil
{


ResetBoundsVisitor::ResetBoundsVisitor( jag::sg::NodePtr node, const std::string& logName )
    : jag::sg::VisitorBase( "resetbnds", logName )
{
    node->accept( *this );
}
ResetBoundsVisitor::ResetBoundsVisitor( const ResetBoundsVisitor& rhs )
    : jag::sg::VisitorBase( rhs )
{
}
ResetBoundsVisitor::~ResetBoundsVisitor()
{
}


void ResetBoundsVisitor::visit( jag::sg::Node& node )
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
