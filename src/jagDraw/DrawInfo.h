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

#ifndef __JAGDRAW_DRAW_INFO_H__
#define __JAGDRAW_DRAW_INFO_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/ContextSupport.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/PerContextData.h>

#include <jagDraw/Uniform.h>
#include <jagDraw/UniformBlock.h>
#include <gmtl/gmtl.h>


namespace jagDraw {


/** \class DrawInfo DrawInfo.h <jagDraw/DrawInfo.h>
\brief Draw-traversal control information.
\details Contains information used to execute and optimize the draw traversal.
*/
struct JAGDRAW_EXPORT DrawInfo
{
    DrawInfo();


    /** Jag context identifier. */
    jagDrawContextID _id;

    /** The current CommandMap. As each jagDraw::Node is executed during
    the draw traversal, \c _current is updated to reflect the CommandMap in
    effect. \c _current is used to access the current jagDraw::Program to
    bind new Uniform and UniformBlock objects, amoung other things. */
    CommandMap _current;

    /** \brief Buffer object ID most recently bound to GL_ELEMENT_ARRAY_BUFFER.
    \details Facilitates element buffer sharing and eliminates redundant
    element buffer binding by tracking the most recently bound element buffer
    object. */
    GLuint _elementBufferID;

    /** */
    gmtl::Matrix44d _transform;


    typedef std::map< Program::HashValue, UniformPtr > UniformMap;

    /** \brief List of active uniforms during draw.
    \details When a new Program is used, it iterates over its
    active uniforms and attempts to find a hash key match in this map.
    If a match is found, the corresponding uniform value is specified.
    */
    UniformMap _uniformMap;

    /**\brief Returns an existi9ng, or creates a new uniform in _uniformMap.
    \details TBD
    */
    UniformPtr& getOrCreateUniform( const std::string& name )
    {
        const Program::HashValue hashVal( Program::createHash( name ) );
        UniformMap::iterator it( _uniformMap.find( hashVal ) );
        if( it == _uniformMap.end() )
        {
            UniformPtr uniform( UniformPtr( new Uniform( name ) ) );
            _uniformMap[ hashVal ] = uniform;
            it = _uniformMap.find( hashVal );
        }
        return( it->second );
    }


    typedef std::map< Program::HashValue, ConstUniformBlockPtr > UniformBlockMap;

    /** \brief List of active uniform blocks during draw.
    \details When a new Program is used, it iterates over its
    active uniform blocks and attempts to find a hash key match in this map.
    If a match is found, it binds the corresponding uniform block.
    */
    UniformBlockMap _uniformBlockMap;
};

typedef PerContextData< DrawInfo > PerContextDrawInfo;


// jagDraw
}


// __JAGDRAW_DRAW_INFO_H__
#endif
