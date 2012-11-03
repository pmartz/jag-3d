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

#ifndef __JAGBASE_TRANSFORM_H__
#define __JAGBASE_TRANSFORM_H__ 1

#include <gmtl/gmtl.h>


namespace jagBase {


/** \class Transform Transform.h <jagBase/Transform.h>
\brief A class for tracking model, view, and projection matrices.
\details Utility for storing the traditional OpenGL FFP model, view, and projection
matrices, as well as matrices derived from them (concatenation, inverse, normal, etc).

The class also tracks a viewing frustum based on the view and projection matrices.
In the future, this class should also store a viewport so that it can compute
pixel sizes from world coordinate metrics.

This class is not thread safe. When used for rendering, one Transform instance should
be created per rendering thread.
*/
template< typename T >
class Transform
{
protected:
    typedef gmtl::Frustum< T > FTYPE;
    typedef gmtl::Matrix< T, 3, 3 > M3TYPE;
    typedef gmtl::Matrix< T, 4, 4 > M4TYPE;

public:
    Transform()
      : _dirty( ALL_DIRTY )
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

        _projInv( rhs._projInv ),
        _viewInv( rhs._viewInv ),
        _modelInv( rhs._modelInv ),
        _viewProjInv( rhs._viewProjInv ),
        _modelViewProjInv( rhs._modelViewProjInv ),
        _modelViewInv( rhs._modelViewInv ),

        _dirty( rhs._dirty )
    {
    }
    ~Transform()
    {
    }


