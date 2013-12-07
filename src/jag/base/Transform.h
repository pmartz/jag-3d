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

#ifndef __JAG_BASE_TRANSFORM_H__
#define __JAG_BASE_TRANSFORM_H__ 1

#include <jag/base/LogMacros.h>
#include <gmtl/gmtl.h>


namespace jag {
namespace base {


/** \class Transform Transform.h <jag/base/Transform.h>
\brief A class for tracking model, view, and projection matrices.
\details Utility for storing the traditional OpenGL FFP model, view, and projection
matrices, matrices derived from them (concatenation, inverse, normal, etc), the
viewport, and a viewing frustum based on the view and projection matrices.

This class is not thread safe. When used for rendering, one Transform instance should
be created per rendering thread. */
template< typename T >
class Transform
{
public:
    typedef gmtl::Frustum< T > FTYPE;
    typedef gmtl::Matrix< T, 3, 3 > M3TYPE;
    typedef gmtl::Matrix< T, 4, 4 > M4TYPE;


    Transform()
      : _vpX( 0 ),
        _vpY( 0 ),
        _vpWidth( 0 ),
        _vpHeight( 0 ),
        _dirty( ALL_MATRICES )
    {
    }
    Transform( const Transform& rhs )
      : _frustum( rhs._frustum ),

        _proj( rhs._proj ),
        _view( rhs._view ),
        _model( rhs._model ),

        _viewProj( rhs._viewProj ),
        _modelViewProj( rhs._modelViewProj ),
        _modelView( rhs._modelView ),
        _modelViewInvTrans( rhs._modelViewInvTrans ),
        _modelViewInvTrans4( rhs._modelViewInvTrans4 ),

        _projInv( rhs._projInv ),
        _viewInv( rhs._viewInv ),
        _modelInv( rhs._modelInv ),
        _viewProjInv( rhs._viewProjInv ),
        _modelViewProjInv( rhs._modelViewProjInv ),
        _modelViewInv( rhs._modelViewInv ),

        _vpX( rhs._vpX ),
        _vpY( rhs._vpY ),
        _vpWidth( rhs._vpWidth ),
        _vpHeight( rhs._vpHeight ),

        _dirty( rhs._dirty )
    {
    }
    ~Transform()
    {
    }


    enum {
        PROJ = 0x1 << 0,
        VIEW = 0x1 << 1,
        MODEL = 0x1 << 2,
        VIEW_PROJ = 0x1 << 3,
        MODEL_VIEW_PROJ = 0x1 << 4,
        MODEL_VIEW = 0x1 << 5,
        MODEL_VIEW_INV_TRANS = 0x1 << 6,
        MODEL_VIEW_INV_TRANS_4 = 0x1 << 7,
        PROJ_INV = 0x1 << 8,
        VIEW_INV = 0x1 << 9,
        MODEL_INV = 0x1 << 10,
        VIEW_PROJ_INV = 0x1 << 11,
        MODEL_VIEW_PROJ_INV = 0x1 << 12,
        MODEL_VIEW_INV = 0x1 << 13,

        ALL_PROJ = ( /* PROJ | */
                     VIEW_PROJ |
                     MODEL_VIEW_PROJ |
                     PROJ_INV |
                     VIEW_PROJ_INV |
                     MODEL_VIEW_PROJ_INV ),

        ALL_VIEW = ( /* VIEW | */
                     VIEW_PROJ |
                     MODEL_VIEW_PROJ |
                     MODEL_VIEW |
                     MODEL_VIEW_INV_TRANS |
                     MODEL_VIEW_INV_TRANS_4 |
                     VIEW_INV |
                     VIEW_PROJ_INV |
                     MODEL_VIEW_PROJ_INV |
                     MODEL_VIEW_INV ),

        ALL_MODEL = ( /* MODEL | */
                      MODEL_VIEW_PROJ |
                      MODEL_VIEW |
                      MODEL_VIEW_INV_TRANS |
                      MODEL_VIEW_INV_TRANS_4 |
                      MODEL_INV |
                      MODEL_VIEW_PROJ_INV |
                      MODEL_VIEW_INV ),

        ALL_MATRICES = ( ALL_PROJ |
                         ALL_VIEW |
                         ALL_MODEL )
    };

