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

#ifndef __ASSIMP_2_JAG_H__
#define __ASSIMP_2_JAG_H__ 1


#include <jag/base/Config.h>
#include <jag/disk/ReaderWriter.h>
#include <jag/disk/Options.h>
#include <jag/sg/Node.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/VertexArrayObject.h>
#include <jag/draw/Texture.h>
#include <jag/draw/CommandMap.h>

#include <assimp/scene.h>

#include <vector>
#include <string>


class Assimp2Jag
{
    std::string _logName;

public:
    Assimp2Jag( const aiScene* aiScene, const jag::disk::Options* options );
    ~Assimp2Jag();

    jag::sg::NodePtr getJagScene();

protected:
    struct ArrayInfo {
        jag::base::BufferPtr _buffer;
        GLenum _type;
        unsigned int _numElements;
        unsigned int _componentsPerElement;
    };

    void initData();
    jag::sg::NodePtr traverse( const aiNode* aiNode );

    ArrayInfo get3fData( const aiVector3D* data, const unsigned int size ) const;
    ArrayInfo getUIntFaceData( const struct aiMesh* mesh ) const;
    static gmtl::Matrix44d asGMTLMatrix( aiMatrix4x4 m );
    static jag::draw::TexturePtr loadTexture( const std::string& fileName );


    const aiScene& _aiScene;

    jag::sg::NodePtr _jagScene;
    jag::sg::NodePtr _current;

    jag::draw::VertexArrayObjectVec _vao;
    jag::draw::DrawableVec _draw;

    struct TexInfo {
        TexInfo( const GLuint index, jag::draw::TexturePtr& tex )
          : _index( index ),
            _tex( tex )
        {}
        GLuint _index;
        jag::draw::TexturePtr _tex;
    };
    typedef std::vector< TexInfo > TexInfoVec;
    typedef std::vector< TexInfoVec > TexInfoVecs;
    TexInfoVecs _texInfo;

    std::string _vertexAttribName;
    std::string _normalAttribName;
    std::string _texCoordAttribName;

    jag::disk::OptionsPtr _options;
};


// __ASSIMP_2_JAG_H__
#endif
