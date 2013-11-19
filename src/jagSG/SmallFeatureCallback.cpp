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

#include <jagSG/SmallFeatureCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>
#include <jagBase/Profile.h>


namespace jagSG {


SmallFeatureCallback::SmallFeatureCallback( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.small" : logName ),
    jagSG::Node::Callback(),
    _threshold( 3. )
{
}
SmallFeatureCallback::SmallFeatureCallback( const SmallFeatureCallback& rhs )
  : jagBase::LogBase( "jag.sg.coll.small" ),
    jagSG::Node::Callback( rhs ),
    _threshold( rhs._threshold )
{
}
SmallFeatureCallback::~SmallFeatureCallback()
{
}


bool SmallFeatureCallback::operator()( jagSG::VisitorBase* /* visitor */, jagSG::Node::CallbackInfo* info )
{
    JAG3D_PROFILE( "small feature" );

    jagSG::CollectionVisitor::CollectionInfo* ci( static_cast<
        jagSG::CollectionVisitor::CollectionInfo* >( info ) );

    // Continue collection if window coord bound radius is >= the threshold.
    return( ci->getWinCLength( ci->getECBoundRadius() ) >= _threshold );
}

void SmallFeatureCallback::setThreshold( const double threshold )
{
    _threshold = threshold;
}
double SmallFeatureCallback::getThreshold() const
{
    return( _threshold );
}





SmallFeatureDistributionVisitor::SmallFeatureDistributionVisitor()
    : jagSG::VisitorBase( "small" ),
      _cb( SmallFeatureCallbackPtr( new SmallFeatureCallback() ) )
{
}
SmallFeatureDistributionVisitor::SmallFeatureDistributionVisitor( const SmallFeatureDistributionVisitor& rhs )
    : jagSG::VisitorBase( rhs ),
      _cb( rhs._cb )
{
}
SmallFeatureDistributionVisitor::~SmallFeatureDistributionVisitor()
{
}

void SmallFeatureDistributionVisitor::visit( jagSG::Node& node )
{
    node.getCollectionCallbacks().push_back( _cb );

    node.traverse( *this );
}


// jagSG
}
