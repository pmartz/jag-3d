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
#include <jagBase/ptr.h>
#include <jagBase/LogBase.h>


// Forwaed
namespace jagSG {
    class CollectionVisitor;
}


namespace jagUtil
{


// Forwaed
struct ABufferContext;
typedef jagDraw::PerContextData< ABufferContext > PerContextABufferCntxt;


/** \class ABuffer ABuffer.h <jagDraw/ABuffer.h>
*/
class JAGUTIL_EXPORT ABuffer
{
public:
    ABuffer();
    ABuffer( jagDraw::TexturePtr& depthBuffer, jagDraw::TexturePtr& colorBuffer0,
        jagDraw::TexturePtr& colorBuffer1=jagDraw::TexturePtr((jagDraw::Texture*)NULL) );
    ABuffer( const ABuffer& rhs );
    ~ABuffer();

    void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    \details TBD
    \param numContexts Maximum number of OpenGL contexts.
    \param startContainer Index of the DrawGraph's NodeContainer for ABuffer rendering. */
    jagDraw::DrawGraphPtr& createDrawGraphTemplate( const unsigned int startContainer=1 );

    /** \brief Get the CommandMap and collection callback for a-buffer geometry. 
    \details Calling code should assign this CommandMap and Node::Callback to the
    jagSG root node of any subgraphs that are to be rendered using the a-buffer
    algorithm. */
    void getABufferControls( jagDraw::CommandMapPtr& commands, jagSG::SelectContainerCallbackPtr& callback );

    /** \brief Call this function to render a frame. */
    void renderFrame( jagSG::CollectionVisitor& collect, jagDraw::DrawInfo& drawInfo );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

    static unsigned int getRequiredMatrixUniforms();

protected:
    void internalInit();

    jagDraw::TexturePtr _colorBuffer0, _colorBuffer1;
    jagDraw::TexturePtr _depthBuffer;
    unsigned int _startContainer;

    int _width, _height;

    jagDraw::FramebufferPtr _defaultFBO;
    jagDraw::CommandMapPtr _commands;
    jagSG::SelectContainerCallbackPtr _callback;
    jagDraw::DrawGraphPtr _drawGraphTemplate;

    jagDraw::ProgramPtr _clearProgram, _renderProgram, _resolveProgram;

    unsigned int _numContexts;

    PerContextABufferCntxt _abufferCntxt;
};

typedef jagBase::ptr< ABuffer >::shared_ptr ABufferPtr;




struct ABufferContext
{
    ABufferContext();


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
