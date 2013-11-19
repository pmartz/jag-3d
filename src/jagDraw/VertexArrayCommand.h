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

#ifndef __JAGDRAW_VERTEX_ARRAY_COMMAND_H__
#define __JAGDRAW_VERTEX_ARRAY_COMMAND_H__ 1

#include <jagDraw/Export.h>
#include <jagBase/UserDataOwner.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

struct DrawInfo;


/** \class VertexArrayCommand VertexArrayCommand.h <jagDraw/VertexArrayCommand.h>
\brief
\details
*/
class /*JAGDRAW_EXPORT*/ VertexArrayCommand : public jagBase::UserDataOwner
{
public:
    /** \brief TBD
    \details TBD
    */
    typedef enum {
        BufferObject_t,
        VertexAttrib_t
    } Type;

    VertexArrayCommand( const Type type )
      : UserDataOwner(),
        _type( type )
    {
    }
    VertexArrayCommand( const VertexArrayCommand& rhs )
      : UserDataOwner( rhs ),
        _type( rhs._type )
    {
    }
    ~VertexArrayCommand()
    {
    }

    Type getType() const
    {
        return( _type );
    }

    virtual bool isSameKind( const VertexArrayCommand& rhs ) const
    {
        return( _type == rhs._type );
    }

    virtual void execute( DrawInfo& drawInfo ) = 0;

    /** \brief Detect a change in vertex array command data.
    \details This is kind of a TBD hack. There are two types of
    vertex array commands: VertexAttrib, and BufferObject. Right now,
    JAG supports only dynamic BufferObject. BufferObject overrides this
    and returns true if data is dirty on the given context.

    VertexAttrib can not override this, as VertexAttrib is not an
    ObjectID or ObjectIDOwner, and as a result has to setMaxContexts()
    support, so there's no way for it to keep a per context array of
    dirty flags. */
    virtual bool isDirty( const unsigned int contextID )
    {
        return( false );
    }

protected:
    Type _type;
};

typedef jagBase::ptr< jagDraw::VertexArrayCommand >::shared_ptr VertexArrayCommandPtr;
typedef std::vector< VertexArrayCommandPtr > VertexArrayCommandVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_VERTEX_ARRAY_COMMAND_H__
#endif
