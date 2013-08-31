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

    unsigned int flags( _requiredUniforms );
    unsigned int matrixBit( 1 );
    while( flags != 0 )
    {
        if( ( flags & matrixBit ) != 0 )
        {
            // Clear the bit
            flags ^= matrixBit;

            jagDraw::UniformPtr& uniform( drawInfo.getOrCreateUniform( _nameMap[ matrixBit ] ) );
            gmtl::Matrix33f mat3;
            gmtl::Matrix44f mat4;
            if( ( matrixBit == jagBase::TransformD::MODEL_VIEW_INV_TRANS ) &&
                ( true /* !_use4x4MVIT */ ) )
            {
                gmtl::convert( mat3, _transform.getMatrix3() );
                uniform->setType( GL_FLOAT_MAT3 );
                uniform->set( mat3 );
            }
            else
            {
                gmtl::convert( mat4, _transform.getMatrix4( matrixBit ) );
                uniform->setType( GL_FLOAT_MAT4 );
                uniform->set( mat4 );
            }
            uniform->executeWithoutMap( drawInfo );
        }
        matrixBit <<= 1;
    }
    _transform.setDirty( 0 );

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
    _nameMap[ jagBase::TransformD::VIEW_PROJ ] = "jagViewProjMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_PROJ ] = "jagModelViewProjMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW ] = "jagModelViewMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_INV_TRANS ] = "jagModelViewInvTransMatrix";
    _nameMap[ jagBase::TransformD::PROJ_INV ] = "jagProjInvMatrix";
    _nameMap[ jagBase::TransformD::VIEW_INV ] = "jagViewInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_INV ] = "jagModelInvMatrix";
    _nameMap[ jagBase::TransformD::VIEW_PROJ_INV ] = "jagViewProjInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_PROJ_INV ] = "jagModelViewProjInvMatrix";
    _nameMap[ jagBase::TransformD::MODEL_VIEW_INV ] = "jagModelViewInvMatrix";
}


// jagDraw
}
