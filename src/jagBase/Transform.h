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

This class is not thread safe. When used for rendering, one Transform instance should
be created per rendering thread.
*/
template< class MXTYPE >
class Transform
{
public:
    Transform()
      : _dirty( 0 )
    {
    }
    Transform( const Transform& rhs )
      : _proj( rhs._proj ),
        _view( rhs._view ),
        _model( rhs._model ),
        _dirty( rhs._dirty )
    {
    }
    ~Transform()
    {
    }


    void setProj( const MXTYPE& proj )
    {
        _proj = proj;
        _dirty |= ALL_PROJ_DIRTY;
    }
    void setView( const MXTYPE& view )
    {
        _view = view;
        _dirty |= ALL_VIEW_DIRTY;
    }
    void setModel( const MXTYPE& model )
    {
        _model = model;
        _dirty |= ALL_MODEL_DIRTY;
    }

    const MXTYPE& getProj()
    {
        return( _proj );
    }
    const MXTYPE& getView()
    {
        return( _view );
    }
    const MXTYPE& getModel()
    {
        return( _model );
    }

    const MXTYPE& getViewProj()
    {
        if( _dirty & VIEW_PROJ_DIRTY )
        {
            _viewProj = _proj * _view;
            _dirty &= ~VIEW_PROJ_DIRTY;
        }
        return( _viewProj );
    }
    const MXTYPE& getModelViewProj()
    {
        if( _dirty & MODEL_VIEW_PROJ_DIRTY )
        {
            const MXTYPE vp( getViewProj() );
            _modelViewProj = vp * _model;
            _dirty &= ~MODEL_VIEW_PROJ_DIRTY;
        }
        return( _modelViewProj );
    }
    const MXTYPE& getModelView()
    {
        if( _dirty & MODEL_VIEW_DIRTY )
        {
            _modelView = _view * _model;
            _dirty &= ~MODEL_VIEW_DIRTY;
        }
        return( _modelView );
    }
    const MXTYPE& getModelViewInvTrans()
    {
        if( _dirty & MODEL_VIEW_INV_TRANS_DIRTY )
        {
            const MXTYPE mvInv( getModelViewInv() );
            _modelViewInvTrans = gmtl::transpose( mvInv );
            _dirty &= ~MODEL_VIEW_INV_TRANS_DIRTY;
        }
        return( _modelViewInvTrans );
    }

    const MXTYPE& getProjInv()
    {
        if( _dirty & PROJ_INV_DIRTY )
        {
            gmtl::invert( _projInv, _proj );
            _dirty &= ~PROJ_INV_DIRTY;
        }
        return( _projInv );
    }
    const MXTYPE& getViewInv()
    {
        if( _dirty & VIEW_INV_DIRTY )
        {
            gmtl::invert( _viewInv, _view );
            _dirty &= ~VIEW_INV_DIRTY;
        }
        return( _viewInv );
    }
    const MXTYPE& getModelInv()
    {
        if( _dirty & MODEL_INV_DIRTY )
        {
            gmtl::invert( _modelInv, _model );
            _dirty &= ~MODEL_INV_DIRTY;
        }
        return( _modelInv );
    }
    const MXTYPE& getViewProjInv()
    {
        if( _dirty & VIEW_PROJ_INV_DIRTY )
        {
            gmtl::invert( _viewProjInv, getViewProj() );
            _dirty &= ~VIEW_PROJ_INV_DIRTY;
        }
        return( _viewProjInv );
    }
    const MXTYPE& getModelViewProjInv()
    {
        if( _dirty & MODEL_VIEW_PROJ_INV_DIRTY )
        {
            gmtl::invert( _modelViewProjInv, getModelViewProj() );
            _dirty &= ~MODEL_VIEW_PROJ_INV_DIRTY;
        }
        return( _modelViewProjInv );
    }
    const MXTYPE& getModelViewInv()
    {
        if( _dirty & MODEL_VIEW_INV_DIRTY )
        {
            gmtl::invert( _modelViewInv, getModelView() );
            _dirty &= ~MODEL_VIEW_INV_DIRTY;
        }
        return( _modelViewInv );
    }

protected:
    MXTYPE _proj;
    MXTYPE _view;
    MXTYPE _model;

    MXTYPE _viewProj;
    MXTYPE _modelViewProj;
    MXTYPE _modelView;
    MXTYPE _modelViewInvTrans;

    MXTYPE _projInv;
    MXTYPE _viewInv;
    MXTYPE _modelInv;
    MXTYPE _viewProjInv;
    MXTYPE _modelViewProjInv;
    MXTYPE _modelViewInv;

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
                            MODEL_VIEW_INV_DIRTY )
    };

    unsigned int _dirty;
};

typedef Transform< gmtl::Matrix44f > Transform44f;
typedef Transform< gmtl::Matrix44d > Transform44d;


// jagBase
}


// __JAGBASE_TRANSFORM_H__
#endif
