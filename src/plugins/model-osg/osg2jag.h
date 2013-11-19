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


#include <jagDisk/Options.h>
#include <jagBase/Buffer.h>
#include <jagSG/Node.h>
#include <jagDraw/UniformBlock.h>

#include <osg/NodeVisitor>
#include <osg/Matrix>
#include <osg/PrimitiveSet>

#include <map>


namespace osg {
    class Node;
    class MatrixTransform;
    class Geometry;
    class Array;
    class StateSet;
    class Material;
}


class Osg2Jag : public osg::NodeVisitor
{
public:
    Osg2Jag( const jagDisk::Options* options );
    ~Osg2Jag();

    virtual void apply( osg::Node& osgNode );
    virtual void apply( osg::MatrixTransform& osgNode );

    virtual void apply( osg::Geode& osgNode );

    jagSG::NodePtr getJagScene();


protected:
    void apply( osg::Geometry* geom );
    void apply( osg::StateSet* stateSet );

    bool preTraverse( osg::Object* osgObject, osg::StateSet* stateSet, const gmtl::Matrix44d& m=gmtl::MAT_IDENTITY44D );
    void postTraverse();

    jagDraw::UniformBlockPtr findMaterial( osg::Material* m );
    void addMaterial( osg::Material* m, jagDraw::UniformBlockPtr ub );


    jagSG::NodePtr _jagScene;
    jagSG::NodePtr _current;
    jagSG::NodeVec _nodeStack; // Required to support multiparenting.

    typedef std::map< osg::Object*, jagSG::NodePtr > OSGObjectMap;
    OSGObjectMap _objInstances;

    typedef std::map< osg::StateSet*, jagDraw::CommandMapPtr > OSGStateSetMap;
    OSGStateSetMap _ssInstances;

    typedef std::map< osg::Material*, jagDraw::UniformBlockPtr > OSGMaterialMap;
    OSGMaterialMap _matInstances;


    struct ArrayInfo {
        jagBase::BufferPtr _buffer;
        GLenum _type;
        unsigned int _numElements;
        unsigned int _componentsPerElement;
    };

    ArrayInfo asJagArray( const osg::Array* arrayIn );

    ArrayInfo asJagArray( const osg::VectorGLubyte* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLushort* arrayIn );
    ArrayInfo asJagArray( const osg::VectorGLuint* arrayIn );

    static gmtl::Matrix44d asGmtlMatrix( const osg::Matrixd& m );
    static gmtl::Matrix44d asGmtlMatrix( const osg::Matrixf& m );

    std::string _vertexAttribName;
    std::string _normalAttribName;
    std::string _texCoordAttribName;
};


// __OSG_2_JAG_H__
#endif
