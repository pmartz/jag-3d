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

#ifndef __JAG_UTIL_QUAD_NODE_H__
#define __JAG_UTIL_QUAD_NODE_H__ 1

#include <jag/util/Export.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/Texture.h>
#include <jag/draw/Framebuffer.h>
#include <jag/draw/CommandMap.h>
#include <jag/draw/Shader.h>
#include <jag/draw/Program.h>
#include <jag/base/ptr.h>
#include <jag/base/LogBase.h>



namespace jag {
namespace util {


/** \class QuadNode QuadNode.h <jag/util/QuadNode.h>
\brief A jag::draw::DrawNode for drawing a textured quad.
\details TBD

\logname jag.util.quadnode
*/
class JAGUTIL_EXPORT QuadNode : public jag::draw::DrawNode
{
public:
    QuadNode( const std::string& logName=std::string( "" ) );
    /** Constructor for one input texture. */
    QuadNode( jag::draw::TexturePtr inputBuffer,
        jag::draw::TexturePtr outputBuffer=jag::draw::TexturePtr(),
        const std::string& logName=std::string( "" ) );
    /** Constructor for multiple input textures. */
    QuadNode( jag::draw::TextureVec& inputBuffers,
        jag::draw::TexturePtr outputBuffer=jag::draw::TexturePtr(),
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
    void setShaders( jag::draw::ShaderPtr& frag,
        jag::draw::ShaderPtr& vert );
    /** \overload */
    void setProgram( jag::draw::ProgramPtr& program );

    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit( jag::draw::ProgramPtr& program );
    std::string getTextureCountString() const;

    jag::draw::TextureVec _inputBuffers;
    jag::draw::TexturePtr _outputBuffer;

    int _width, _height;

    jag::draw::FramebufferPtr _fbo;
    jag::draw::CommandMapPtr _commands;

    unsigned int _numContexts;
};

typedef jag::base::ptr< QuadNode >::shared_ptr QuadNodePtr;


// namespace jag::util::
}
}


// __JAG_UTIL_QUAD_NODE_H__
#endif
