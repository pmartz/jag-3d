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

#ifndef __OSG_2_JAG_H__
#define __OSG_2_JAG_H__ 1

#include <osg/NodeVisitor>
#include <jagDraw/Drawable.h>
#include <jagDraw/Node.h>
#include <jagDraw/NodeContainer.h>
#include <jagBase/Buffer.h>

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

    jagDraw::NodeContainer& getDrawNodeContainer();


    struct ArrayInfo {
        jagBase::BufferPtr _buffer;
        GLenum _type;
        unsigned int _numElements;
        unsigned int _componentsPerElement;
    };

    ArrayInfo asJagArray( const osg::Array* arrayIn, const osg::Matrix& m );

    ArrayInfo asJagArray( const osg::VectorGLubyte* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLushort* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLuint* arrayIn );

protected:
    jagDraw::NodeContainer _jagDrawNodes;
};


// __OSG_2_JAG_H__
#endif