    void setProj( const M4TYPE& proj )
    {
        _proj = proj;
        _frustum.extractPlanes( _view, _proj );
        _dirty |= ALL_PROJ_DIRTY;
    }
    void setView( const M4TYPE& view )
    {
        _view = view;
        _frustum.extractPlanes( _view, _proj );
        _dirty |= ALL_VIEW_DIRTY;
    }
    void setModel( const M4TYPE& model )
    {
        _model = model;
        _dirty |= ALL_MODEL_DIRTY;
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

    const M4TYPE& getViewProj()
    {
        if( _dirty & VIEW_PROJ_DIRTY )
        {
            _viewProj = _proj * _view;
            _dirty &= ~VIEW_PROJ_DIRTY;
        }
        return( _viewProj );
    }
    const M4TYPE& getModelViewProj()
    {
        if( _dirty & MODEL_VIEW_PROJ_DIRTY )
        {
            const M4TYPE& vp( getViewProj() );
            _modelViewProj = vp * _model;
            _dirty &= ~MODEL_VIEW_PROJ_DIRTY;
        }
        return( _modelViewProj );
    }
    const M4TYPE& getModelView()
    {
        if( _dirty & MODEL_VIEW_DIRTY )
        {
            _modelView = _view * _model;
            _dirty &= ~MODEL_VIEW_DIRTY;
        }
        return( _modelView );
    }
    const M3TYPE& getModelViewInvTrans()
    {
        if( _dirty & MODEL_VIEW_INV_TRANS_DIRTY )
        {
            M4TYPE m( getModelViewInv() );
            // Transpose during set():
            _modelViewInvTrans.set(  m( 0, 0 ),  m( 1, 0 ),  m( 2, 0 ),
                                     m( 0, 1 ),  m( 1, 1 ),  m( 2, 1 ),
                                     m( 0, 2 ),  m( 1, 2 ),  m( 2, 2 )  );
            _dirty &= ~MODEL_VIEW_INV_TRANS_DIRTY;
        }
        return( _modelViewInvTrans );
    }

    const M4TYPE& getProjInv()
    {
        if( _dirty & PROJ_INV_DIRTY )
        {
            gmtl::invert( _projInv, _proj );
            _dirty &= ~PROJ_INV_DIRTY;
        }
        return( _projInv );
    }
    const M4TYPE& getViewInv()
    {
        if( _dirty & VIEW_INV_DIRTY )
        {
            gmtl::invert( _viewInv, _view );
            _dirty &= ~VIEW_INV_DIRTY;
        }
        return( _viewInv );
    }
    const M4TYPE& getModelInv()
    {
        if( _dirty & MODEL_INV_DIRTY )
        {
            gmtl::invert( _modelInv, _model );
            _dirty &= ~MODEL_INV_DIRTY;
        }
        return( _modelInv );
    }
    const M4TYPE& getViewProjInv()
    {
        if( _dirty & VIEW_PROJ_INV_DIRTY )
        {
            gmtl::invert( _viewProjInv, getViewProj() );
            _dirty &= ~VIEW_PROJ_INV_DIRTY;
        }
        return( _viewProjInv );
    }
    const M4TYPE& getModelViewProjInv()
    {
        if( _dirty & MODEL_VIEW_PROJ_INV_DIRTY )
        {
            gmtl::invert( _modelViewProjInv, getModelViewProj() );
            _dirty &= ~MODEL_VIEW_PROJ_INV_DIRTY;
        }
        return( _modelViewProjInv );
    }
    const M4TYPE& getModelViewInv()
    {
        if( _dirty & MODEL_VIEW_INV_DIRTY )
        {
            gmtl::invert( _modelViewInv, getModelView() );
            _dirty &= ~MODEL_VIEW_INV_DIRTY;
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

    M4TYPE _projInv;
    M4TYPE _viewInv;
    M4TYPE _modelInv;
    M4TYPE _viewProjInv;
    M4TYPE _modelViewProjInv;
    M4TYPE _modelViewInv;

    enum {
        VIEW_PROJ_DIRTY = 0x1 << 0,
        MODEL_VIEW_PROJ_DIRTY = 0x1 << 1,
        MODEL_VIEW_DIRTY = 0x1 << 2,
        MODEL_VIEW_INV_TRANS_DIRTY = 0x1 << 3,
        PROJ_INV_DIRTY = 0x1 << 4,
        VIEW_INV_DIRTY = 0x1 << 5,
        MODEL_INV_DIRTY = 0x1 << 6,
        VIEW_PROJ_INV_DIRTY = 0x1 << 7,
        MODEL_VIEW_PROJ_INV_DIRTY = 0x1 << 8,
        MODEL_VIEW_INV_DIRTY = 0x1 << 9,

        ALL_PROJ_DIRTY = ( VIEW_PROJ_DIRTY |
                           MODEL_VIEW_PROJ_DIRTY |
                           PROJ_INV_DIRTY |
                           VIEW_PROJ_INV_DIRTY |
                           MODEL_VIEW_PROJ_INV_DIRTY ),

        ALL_VIEW_DIRTY = ( VIEW_PROJ_DIRTY |
                           MODEL_VIEW_PROJ_DIRTY |
                           MODEL_VIEW_DIRTY |
                           MODEL_VIEW_INV_TRANS_DIRTY |
                           VIEW_INV_DIRTY |
                           VIEW_PROJ_INV_DIRTY |
                           MODEL_VIEW_PROJ_INV_DIRTY |
                           MODEL_VIEW_INV_DIRTY ),

        ALL_MODEL_DIRTY = ( MODEL_VIEW_PROJ_DIRTY |
                            MODEL_VIEW_DIRTY |
                            MODEL_VIEW_INV_TRANS_DIRTY |
                            MODEL_INV_DIRTY |
                            MODEL_VIEW_PROJ_INV_DIRTY |
                            MODEL_VIEW_INV_DIRTY ),

        ALL_DIRTY = ( ALL_PROJ_DIRTY |
                      ALL_VIEW_DIRTY |
                      ALL_MODEL_DIRTY )
    };

    unsigned int _dirty;
};

typedef Transform< float > TransformF;
typedef Transform< double > TransformD;


// jagBase
}


// __JAGBASE_TRANSFORM_H__
#endif
