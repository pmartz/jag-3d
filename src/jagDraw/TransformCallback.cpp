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

#include <jagDraw/TransformCallback.h>
#include <jagDraw/DrawInfo.h>


namespace jagDraw {


TransformCallback::TransformCallback()
    : Node::Callback(),
    _transform(),
    _requiredUniforms( jagBase::TransformD::MODEL_VIEW_PROJ |
            jagBase::TransformD::MODEL_VIEW |
            jagBase::TransformD::MODEL_VIEW_INV_TRANS )
{
    setDefaultMatrixUniformNames();
}
TransformCallback::TransformCallback( const TransformCallback& rhs )
    : Node::Callback( rhs ),
    _transform( rhs._transform ),
    _requiredUniforms( rhs._requiredUniforms ),
    _nameMap( rhs._nameMap )
{
}
TransformCallback::~TransformCallback()
{
}


bool TransformCallback::operator()( jagDraw::Node& node, jagDraw::DrawInfo& drawInfo )
{
    if( !drawInfo._startFrame &&
        ( drawInfo._transform == node.getTransform() ) )
    {
        return( true );
    }
    drawInfo.startFrame( false );
    drawInfo._transform = node.getTransform();

    _transform.setModel( node.getTransform() );

    gmtl::Matrix33f mat3;
    gmtl::Matrix44f mat4;
    unsigned int flags( _requiredUniforms );
    unsigned int matrixBit( 1 );
    while( flags != 0 )
    {
        if( ( flags & matrixBit ) != 0 )
        {
            // Clear the bit
            flags ^= matrixBit;

            jagDraw::UniformPtr& uniform( drawInfo.getOrCreateUniform( _nameMap[ matrixBit ] ) );
            if( matrixBit != jagBase::TransformD::MODEL_VIEW_INV_TRANS )
            {
                gmtl::convert( mat4, _transform.getMatrix4( matrixBit ) );
                uniform->setType( GL_FLOAT_MAT4 );
                uniform->set( mat4 );
            }
            else
            {
                gmtl::convert( mat3, _transform.getMatrix3() );
                uniform->setType( GL_FLOAT_MAT3 );
                uniform->set( mat3 );
            }
            uniform->executeWithoutMap( drawInfo );
        }
        matrixBit <<= 1;
    }
    //_transform.setDirty( 0 );

    return( true );
}


void TransformCallback::setRequiredMatrixUniforms( const unsigned int flags )
{
    _requiredUniforms = flags;
}
unsigned int TransformCallback::getRequiredMatrixUniforms() const
{
    return( _requiredUniforms );
}

void TransformCallback::setMatrixUniformName( const unsigned int matrix, const std::string& name )
{
    _nameMap[ matrix ] = name;
}
std::string TransformCallback::getMatrixUniformName( const unsigned int matrix ) const
{
    UniformNameMap::const_iterator it( _nameMap.find( matrix ) );
    if( it != _nameMap.end() )
        return( it->second );
    else
        return( std::string( "" ) );
}
void TransformCallback::setDefaultMatrixUniformNames()
{
    _nameMap[ jagBase::TransformD::MODEL ] = "jagModelMatrix";
    _nameMap[ jagBase::TransformD::VIEW ] = "jagViewMatrix";
    _nameMap[ jagBase::TransformD::PROJ ] = "jagProjMatrix";
    _nameMap[ jagBase::TransformD::VIEW_PROJ ] = "jagViewProjMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_PROJ ] = "jagModelViewProjMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW ] = "jagModelViewMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_INV_TRANS ] = "jagModelViewInvTransMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_INV_TRANS_4 ] = "jagModelViewInvTrans4Matrix";
    _nameMap[ jagBase::TransformD::PROJ_INV ] = "jagProjInvMatrix";
    _nameMap[ jagBase::TransformD::VIEW_INV ] = "jagViewInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_INV ] = "jagModelInvMatrix";
    _nameMap[ jagBase::TransformD::VIEW_PROJ_INV ] = "jagViewProjInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_PROJ_INV ] = "jagModelViewProjInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_INV ] = "jagModelViewInvMatrix";
}


// jagDraw
}
