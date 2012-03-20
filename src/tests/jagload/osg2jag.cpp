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

#include "osg2jag.h"

#include <osg/Geode>
#include <osg/Geometry>

#include <jagBase/LogMacros.h>


Osg2Jag::Osg2Jag()
  : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN )
{
    jagDraw::DrawablePtr draw( jagDraw::DrawablePtr( new jagDraw::Drawable ) );
    _jagDrawables.push_back( draw );

    /*
    const char* fShaderSource =
        "#version 130 \n"
        "uniform float scale; \n"
        "in vec3 cOut; \n"
        "out vec4 colorOut; \n"
        "void main() { \n"
        "    colorOut = vec4( cOut * scale, 1. ); \n"
        "}";
    jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
    fs->addSourceString( std::string( fShaderSource ) );

    _spp2 = jagDraw::ShaderProgramPtr( new jagDraw::ShaderProgram );
    _spp2->attachShader( vs );
    _spp2->attachShader( fs );
    */
}
Osg2Jag::~Osg2Jag()
{
}

void Osg2Jag::apply( osg::Node& node )
{
    JAG3D_NOTICE_STATIC( "jag.demo.jagload", "Node" );
    traverse( node );
}
void Osg2Jag::apply( osg::Geode& node )
{
    JAG3D_NOTICE_STATIC( "jag.demo.jagload", "Geode" );
    traverse( node );
}

const jagDraw::DrawableList& Osg2Jag::getJagDrawableList() const
{
    return( _jagDrawables );
}
