/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

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


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class DrawInfo DrawInfo.h <jagDraw/DrawInfo.h>
\brief Draw-traversal control information.
\details Contains information used to execute and optimize the draw traversal.
*/
struct JAGDRAW_EXPORT DrawInfo
{
    DrawInfo();

    void startFrame( const bool startFrame=true );


    /** \brief Set to true just before the render traversal. */
    bool _startFrame;

    /** Jag context identifier. */
    jagDrawContextID _id;

    /** The current CommandMap. As each jagDraw::Node is executed during
    the draw traversal, \c _current is updated to reflect the CommandMap in
    effect. \c _current is used to access the current jagDraw::Program to
    bind new Uniform and UniformBlock objects, amoung other things. */
    CommandMap _current;

    /** \brief Support for external OpenGL programs.
    \details To use an application-bound program with JAG rendering, do not
    specify a Program object in the _current CommandMap. Instead, specify
    the OpenGL program id here. */
    GLuint _externalProgramID;

    /** \brief Buffer object ID most recently bound to GL_ELEMENT_ARRAY_BUFFER.
    \details Facilitates element buffer sharing and eliminates redundant
    element buffer binding by tracking the most recently bound element buffer
    object. */
    GLuint _elementBufferID;

    /** \brief Track the most recent jagDraw::Node matrix. */
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


/*@}*/


// jagDraw
}


// __JAGDRAW_DRAW_INFO_H__
#endif
