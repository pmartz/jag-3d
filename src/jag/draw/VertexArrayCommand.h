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

#ifndef __JAG_DRAW_VERTEX_ARRAY_COMMAND_H__
#define __JAG_DRAW_VERTEX_ARRAY_COMMAND_H__ 1

#include <jag/draw/Export.h>
#include <jag/base/UserDataOwner.h>
#include <jag/base/ptr.h>

#include <vector>


namespace jag {
namespace draw {


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

struct DrawInfo;


/** \class VertexArrayCommand VertexArrayCommand.h <jag/draw/VertexArrayCommand.h>
\brief
\details
*/
class /*JAGDRAW_EXPORT*/ VertexArrayCommand : public jag::base::UserDataOwner
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

typedef jag::base::ptr< jag::draw::VertexArrayCommand >::shared_ptr VertexArrayCommandPtr;
typedef std::vector< VertexArrayCommandPtr > VertexArrayCommandVec;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_VERTEX_ARRAY_COMMAND_H__
#endif
