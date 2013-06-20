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

#ifndef __JAGDRAW_TRANSFORM_CALLBACK_H__
#define __JAGDRAW_TRANSFORM_CALLBACK_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/Node.h>
#include <jagBase/Transform.h>


namespace jagDraw {


/** \struct TransformCallback TransformCallback.h <TransformCallback/Node.h>
\brief A draw node execute callback that specifies transform uniforms.
\details TBD */
struct JAGDRAW_EXPORT TransformCallback : protected jagDraw::Node::Callback
{
    TransformCallback();
    TransformCallback( const TransformCallback& rhs );
    virtual ~TransformCallback();

    virtual bool operator()( jagDraw::Node& node );
};


// jagDraw
}


// __JAGDRAW_TRANSFORM_CALLBACK_H__
#endif
