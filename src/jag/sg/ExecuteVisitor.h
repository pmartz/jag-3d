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

#ifndef __JAG_SG_EXECUTE_VISITOR_H__
#define __JAG_SG_EXECUTE_VISITOR_H__ 1

#include <jag/sg/Export.h>
#include <jag/sg/Visitor.h>
#include <jag/base/Transform.h>
#include <jag/draw/Uniform.h>
#include <jag/base/ptr.h>


namespace jag {
namespace sg {


/** \class ExecuteVisitor ExecuteVisitor.h <jag/sg/ExecuteVisitor.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT ExecuteVisitor : public Visitor
{
public:
    ExecuteVisitor( jag::draw::DrawInfo& drawInfo );
    ExecuteVisitor( jag::draw::DrawInfo& drawInfo, jag::sg::Node& node );
    ExecuteVisitor( const ExecuteVisitor& rhs );
    virtual ~ExecuteVisitor();


    /** \brief TBD
    \details TBD */
    void setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

    /** \brief TBD
    \details TBD */
    void reset();

    /** \brief TBD
    \details TBD */
    virtual void visit( jag::sg::Node& node );

protected:
    void updateTransformUniforms();

    jag::draw::DrawInfo& _drawInfo;
    jag::base::TransformD _transform;
};

typedef jag::base::ptr< jag::sg::ExecuteVisitor >::shared_ptr ExecuteVisitorPtr;


// namespace jag::sg::
}
}


// __JAG_SG_EXECUTE_VISITOR_H__
#endif
