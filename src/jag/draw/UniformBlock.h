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

#ifndef __JAG_DRAW_UNIFORM_BLOCK_H__
#define __JAG_DRAW_UNIFORM_BLOCK_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/Program.h>
#include <jag/draw/Uniform.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/Command.h>
#include <jag/base/LogBase.h>
#include <jag/draw/ObjectID.h>
#include <jag/draw/Shader.h>
#include <jag/draw/CommandMap.h>
#include <jag/base/ptr.h>

#include <map>
#include <vector>


namespace jag {
namespace draw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

struct DrawInfo;

class UniformBlock;
typedef jag::base::ptr< jag::draw::UniformBlock >::shared_ptr UniformBlockPtr;

/** \class UniformBlock UniformBlock.h <jag/draw/UniformBlock.h>
\brief Support for GLSL interface blocks / named uniform blocks.
\details

Future enhancements:

Requires efficiency enhancements. If the attached Uniform values
have not changed, there is no need to copy their values into the
buffer. But when the values have changes, UniformBlock should
map the buffer and modify the values.

\gl{section 2.9} \gl{section 2.11.4}.

\specBegin

\specTableBegin
\specLog{jag.draw.ublock}
\specTableEnd

\specEnd
*/
class JAGDRAW_EXPORT UniformBlock : public Command,
            public ObjectIDOwner, public SHARED_FROM_THIS(UniformBlock),
            protected jag::base::LogBase
{
public:
    UniformBlock( const std::string& name, const std::string& logName=std::string( "" ) );
    UniformBlock( const UniformBlock& rhs );
    virtual ~UniformBlock();

    /** \brief TBD
    \details TBD */
    std::string getName() const;
    /** \brief TBD
    \details TBD */
    Program::HashValue getNameHash() const;

    /** \brief TBD
    \details TBD */
    void addUniform( UniformPtr uniform );

    /** \brief TBD
    \details TBD */
    bool empty() const
    {
        return( _uniforms.empty() );
    }

    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const { return( UniformBlockPtr( new UniformBlock( *this ) ) ); }

    /** \brief TBD
    \details TBD
    Does not add this uniform block to drawInfo._uniformBlockMap. */
    void execute( DrawInfo& drawInfo, const Program::BlockInfo& blockInfo ) const;
    /** \brief TBD
    \details Override methods from Command.
    Adds this uniform to drawInfo._uniformBlockMap. */
    virtual void execute( DrawInfo& drawInfo );

    /** Override methods from ObjectIDOwner */
    virtual void setMaxContexts( const unsigned int numContexts );
    virtual void deleteID( const jag::draw::jagDrawContextID contextID );

protected:
    std::string _name;
    Program::HashValue _nameHash;

    UniformVec _uniforms;

    UniformBufferPtr _bufferObject;
    jag::base::BufferPtr _buffer;

    mutable bool _dirty;
};

typedef jag::base::ptr< const jag::draw::UniformBlock >::shared_ptr ConstUniformBlockPtr;
typedef std::vector< UniformBlockPtr > UniformBlockVec;



class UniformBlockSet;
typedef jag::base::ptr< jag::draw::UniformBlockSet >::shared_ptr UniformBlockSetPtr;

class UniformBlockSet : public ObjectIDOwner,
        public CommandSet< Program::HashValue, UniformBlockPtr, UniformBlockSet, UniformBlockSetPtr >
{
protected:
    typedef CommandSet< Program::HashValue, UniformBlockPtr, UniformBlockSet, UniformBlockSetPtr > SET_TYPE;

public:
    UniformBlockSet()
        : ObjectIDOwner(),
        SET_TYPE( Command::UniformBlockSet_t )
    {}
    UniformBlockSet( const UniformBlockSet& rhs )
        : ObjectIDOwner( rhs ),
        SET_TYPE( rhs )
    {}
    ~UniformBlockSet()
    {}


    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const
    {
        return( UniformBlockSetPtr( new UniformBlockSet( *this ) ) );
    }

    /** \brief TBD
    \details TBD */
    void insert( UniformBlockPtr ubp )
    {
        operator[]( ubp->getNameHash() ) = ubp;
    }

    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->setMaxContexts( numContexts );
        }
    }
    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void deleteID( const jag::draw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->deleteID( contextID );
        }
    }
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_UNIFORM_BLOCK_H__
#endif
