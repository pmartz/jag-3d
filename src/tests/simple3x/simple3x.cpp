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

#include <DemoInterface.h>

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/Shader.h>
#include <jagDraw/ShaderProgram.h>
#include <jagDraw/Error.h>

#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;


class Simple3xDemo : public DemoInterface
{
public:
    Simple3xDemo() {}
    virtual ~Simple3xDemo() {}

    virtual bool init();
    virtual bool frame();
    virtual bool shutdown() { return( true ); }

protected:
    jagDraw::BufferObjectPtr _bop;
    jagDraw::ShaderProgramPtr _spp;
};


DemoInterface* DemoInterface::create()
{
    return( new Simple3xDemo );
}

bool Simple3xDemo::init()
{
    // Display information on the type of vontext we created.
    cout << string( "GL_VENDOR: " ) << string( (char*)(glGetString( GL_VENDOR )) ) << endl;;
    cout << string( "GL_RENDERER: " ) << string( (char*)(glGetString( GL_RENDERER )) ) << endl;
    cout << string( "GL_VERSION: " ) << string( (char*)(glGetString( GL_VERSION )) ) << endl;
    cout << string( "GL_SHADING_LANGUAGE_VERSION: " ) << string( (char*)(glGetString( GL_SHADING_LANGUAGE_VERSION )) ) << endl;

    glClearColor( 0.5f, 0.5f, 0.5f, 0.f );

    {
        float z = .5;
        float verts[] = {
            -1., -1., z,
            .5, -1., z,
            -.5, 1., z,
            1., 1., z };
        jagBase::BufferPtr bp( new jagBase::Buffer( sizeof( verts ), (void*)verts ) );
        _bop = jagDraw::BufferObjectPtr( new jagDraw::BufferObject( jagDraw::BufferObject::ArrayBuffer, bp, jagDraw::BufferObject::StaticDraw ) );
    }

    {
        const char* vShaderSource =
            "#version 120 \n" \
            "in vec3 vertex; \n" \
            "void main() { \n" \
            "    gl_Position = vec4( vertex, 1. ); \n" \
            "}";
        jagDraw::ShaderPtr vs( new jagDraw::Shader( GL_VERTEX_SHADER ) );
        vs->addSourceString( std::string( vShaderSource ) );

        const char* fShaderSource =
            "#version 120 \n" \
            "void main() { \n" \
            "    gl_FragData[ 0 ] = vec4( 0., 1., 1., 0. ); \n" \
            "}";
        jagDraw::ShaderPtr fs( new jagDraw::Shader( GL_FRAGMENT_SHADER ) );
        fs->addSourceString( std::string( fShaderSource ) );

        _spp = jagDraw::ShaderProgramPtr( new jagDraw::ShaderProgram );
        _spp->attachShader( vs );
        _spp->attachShader( fs );
    }

    return( true );
}

bool Simple3xDemo::frame()
{
    glClear( GL_COLOR_BUFFER_BIT );

    _spp->apply();

    _bop->apply();
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0 );
    glEnableVertexAttribArray( 0 );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    glFlush ();

    JAG_ERROR_CHECK( "simple3x display()" );

    return( true );
}
