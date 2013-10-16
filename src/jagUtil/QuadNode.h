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

#ifndef __JAGUTIL_QUAD_NODE_H__
#define __JAGUTIL_QUAD_NODE_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Node.h>
#include <jagDraw/Texture.h>
#include <jagDraw/Framebuffer.h>
#include <jagDraw/CommandMap.h>
#include <jagBase/ptr.h>
#include <jagBase/LogBase.h>



namespace jagUtil
{


/** \class QuadNode QuadNode.h <jagUtil/QuadNode.h>
\brief A jagDraw::Node for drawing a textured quad.
\details TBD

\logname jag.util.quadnode
*/
class JAGUTIL_EXPORT QuadNode : public jagDraw::Node
{
public:
    QuadNode( const std::string& logName=std::string( "" ) );
    QuadNode( jagDraw::TexturePtr inputBuffer,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    QuadNode( jagDraw::TextureVec& inputBuffers,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    QuadNode( const QuadNode& rhs );
    ~QuadNode();

    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit();

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
