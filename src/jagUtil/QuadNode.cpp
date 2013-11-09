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


// Convenience utility for reading shader source.
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

void QuadNode::setShaders( const std::string& fragName, const std::string& vertName )
{
    const std::string localVertName( vertName.empty() ? "quadNode.vert" : vertName );
    jagDraw::ShaderPtr vs, fs;
    __READ_UTIL( vs, jagDraw::ShaderPtr, localVertName );
    __READ_UTIL( fs, jagDraw::ShaderPtr, fragName );

    // Tell the fragment shader how many textures to combine.
    fs->insertSourceString( getTextureCountString() );

    jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program() ) );
    prog->attachShader( vs );
    prog->attachShader( fs );

    internalInit( prog );
}
void QuadNode::setShaders( jagDraw::ShaderPtr& frag, jagDraw::ShaderPtr& vert )
{
    jagDraw::ShaderPtr localVert( vert );
    if( vert == NULL )
    {
        __READ_UTIL( localVert, jagDraw::ShaderPtr, "quadNode.vert" );
    }

    jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program() ) );
    prog->attachShader( localVert );
    prog->attachShader( frag );

    internalInit( prog );
}
void QuadNode::setProgram( jagDraw::ProgramPtr& program )
{
    internalInit( program );
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

void QuadNode::internalInit( jagDraw::ProgramPtr& program )
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

    if( program != NULL )
    {
        // Use app-supplied program.
        _commands->insert( program );
    }
    else // Use default program.
    {
        // Load shaders and create program.
        jagDraw::ShaderPtr vs, fs;
        __READ_UTIL( vs, jagDraw::ShaderPtr, "quadNode.vert" );
        __READ_UTIL( fs, jagDraw::ShaderPtr, "quadNode.frag" );

        // Tell the fragment shader how many textures to combine.
        fs->insertSourceString( getTextureCountString() );

        jagDraw::ProgramPtr prog( jagDraw::ProgramPtr( new jagDraw::Program() ) );
        prog->attachShader( vs );
        prog->attachShader( fs );
        _commands->insert( prog );
    }


    this->setCommandMap( _commands );
    this->addDrawable( fstp );
}

std::string QuadNode::getTextureCountString() const
{
    std::ostringstream texCount;
    texCount << std::min< unsigned int >( (unsigned int)(_inputBuffers.size()), 4 ) << "\n";
    return( std::string( "#define TEXTURE_COUNT " ) +
        std::string( texCount.str() ) );
}


// jagUtil
}
