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

#ifndef __OSG_2_JAG_H__
#define __OSG_2_JAG_H__ 1

#include <osg/NodeVisitor>
#include <osg/Matrix>
#include <jagBase/Buffer.h>
#include <jagSG/Node.h>
#include <osg/PrimitiveSet>

#include <map>


namespace osg {
    class Node;
    class MatrixTransform;
    class Geometry;
    class Array;
    class StateSet;
}


class Osg2Jag : public osg::NodeVisitor
{
public:
    Osg2Jag();
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


    jagSG::NodePtr _jagScene;
    jagSG::NodePtr _current;
    jagSG::NodeVec _nodeStack; // Required to support multiparenting.

    typedef std::map< osg::Object*, jagSG::NodePtr > OSGObjectMap;
    OSGObjectMap _objInstances;

    typedef std::map< osg::StateSet*, jagDraw::CommandMapPtr > OSGStateSetMap;
    OSGStateSetMap _ssInstances;


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
};


// __OSG_2_JAG_H__
#endif