    unsigned int getDirty()
    {
        return( _dirty );
    }
    void setDirty( unsigned int dirty=ALL_DIRTY )
    {
        _dirty |= dirty;
    }


    void setProj( const M4TYPE& proj )
    {
        _proj = proj;
        _dirty |= ALL_PROJ;
        _frustum.extractPlanes( getModelView(), _proj );
    }
    void setView( const M4TYPE& view )
    {
        _view = view;
        _dirty |= ALL_VIEW;
        _frustum.extractPlanes( getModelView(), _proj );
    }
    void setModel( const M4TYPE& model )
    {
        _model = model;
        _dirty |= ALL_MODEL;
        _frustum.extractPlanes( getModelView(), _proj );
    }
    void setViewport( const int x, const int y, const int width, const int height )
    {
        _vpX = x;
        _vpY = y;
        _vpWidth = width;
        _vpHeight = height;
    }



    /** \brief Get any matrix as a 4x4 matrix.
    \details Provides access to all stored matrices, supported with
    an internal switch. If MODEL_VIEW_INV_TRANS is requested, the 4x4
    matrix with translation zeroed is returned. */
    const M4TYPE& getMatrix4( const unsigned int matrix )
    {
        switch( matrix )
        {
        case PROJ:
            return( getProj() );
            break;
        case VIEW:
            return( getView() );
            break;
        case MODEL:
            return( getModel() );
            break;
        case VIEW_PROJ:
            return( getViewProj() );
            break;
        default:
            JAG3D_WARNING_STATIC( "jag.base.transform", "getMatrix4(): Invalid matrix parameter." );
            // Intentional fallthrough
        case MODEL_VIEW_PROJ:
            return( getModelViewProj() );
            break;
        case MODEL_VIEW:
            return( getModelView() );
            break;
        case MODEL_VIEW_INV_TRANS:
            // App code asked for a 4x4 matrix, but passed the 3x3 mvit bit.
            JAG3D_WARNING_STATIC( "jag.base.transform", "getMatrix4() called for 3x3 MODEL_VIEW_INV_TRANS." );
            // Intentional fallthrough
        case MODEL_VIEW_INV_TRANS_4:
            return( getModelViewInvTrans4() );
            break;
        case PROJ_INV:
            return( getProjInv() );
            break;
        case VIEW_INV:
            return( getViewInv() );
            break;
        case MODEL_INV:
            return( getModelInv() );
            break;
        case VIEW_PROJ_INV:
            return( getViewProjInv() );
            break;
        case MODEL_VIEW_PROJ_INV:
            return( getModelViewProjInv() );
            break;
        case MODEL_VIEW_INV:
            return( getModelViewInv() );
            break;
        }
    }
    /** \brief Returns the 3x3 MODEL_VIEW_INV_TRANS matrix.
    \details Requesting any other matrix will generate a warning. In the
    furure, this function could support returning any matrix as orientation-only
    (translation zeroed), but this is not currently supported. */
    const M3TYPE& getMatrix3( const unsigned int matrix=MODEL_VIEW_INV_TRANS )
    {
        if( matrix != MODEL_VIEW_INV_TRANS )
        {
            JAG3D_WARNING_STATIC( "jag.base.transform", "getMatrix3(): matrix != MODEL_VIEW_INV_TRANS" );
        }
        return( getModelViewInvTrans() );
    }

    const FTYPE& getFrustum()
    {
        return( _frustum );
    }
    const M4TYPE& getProj()
    {
        return( _proj );
    }
    const M4TYPE& getView()
    {
        return( _view );
    }
    const M4TYPE& getModel()
    {
        return( _model );
    }
    void getViewport( int& x, int& y, int& width, int& height )
    {
        x = _vpX;
        y = _vpY;
        width = _vpWidth;
        height = _vpHeight;
    }

