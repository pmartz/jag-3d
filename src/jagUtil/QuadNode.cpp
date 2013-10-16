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

#include <jagUtil/QuadNode.h>

#include <jagDraw/Common.h>
#include <jagDisk/ReadWrite.h>
#include <jagUtil/Shapes.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>
#include <sstream>
#include <iomanip>


namespace jagUtil
{


QuadNode::QuadNode( const std::string& logName )
    : jagDraw::Node( logName.empty() ? "jag.util.quadnode" : logName ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
}
QuadNode::QuadNode( jagDraw::TexturePtr inputBuffer, jagDraw::TexturePtr outputBuffer,
        const std::string& logName )
    : jagDraw::Node( logName.empty() ? "jag.util.quadnode" : logName ),
      _outputBuffer( outputBuffer ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
    _inputBuffers.push_back( inputBuffer );
}
QuadNode::QuadNode( jagDraw::TextureVec& inputBuffers, jagDraw::TexturePtr outputBuffer,
        const std::string& logName )
    : jagDraw::Node( logName.empty() ? "jag.util.quadnode" : logName ),
      _inputBuffers( inputBuffers ),
      _outputBuffer( outputBuffer ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
}
QuadNode::QuadNode( const QuadNode& rhs )
    : jagDraw::Node( rhs ),
      _inputBuffers( rhs._inputBuffers ),
      _outputBuffer( rhs._outputBuffer ),
      _width( rhs._width ),
      _height( rhs._height ),
      _numContexts( rhs._numContexts )
{
}
QuadNode::~QuadNode()
{
}

void QuadNode::setMaxContexts( const unsigned int numContexts )
{
    _numContexts = numContexts;

    if( _fbo == NULL )
        internalInit();

    jagDraw::Node::setMaxContexts( numContexts );
}

void QuadNode::reshape( const int w, const int h )
{
    _width = w;
    _height = h;

    if( _fbo == NULL )
        internalInit();

    _fbo->setViewport( 0, 0, _width, _height );
}

void QuadNode::internalInit()
{
    _commands.reset( new jagDraw::CommandMap() );

    // Create texture set. Specify sampler uniforms.
    jagDraw::TextureSetPtr texSet( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
    jagDraw::UniformSetPtr usp( jagDraw::UniformSetPtr( new jagDraw::UniformSet() ) );
    unsigned int idx( 0 );
    BOOST_FOREACH( jagDraw::TexturePtr& item, _inputBuffers )
    {
        std::ostringstream ostr;
        ostr << "texture" << idx;
        jagDraw::UniformPtr uniform( jagDraw::UniformPtr( new jagDraw::Uniform( ostr.str(), GL_SAMPLER_2D, idx ) ) );
        usp->insert( uniform );

        (*texSet)[ GL_TEXTURE0 + idx++ ] = item;
    }
    _commands->insert( usp );
    _commands->insert( texSet );

    // Create quad (tri pair) geometry.
    jagUtil::VNTCVec data;
    jagDraw::DrawablePtr fstp( jagUtil::makePlane( data, gmtl::Point3f( -1., -1., 0. ),
        gmtl::Vec3f( 2., 0., 0. ), gmtl::Vec3f( 0., 2., 0. ) ) );
    _commands->insert( jagUtil::createVertexArrayObject( data ) );

    _fbo.reset( new jagDraw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
    _fbo->setViewport( 0, 0, _width, _height );
    _fbo->setClear( 0 );
    if( _outputBuffer != NULL )
        _fbo->addAttachment( GL_COLOR_ATTACHMENT0, _outputBuffer );
    _commands->insert( _fbo );

    // Load shaders and create program.
#define __READ_UTIL( _RESULT, _TYPE, _NAME ) \
    { \
        boost::any anyTemp( jagDisk::read( _NAME ) ); \
        try { \
            _RESULT = boost::any_cast< _TYPE >( anyTemp ); \
        } \
        catch( boost::bad_any_cast bac ) \
        { \
            bac.what(); \
        } \
        if( _RESULT == NULL ) \
        { \
            JAG3D_FATAL( std::string("Can't load \"") + std::string(_NAME) + std::string("\".") ); \
            return; \
        } \
    }
    jagDraw::ShaderPtr vs, fs;
    __READ_UTIL( vs, jagDraw::ShaderPtr, "quadNode.vert" );
    __READ_UTIL( fs, jagDraw::ShaderPtr, "quadNode.frag" );
#undef __READ_UTIL

    // Tell the fragment shader how many textures to combine.
    std::ostringstream texCount;
    texCount << std::min< unsigned int >( (unsigned int)(_inputBuffers.size()), 4 ) << "\n";
    fs->insertSourceString( std::string( "#define TEXTURE_COUNT " ) +
        std::string( texCount.str() ) );

    jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program() ) );
    prog->attachShader( vs );
    prog->attachShader( fs );
    _commands->insert( prog );


    this->setCommandMap( _commands );
    this->addDrawable( fstp );
}


// jagUtil
}
