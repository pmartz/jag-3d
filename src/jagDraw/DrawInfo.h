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
#include <jagDraw/ContextSupport.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Program.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/UniformBlock.h>


namespace jagDraw {


/** \class DrawInfo DrawInfo.h <jagDraw/DrawInfo.h>
\brief
\details
*/
struct JAGDRAW_EXPORT DrawInfo
{
    DrawInfo();

    jagDrawContextID _id;

    CommandMap _current;


    typedef std::map< Program::HashValue, ConstUniformPtr > UniformMap;

    /** \brief List of active uniforms during draw.
    \details When a new Program is used, it iterates over its
    active uniforms and attempts to find a hash key match in this map.
    If a match is found, the corresponding uniform value is specified.
    */
    UniformMap _uniformMap;


    typedef std::map< Program::HashValue, ConstUniformBlockPtr > UniformBlockMap;

    /** \brief List of active uniform blocks during draw.
    \details When a new Program is used, it iterates over its
    active uniform blocks and attempts to find a hash key match in this map.
    If a match is found, it binds the corresponding uniform block.
    */
    UniformBlockMap _uniformBlockMap;
};


// jagDraw
}


// __JAGDRAW_DRAW_INFO_H__
#endif
