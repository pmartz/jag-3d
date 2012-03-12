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

#ifndef __JAGDRAW_VERTEX_ARRAY_COMMAND_H__
#define __JAGDRAW_VERTEX_ARRAY_COMMAND_H__ 1

#include <jagDraw/Export.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class VertexArrayCommand VertexArrayCommand.h <jagDraw/VertexArrayCommand.h>
\brief
\details
*/
class /*JAGDRAW_EXPORT*/ VertexArrayCommand
{
public:
    typedef enum {
        BufferObjectType,
        VertexAttribType,
        VertexArrayObjectType
    } Type;

    VertexArrayCommand( const Type type )
      : _type( type )
    {}
    VertexArrayCommand( const VertexArrayCommand& rhs )
      : _type( rhs._type )
    {}
    ~VertexArrayCommand()
    {}

    Type getType() const { return( _type ); }

    virtual void operator()( DrawInfo& drawInfo ) = 0;

protected:
    Type _type;
};

typedef jagBase::ptr< jagDraw::VertexArrayCommand >::shared_ptr VertexArrayCommandPtr;
typedef std::vector< VertexArrayCommandPtr > VertexArrayCommandList;


// jagDraw
}


// __JAGDRAW_VERTEX_ARRAY_COMMAND_H__
#endif