    const M4TYPE& getViewProj()
    {
        if( _dirty & VIEW_PROJ )
        {
            _viewProj = _proj * _view;
            _dirty &= ~VIEW_PROJ;
        }
        return( _viewProj );
    }
    const M4TYPE& getModelViewProj()
    {
        if( _dirty & MODEL_VIEW_PROJ )
        {
            _modelViewProj = _proj * getModelView();
            _dirty &= ~MODEL_VIEW_PROJ;
        }
        return( _modelViewProj );
    }
    const M4TYPE& getModelView()
    {
        if( _dirty & MODEL_VIEW )
        {
            _modelView = _view * _model;
            _dirty &= ~MODEL_VIEW;
        }
        return( _modelView );
    }
    const M3TYPE& getModelViewInvTrans()
    {
        if( _dirty & MODEL_VIEW_INV_TRANS )
        {
            const M4TYPE& m( getModelViewInv() );
            // Transpose during set():
            _modelViewInvTrans.set(  m( 0, 0 ),  m( 1, 0 ),  m( 2, 0 ),
                                     m( 0, 1 ),  m( 1, 1 ),  m( 2, 1 ),
                                     m( 0, 2 ),  m( 1, 2 ),  m( 2, 2 )  );
            _dirty &= ~MODEL_VIEW_INV_TRANS;
        }
        return( _modelViewInvTrans );
    }
    /** \brief Get the 4x4 version of the MODEL_VIEW_INV_TRANS matrix.
    \details Normally a 3x3 matrix, this function returns MODEL_VIEW_INV_TRANS
    as a 4x4 matrix with final row and column set to zero and final element
    set to 1. */
    const M4TYPE& getModelViewInvTrans4()
    {
        if( _dirty & MODEL_VIEW_INV_TRANS_4 )
        {
            gmtl::transpose( _modelViewInvTrans4, getModelViewInv() );
            _modelViewInvTrans4( 3, 0 ) =
            _modelViewInvTrans4( 3, 1 ) =
            _modelViewInvTrans4( 3, 2 ) =
            _modelViewInvTrans4( 0, 3 ) =
            _modelViewInvTrans4( 1, 3 ) =
            _modelViewInvTrans4( 2, 3 ) = 0.;
            _modelViewInvTrans4( 3, 3 ) = 1.;
            _dirty &= ~MODEL_VIEW_INV_TRANS_4;
        }
        return( _modelViewInvTrans4 );
    }

    const M4TYPE& getProjInv()
    {
        if( _dirty & PROJ_INV )
        {
            gmtl::invert( _projInv, _proj );
            _dirty &= ~PROJ_INV;
        }
        return( _projInv );
    }
    const M4TYPE& getViewInv()
    {
        if( _dirty & VIEW_INV )
        {
            gmtl::invert( _viewInv, _view );
            _dirty &= ~VIEW_INV;
        }
        return( _viewInv );
    }
    const M4TYPE& getModelInv()
    {
        if( _dirty & MODEL_INV )
        {
            gmtl::invert( _modelInv, _model );
            _dirty &= ~MODEL_INV;
        }
        return( _modelInv );
    }
    const M4TYPE& getViewProjInv()
    {
        if( _dirty & VIEW_PROJ_INV )
        {
            gmtl::invert( _viewProjInv, getViewProj() );
            _dirty &= ~VIEW_PROJ_INV;
        }
        return( _viewProjInv );
    }
    const M4TYPE& getModelViewProjInv()
    {
        if( _dirty & MODEL_VIEW_PROJ_INV )
        {
            gmtl::invert( _modelViewProjInv, getModelViewProj() );
            _dirty &= ~MODEL_VIEW_PROJ_INV;
        }
        return( _modelViewProjInv );
    }
    const M4TYPE& getModelViewInv()
    {
        if( _dirty & MODEL_VIEW_INV )
        {
            gmtl::invert( _modelViewInv, getModelView() );
            _dirty &= ~MODEL_VIEW_INV;
        }
        return( _modelViewInv );
    }

protected:
    FTYPE _frustum;

    M4TYPE _proj;
    M4TYPE _view;
    M4TYPE _model;

    M4TYPE _viewProj;
    M4TYPE _modelViewProj;
    M4TYPE _modelView;
    M3TYPE _modelViewInvTrans;
    M4TYPE _modelViewInvTrans4;

    M4TYPE _projInv;
    M4TYPE _viewInv;
    M4TYPE _modelInv;
    M4TYPE _viewProjInv;
    M4TYPE _modelViewProjInv;
    M4TYPE _modelViewInv;

    int _vpX, _vpY, _vpWidth, _vpHeight;

    unsigned int _dirty;
};

typedef Transform< float > TransformF;
typedef Transform< double > TransformD;


// namespace jag::base::
}
}


// __JAG_BASE_TRANSFORM_H__
#endif
