/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jag/util/Blur.h>

#include <jag/draw/Common.h>
#include <jag/util/QuadNode.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>
#include <jag/disk/ReadWrite.h>

#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>
#include <sstream>
#include <iomanip>


namespace jag {
namespace util {


Blur::Blur( const std::string& logName )
    : jag::base::LogBase( logName.empty() ? "jag.util.blur" : logName ),
      _numContexts( 0 ),
      _width( 0 ),
      _height( 0 )
{
}
Blur::Blur( jag::draw::TexturePtr& inputBuffer, jag::draw::TexturePtr& outputBuffer,
        const std::string& logName )
    : jag::base::LogBase( logName.empty() ? "jag.util.blur" : logName ),
      _inputBuffer( inputBuffer ),
      _outputBuffer( outputBuffer ),
      _numContexts( 0 ),
      _width( 0 ),
      _height( 0 )
{
}
Blur::Blur( const Blur& rhs )
    : jag::base::LogBase( rhs ),
      _inputBuffer( rhs._inputBuffer ),
      _outputBuffer( rhs._outputBuffer ),
      _numContexts( rhs._numContexts ),
      _width( rhs._width ),
      _height( rhs._height )
{
}
Blur::~Blur()
{
}

void Blur::setMaxContexts( const unsigned int numContexts )
{
    _numContexts = numContexts;

    if( _vQuad == NULL )
    {
        jag::draw::ShaderPtr fragStage0;
        jag::draw::ShaderPtr fragStage1;
        internalInit( fragStage0, fragStage1 );
    }

    _container.setMaxContexts( numContexts );
}


// Convenience utility for reading shader source.
#define __READ_UTIL( _RESULT, _TYPE, _NAME ) \
    { \
        boost::any anyTemp( jag::disk::read( _NAME ) ); \
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

void Blur::setShaders( const std::string& fragStage0, const std::string& fragStage1 )
{
    jag::draw::ShaderPtr stage0, stage1;
    __READ_UTIL( stage0, jag::draw::ShaderPtr, fragStage0 );
    __READ_UTIL( stage1, jag::draw::ShaderPtr, fragStage1 );

    internalInit( stage0, stage1 );
}
void Blur::setShaders( jag::draw::ShaderPtr& fragStage0, jag::draw::ShaderPtr& fragStage1 )
{
    internalInit( fragStage0, fragStage1 );
}

jag::draw::DrawNodeContainer& Blur::getNodeContainer()
{
    setMaxContexts( _numContexts );

    return( _container );
}

void Blur::reshape( const int w, const int h )
{
    _width = w;
    _height = h;

    if( _vQuad == NULL )
    {
        jag::draw::ShaderPtr fragStage0;
        jag::draw::ShaderPtr fragStage1;
        internalInit( fragStage0, fragStage1 );
    }

    _hQuad->reshape( w, h );
    _vQuad->reshape( w, h );

    _intermediateBuffer->getImage()->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _intermediateBuffer->markAllDirty();
}

void Blur::internalInit( jag::draw::ShaderPtr& fragStage0, jag::draw::ShaderPtr& fragStage1 )
{
    // Create the intermediate color buffer.
    jag::draw::ImagePtr image( new jag::draw::Image() );
    image->set( 0, GL_RGBA, _width, _height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    _intermediateBuffer.reset( new jag::draw::Texture( GL_TEXTURE_2D, image,
        jag::draw::SamplerPtr( new jag::draw::Sampler() ) ) );
    _intermediateBuffer->setUserDataName( "Blur intermediateBuffer" );
    _intermediateBuffer->getSampler()->getSamplerState()->_minFilter = GL_NEAREST;
    _intermediateBuffer->getSampler()->getSamplerState()->_magFilter = GL_NEAREST;

    // Create the horizontal blur.
    _hQuad.reset( new QuadNode( _inputBuffer, _intermediateBuffer ) );
    if( fragStage0 != NULL )
    {
        jag::draw::ShaderPtr vert;
        _hQuad->setShaders( fragStage0, vert );
    }
    else
        _hQuad->setShaders( "blurHorizontal.frag" );

    // Create the vertical blur.
    _vQuad.reset( new QuadNode( _intermediateBuffer, _outputBuffer ) );
    if( fragStage1 != NULL )
    {
        jag::draw::ShaderPtr vert;
        _vQuad->setShaders( fragStage1, vert );
    }
    else
        _vQuad->setShaders( "blurVertical.frag" );

    // Configure the NodeContainer.
    _container.clear();
    _container.setResetEnable( false );
    // TBD add depth control.
    _container.push_back( _hQuad );
    _container.push_back( _vQuad );
}


// namespace jag::util::
}
}
