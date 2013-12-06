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

#ifndef __JAGUTIL_A_BUFFER_H__
#define __JAGUTIL_A_BUFFER_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/DrawGraph.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Framebuffer.h>
#include <jagDraw/Texture.h>
#include <jagDraw/Program.h>
#include <jagSG/SelectContainerCallback.h>
#include <jagDraw/PerContextData.h>
#include <jag/base/ptr.h>
#include <jag/base/LogBase.h>


// Forwaed
namespace jagSG {
    class CollectionVisitor;
}


namespace jagUtil
{


// Forwaed
struct ABufferContext;
typedef jagDraw::PerContextData< ABufferContext > PerContextABufferCntxt;


/** \class ABuffer ABuffer.h <jagUtil/ABuffer.h>
\brief Support for a-buffer order independent transparency.
\details This is a port of the OpenGL a-buffer sample code available from
http://blog.icare3d.org/2010/07/opengl-40-abuffer-v20-linked-lists-of.html.
The code has been ported to work with the JAG scene graph and draw graph
data structures. It supports opaque rendering and blending with a secondary
color buffer.

\logname jag.util.abuf
*/
class JAGUTIL_EXPORT ABuffer : public jag::base::LogBase
{
public:
    ABuffer( const std::string& logName=std::string( "" ) );
    ABuffer( jagDraw::TexturePtr& depthBuffer, jagDraw::TexturePtr& colorBuffer0,
        jagDraw::TexturePtr& colorBuffer1=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    ABuffer( const ABuffer& rhs );
    ~ABuffer();

    void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    \details TBD
    \param startContainer Index of the DrawGraph's NodeContainer for ABuffer rendering.
    NodeContainers numbers \c startContainer through \c startContainer + 2 are reserved
    for use by the ABuffer class. */
    jagDraw::DrawGraphPtr& createDrawGraphTemplate( const unsigned int startContainer=1 );


    /** \name Transparency Control Support
    \details Methods for controlling which parts of the scene graph are
    rendered using the ABuffer transparency algorithm, and which are
    rendered by normal (opaque) means. */
    /**@{*/

    /** \brief Convenience utility for toggling ABuffer transparancy at the jagSG::Node level.
    \details If \c enable is true, this method saves the Node's existing CommandMap (of any)
    as user data, replaces the CommandMap with one required for ABuffer rendering, and adds a
    jagSG::SelectContainerCallback to route collected Drawables into the ABuffer's render
    NodeContainer. If \c enable is false, \c node is restored to its original state. */
    void setTransparencyEnable( jagSG::Node& node, const bool enable=true );
    /** \brief Convenience utility to toggle transparency at the jagSG::Node level.
    \details Determines current transparency state by checking for presense or absence
    of our NodeContainer selection callback, then calls setTransparencyEnable() to
    invert the current state. */
    void toggleTransparencyEnable( jagSG::Node& node );
    /** \brief Set a CommandMap to use for toggling to non-transparent.
    \details When setting a Node to be opaque (via setTransparencyEnagle(node,false)
    or by toggleTransparencyEnable(), if the Node's UserData does not contain a
    saved CommandMap, use the specified \c commands. \c commands should contain
    at least a jagDraw::Program and a jagDraw::Framebuffer. */
    void setOpaqueControls( const jagDraw::CommandMapPtr& commands, const unsigned int opaqueContainer=0 );
    /** \brief Get the startiing NodeContainer index.
    \details Use the createDrawGraphTemplate() method to set the start container.
    Default is 1, leaving container 0 for the application.
    ABuffer uses three containers, so the app may also use containers with
    index > _startContainer + 2. */
    unsigned int getStartContainer() const;
    /** \brief Get the CommandMap and collection callback for a-buffer geometry. 
    \details Calling code should assign this CommandMap and Node::Callback to the
    jagSG root node of any subgraphs that are to be rendered using the a-buffer
    algorithm. */
    void getABufferControls( jagDraw::CommandMapPtr& commands, jagSG::SelectContainerCallbackPtr& callback );
    /** \overload */
    void getABufferControls( jagSG::SelectContainerCallbackPtr& callback );
    /** \brief Get the NodeContainer index for transparent geometry rendering.
    \details Equivalent to getStartContainer() + 1. */
    unsigned int getTransparentNodeContainer() const;

    /**@}*/


    /** \brief Call this function to render a frame. */
    void renderFrame( jagSG::CollectionVisitor& collect, jagDraw::DrawInfo& drawInfo );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

    /** \brief Bitflags indicating matrix uniforms required by ABuffer shader code.
    \details Application code should bitwise-OR these flags into the TransformCallback class.
    Here's an example, for use with jagSG::CollectionVisitor:
    \code
    jagDraw::TransformCallback* xformCB( CollectionVisitor::getTransformCallback() );
    xformCB->setRequiredMatrixUniforms( xformCB->getRequiredMatrixUniforms() |
        ABuffer::getRequiredMatrixUniforms() );
    \endcode */
    static unsigned int getRequiredMatrixUniforms();


    typedef enum {
        RESOLVE_GELLY, /*!< Uses the gelly resolve from the originaal icare3d.org example code. */
        RESOLVE_ALPHA_BLEND, /*!< Uses the alpha blend resolve from the originaal icare3d.org example code. */

        /// Uses a modified alpha blend that sacrifices interior geometry color
        /// in order to eliminate co-planar geometry z-fighting artifacts, which
        /// commonly occur with CAD models that were designed for manufacturing rather
        /// than visual rendering.
        RESOLVE_ALPHA_BLEND_CAD,
        UNSPECIFIED /*!< Behavior is identical to RESOLVE_ALPHA_BLEND_CAD. */
    } ResolveMethod;

    /** \brief Specify the a-buffer resolve algorithm.
    \details Controls how the linked list of fragment data is combined into
    a final color. The default is RESOLVE_ALPHA_BLEND_CAD. */
    void setResolveMethod( const ResolveMethod resolveMethod );
    /** \brief Get the resolve algorithm. */
    ResolveMethod getResolveMethod() const;

    /** \brief Enum / string conversion. */
    static std::string resolveMethodToString( const ResolveMethod& resolveMethod );
    /** \brief Enum / string conversion. */
    static ResolveMethod stringToResolveMethod( const std::string& resolveMethod );

    /** \brief Set the alpha value assigned to every fragment in the linked list.
    \details \c fragmentAlpha is clamped to the range 0.0 - 1.0.
    Default value: 0.2f */
    void setFragmentAlpha( const float fragmentAlpha );
    /** \brief Get the fragment alpha value. */
    float getFragmentAlpha() const;

    /** \brief Set the maximum fragments per pixel.
    \details ABuffer shader code uses a temporary buffer to sort the stored
    fragments per pixel. Set the size of this buffer with setMaxFragments().
    If the buffer is too small, incorrect rendering will occur. Too large of
    a saze is wasteful of GPU core memory and may impede performance.

    The buffer is declared in data/abufferResolve.frag as:
    \code
    vec4 fragmentList[ABUFFER_SIZE];
    \endcode
    setMaxFragments() controls the value of the ABUFFER_SIZE constant.

    The default value is 16. */
    void setMaxFragments( const unsigned int maxFragments );
    /** \brief Get the maximum fragments per pixel. */
    unsigned int getMaxFragments() const;

    /** \brief Enable or disable the secondary color buffer.
    \details If enabled, _colorBuffer1 will be added to the final resolve color.
    Default value: Default is true if \c colorBuffer1 is non-NULL during construction.
    Default is false otherwise. */
    void setSecondaryColorBufferEnable( const bool secondaryEnable=true );
    /** \brief Get enable state of secondary color buffer. */
    bool getSecondaryColorBufferEnable() const;

protected:
    void internalInit();

    std::string shaderResolveParameters() const;
    std::string shaderSizeParameters() const;

    jagDraw::TexturePtr _colorBuffer0, _colorBuffer1;
    jagDraw::TexturePtr _depthBuffer;
    unsigned int _startContainer;
    bool _secondaryEnable;

    int _width, _height;

    jagDraw::FramebufferPtr _defaultFBO;
    jagDraw::CommandMapPtr _commands, _opaqueCommands;
    unsigned int _opaqueContainer;
    jagSG::SelectContainerCallbackPtr _callback, _opaqueCallback;
    jagDraw::DrawGraphPtr _drawGraphTemplate;

    jagDraw::ProgramPtr _clearProgram, _renderProgram, _resolveProgram;
    ResolveMethod _resolveMethod;
    float _fragmentAlpha;
    unsigned int _maxFragments;

    unsigned int _numContexts;

    PerContextABufferCntxt _abufferCntxt;
};

typedef jag::base::ptr< ABuffer >::shared_ptr ABufferPtr;




struct ABufferContext
{
    ABufferContext();


    std::string _logName;

    bool _needsInit;
    int _texWidth, _texHeight;

    typedef float AbufferType;

    bool _queryRequested;
    GLuint _queryID;

    unsigned int _sharedPoolSize;
    GLuint _pageIdxBuffID, _fragCountBuffID, _semaphoreBuffID;
    GLuint64EXT _pageIdxAddress, _fragCountAddress, _semaphoreAddress;
    GLuint _sharedPageListBuffID, _sharedLinkListBuffID;
    GLuint64EXT _sharedPageListAddress, _sharedLinkListAddress;
    GLuint _curSharedPageBuffID;
    GLuint64EXT _curSharedPageAddress;

    bool init();

    void assignUniforms( const GLuint prog );

    bool managePool();
};


// jagUtil
}


// __JAGUTIL_A_BUFFER_H__
#endif
