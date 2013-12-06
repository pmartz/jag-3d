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

#include <jagDraw/TransformCallback.h>
#include <jagDraw/DrawInfo.h>


namespace jagDraw {


TransformCallback::TransformCallback()
    : DrawNode::Callback(),
    _transform(),
    _requiredUniforms( jag::base::TransformD::MODEL_VIEW_PROJ |
            jag::base::TransformD::MODEL_VIEW |
            jag::base::TransformD::MODEL_VIEW_INV_TRANS )
{
    setDefaultMatrixUniformNames();
}
TransformCallback::TransformCallback( const TransformCallback& rhs )
    : DrawNode::Callback( rhs ),
    _transform( rhs._transform ),
    _requiredUniforms( rhs._requiredUniforms ),
    _nameMap( rhs._nameMap )
{
}
TransformCallback::~TransformCallback()
{
}


bool TransformCallback::operator()( jagDraw::DrawNode& node, jagDraw::DrawInfo& drawInfo )
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
            if( matrixBit != jag::base::TransformD::MODEL_VIEW_INV_TRANS )
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
    _nameMap[ jag::base::TransformD::MODEL ] = "jagModelMatrix";
    _nameMap[ jag::base::TransformD::VIEW ] = "jagViewMatrix";
    _nameMap[ jag::base::TransformD::PROJ ] = "jagProjMatrix";
    _nameMap[ jag::base::TransformD::VIEW_PROJ ] = "jagViewProjMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW_PROJ ] = "jagModelViewProjMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW ] = "jagModelViewMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW_INV_TRANS ] = "jagModelViewInvTransMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW_INV_TRANS_4 ] = "jagModelViewInvTrans4Matrix";
    _nameMap[ jag::base::TransformD::PROJ_INV ] = "jagProjInvMatrix";
    _nameMap[ jag::base::TransformD::VIEW_INV ] = "jagViewInvMatrix";
    _nameMap[ jag::base::TransformD::MODEL_INV ] = "jagModelInvMatrix";
    _nameMap[ jag::base::TransformD::VIEW_PROJ_INV ] = "jagViewProjInvMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW_PROJ_INV ] = "jagModelViewProjInvMatrix";
    _nameMap[ jag::base::TransformD::MODEL_VIEW_INV ] = "jagModelViewInvMatrix";
}


// jagDraw
}
