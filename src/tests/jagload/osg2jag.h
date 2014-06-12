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

#ifndef __OSG_2_JAG_H__
#define __OSG_2_JAG_H__ 1

#include <osg/NodeVisitor>
#include <jag/draw/Drawable.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/DrawNodeContainer.h>
#include <jag/base/Buffer.h>

#include <osg/PrimitiveSet>

namespace osg {
    class Geometry;
    class Array;
}


class Osg2Jag : public osg::NodeVisitor
{
public:
    Osg2Jag();
    ~Osg2Jag();

    virtual void apply( osg::Node& node );
    virtual void apply( osg::Geode& node );

    void apply( osg::Geometry* geom );

    jag::draw::DrawNodeContainer& getDrawNodeContainer();


    struct ArrayInfo {
        jag::base::BufferPtr _buffer;
        GLenum _type;
        unsigned int _numElements;
        unsigned int _componentsPerElement;
    };

    ArrayInfo asJagArray( const osg::Array* arrayIn, const osg::Matrix& m );

    ArrayInfo asJagArray( const osg::VectorGLubyte* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLushort* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLuint* arrayIn );

protected:
    jag::draw::DrawNodeContainer _jagDrawNodes;
};


// __OSG_2_JAG_H__
#endif
