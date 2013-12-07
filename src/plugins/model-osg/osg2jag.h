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


#include <jag/disk/Options.h>
#include <jag/base/Buffer.h>
#include <jagSG/Node.h>
#include <jag/draw/UniformBlock.h>

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
    Osg2Jag( const jag::disk::Options* options );
    ~Osg2Jag();

    virtual void apply( osg::Node& osgNode );
    virtual void apply( osg::MatrixTransform& osgNode );

    virtual void apply( osg::Geode& osgNode );

    jagSG::NodePtr getJagScene();


protected:
    void apply( osg::Geometry* geom, const bool storeInParent=false );
    void apply( osg::StateSet* stateSet );

    bool preTraverse( osg::Object* osgObject, osg::StateSet* stateSet, const gmtl::Matrix44d& m=gmtl::MAT_IDENTITY44D );
    void postTraverse();

    jag::draw::UniformBlockPtr findMaterial( osg::Material* m );
    void addMaterial( osg::Material* m, jag::draw::UniformBlockPtr ub );


    jagSG::NodePtr _jagScene;
    jagSG::NodePtr _current;
    jagSG::NodeVec _nodeStack; // Required to support multiparenting.

    typedef std::map< osg::Object*, jagSG::NodePtr > OSGObjectMap;
    OSGObjectMap _objInstances;

    typedef std::map< osg::StateSet*, jag::draw::CommandMapPtr > OSGStateSetMap;
    OSGStateSetMap _ssInstances;

    typedef std::map< osg::Material*, jag::draw::UniformBlockPtr > OSGMaterialMap;
    OSGMaterialMap _matInstances;


    struct ArrayInfo {
        jag::base::BufferPtr _buffer;
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
