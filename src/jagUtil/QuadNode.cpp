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

#include <jagUtil/QuadNode.h>

#include <jag/draw/Common.h>
#include <jag/disk/ReadWrite.h>
#include <jagUtil/Shapes.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

#include <gmtl/gmtl.h>

#include <boost/foreach.hpp>

#include <string>
#include <sstream>
#include <iomanip>


namespace jagUtil
{


QuadNode::QuadNode( const std::string& logName )
    : jag::draw::DrawNode( logName.empty() ? "jag.util.quadnode" : logName ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
}
QuadNode::QuadNode( jag::draw::TexturePtr inputBuffer, jag::draw::TexturePtr outputBuffer,
        const std::string& logName )
    : jag::draw::DrawNode( logName.empty() ? "jag.util.quadnode" : logName ),
      _outputBuffer( outputBuffer ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
    _inputBuffers.push_back( inputBuffer );
}
QuadNode::QuadNode( jag::draw::TextureVec& inputBuffers, jag::draw::TexturePtr outputBuffer,
        const std::string& logName )
    : jag::draw::DrawNode( logName.empty() ? "jag.util.quadnode" : logName ),
      _inputBuffers( inputBuffers ),
      _outputBuffer( outputBuffer ),
      _width( 0 ),
      _height( 0 ),
      _numContexts( 0 )
{
}
QuadNode::QuadNode( const QuadNode& rhs )
    : jag::draw::DrawNode( rhs ),
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

void QuadNode::setShaders( const std::string& fragName, const std::string& vertName )
{
    const std::string localVertName( vertName.empty() ? "quadNode.vert" : vertName );
    jag::draw::ShaderPtr vs, fs;
    __READ_UTIL( vs, jag::draw::ShaderPtr, localVertName );
    __READ_UTIL( fs, jag::draw::ShaderPtr, fragName );

    // Tell the fragment shader how many textures to combine.
    fs->insertSourceString( getTextureCountString() );

    jag::draw::ProgramPtr prog( jag::draw::ProgramPtr( new jag::draw::Program() ) );
    prog->attachShader( vs );
    prog->attachShader( fs );

    internalInit( prog );
}
void QuadNode::setShaders( jag::draw::ShaderPtr& frag, jag::draw::ShaderPtr& vert )
{
    jag::draw::ShaderPtr localVert( vert );
    if( vert == NULL )
    {
        __READ_UTIL( localVert, jag::draw::ShaderPtr, "quadNode.vert" );
    }

    jag::draw::ProgramPtr prog( jag::draw::ProgramPtr( new jag::draw::Program() ) );
    prog->attachShader( localVert );
    prog->attachShader( frag );

    internalInit( prog );
}
void QuadNode::setProgram( jag::draw::ProgramPtr& program )
{
    internalInit( program );
}


void QuadNode::setMaxContexts( const unsigned int numContexts )
{
    _numContexts = numContexts;

    if( _fbo == NULL )
        internalInit();

    jag::draw::DrawNode::setMaxContexts( numContexts );
}

void QuadNode::reshape( const int w, const int h )
{
    _width = w;
    _height = h;

    if( _fbo == NULL )
        internalInit();

    _fbo->setViewport( 0, 0, _width, _height );
}

void QuadNode::internalInit( jag::draw::ProgramPtr& program )
{
    _commands.reset( new jag::draw::CommandMap() );

    // Create texture set. Specify sampler uniforms.
    jag::draw::TextureSetPtr texSet( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );
    jag::draw::UniformSetPtr usp( jag::draw::UniformSetPtr( new jag::draw::UniformSet() ) );
    unsigned int idx( 0 );
    BOOST_FOREACH( jag::draw::TexturePtr& item, _inputBuffers )
    {
        std::ostringstream ostr;
        ostr << "texture" << idx;
        jag::draw::UniformPtr uniform( jag::draw::UniformPtr( new jag::draw::Uniform( ostr.str(), GL_SAMPLER_2D, idx ) ) );
        usp->insert( uniform );

        (*texSet)[ GL_TEXTURE0 + idx++ ] = item;
    }
    _commands->insert( usp );
    _commands->insert( texSet );

    // Create quad (tri pair) geometry.
    jagUtil::VNTCVec data;
    jag::draw::DrawablePtr fstp( jagUtil::makePlane( data, gmtl::Point3f( -1., -1., 0. ),
        gmtl::Vec3f( 2., 0., 0. ), gmtl::Vec3f( 0., 2., 0. ) ) );
    _commands->insert( jagUtil::createVertexArrayObject( data ) );

    _fbo.reset( new jag::draw::Framebuffer( GL_DRAW_FRAMEBUFFER ) );
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
        jag::draw::ShaderPtr vs, fs;
        __READ_UTIL( vs, jag::draw::ShaderPtr, "quadNode.vert" );
        __READ_UTIL( fs, jag::draw::ShaderPtr, "quadNode.frag" );

        // Tell the fragment shader how many textures to combine.
        fs->insertSourceString( getTextureCountString() );

        jag::draw::ProgramPtr prog( jag::draw::ProgramPtr( new jag::draw::Program() ) );
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
