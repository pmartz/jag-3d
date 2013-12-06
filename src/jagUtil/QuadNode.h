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

#ifndef __JAGUTIL_QUAD_NODE_H__
#define __JAGUTIL_QUAD_NODE_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/DrawNode.h>
#include <jagDraw/Texture.h>
#include <jagDraw/Framebuffer.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Shader.h>
#include <jagDraw/Program.h>
#include <jagBase/ptr.h>
#include <jagBase/LogBase.h>



namespace jagUtil
{


/** \class QuadNode QuadNode.h <jagUtil/QuadNode.h>
\brief A jagDraw::DrawNode for drawing a textured quad.
\details TBD

\logname jag.util.quadnode
*/
class JAGUTIL_EXPORT QuadNode : public jagDraw::DrawNode
{
public:
    QuadNode( const std::string& logName=std::string( "" ) );
    /** Constructor for one input texture. */
    QuadNode( jagDraw::TexturePtr inputBuffer,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    /** Constructor for multiple input textures. */
    QuadNode( jagDraw::TextureVec& inputBuffers,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    QuadNode( const QuadNode& rhs );
    ~QuadNode();

    /** \brief Explicit rendering control.
    \details By default, QuadNode uses its own shaders, which adds samples from
    up to four textures to create the final color value. To produce a different final
    color value, the app should supply its own shaders or program.

    Note that the addShaders() methods are designed so that apps can use a default
    vertex shader and specify only the fragment shader. In this case, data/quadNode.vert
    is used as the vertex shader, which writes texture coord outputs as follow:
    \code
    out vec2 tc;
    \endcode
    Application fragment shader code should declare:
    \code
    in vec2 tc;
    \endcode
    to access these values.

    Furthermore, QuadNode will insert a CPP definition into fragment shader source
    as follows:
    \code
    #define TEXTURE COUNT <n>
    \endcode
    where \c <n> is the number of input textures.

    Finally, texture samplers should be defined in custom fragment shader source as follows:
    \code
    uniform sampler2D texture0;
    uniform sampler2D texture1;
    \\ etc.
    \endcode
    */
    void setShaders( const std::string& fragName, const std::string& vertName="" );
    /** \overload */
    void setShaders( jagDraw::ShaderPtr& frag,
        jagDraw::ShaderPtr& vert=jagDraw::ShaderPtr((jagDraw::Shader*)NULL) );
    /** \overload */
    void setProgram( jagDraw::ProgramPtr& program );

    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit( jagDraw::ProgramPtr& program=jagDraw::ProgramPtr((jagDraw::Program*)NULL) );
    std::string getTextureCountString() const;

    jagDraw::TextureVec _inputBuffers;
    jagDraw::TexturePtr _outputBuffer;

    int _width, _height;

    jagDraw::FramebufferPtr _fbo;
    jagDraw::CommandMapPtr _commands;

    unsigned int _numContexts;
};

typedef jagBase::ptr< QuadNode >::shared_ptr QuadNodePtr;


// jagUtil
}


// __JAGUTIL_QUAD_NODE_H__
#endif
