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

#ifndef __JAGDRAW_SHADER_H__
#define __JAGDRAW_SHADER_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogBase.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>
#include <string>
#include <vector>


namespace jagDraw {


/** \class Shader Shader.h <jagDraw/Shader.h>
\brief TBD.
*/
class JAGDRAW_EXPORT Shader : public jagBase::LogBase
{
public:
    Shader( GLenum type );
    ~Shader();

    void addSourceFile( const std::string& fileName );

    void addSourceString( const std::string& source );

    // TBD need to get context ID, probably as a param?
    GLuint getId();

    // TBD need to get context ID, probably as a param?
    void printInfoLog( const GLuint id );

private:
    void internalInit( const unsigned int contextID );

    std::string loadSource( const std::string& fileName );

    GLenum _type;
    std::vector< std::string > _sourceList;

    PerContextGLuint _ids;
};

typedef jagBase::ptr< jagDraw::Shader >::shared_ptr ShaderPtr;
typedef std::vector< ShaderPtr > ShaderList;


// jagDraw
}


// __JAGDRAW_SHADER_H__
#endif
