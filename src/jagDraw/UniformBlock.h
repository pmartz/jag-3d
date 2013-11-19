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

#ifndef __JAGDRAW_UNIFORM_BLOCK_H__
#define __JAGDRAW_UNIFORM_BLOCK_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/Program.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/DrawablePrep.h>
#include <jagBase/LogBase.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Shader.h>
#include <jagDraw/CommandMap.h>
#include <jagBase/ptr.h>

#include <map>
#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

struct DrawInfo;

class UniformBlock;
typedef jagBase::ptr< jagDraw::UniformBlock >::shared_ptr UniformBlockPtr;

/** \class UniformBlock UniformBlock.h <jagDraw/UniformBlock.h>
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
class JAGDRAW_EXPORT UniformBlock : public DrawablePrep,
            public ObjectIDOwner, public SHARED_FROM_THIS(UniformBlock),
            protected jagBase::LogBase
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
    virtual DrawablePrepPtr clone() const { return( UniformBlockPtr( new UniformBlock( *this ) ) ); }

    /** \brief TBD
    \details TBD
    Does not add this uniform block to drawInfo._uniformBlockMap. */
    void execute( DrawInfo& drawInfo, const Program::BlockInfo& blockInfo ) const;
    /** \brief TBD
    \details Override methods from DrawablePrep.
    Adds this uniform to drawInfo._uniformBlockMap. */
    virtual void execute( DrawInfo& drawInfo );

    /** Override methods from ObjectIDOwner */
    virtual void setMaxContexts( const unsigned int numContexts );
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    std::string _name;
    Program::HashValue _nameHash;

    UniformVec _uniforms;

    UniformBufferPtr _bufferObject;
    jagBase::BufferPtr _buffer;

    mutable bool _dirty;
};

typedef jagBase::ptr< const jagDraw::UniformBlock >::shared_ptr ConstUniformBlockPtr;
typedef std::vector< UniformBlockPtr > UniformBlockVec;



class UniformBlockSet;
typedef jagBase::ptr< jagDraw::UniformBlockSet >::shared_ptr UniformBlockSetPtr;

class UniformBlockSet : public ObjectIDOwner,
        public DrawablePrepSet< Program::HashValue, UniformBlockPtr, UniformBlockSet, UniformBlockSetPtr >
{
protected:
    typedef DrawablePrepSet< Program::HashValue, UniformBlockPtr, UniformBlockSet, UniformBlockSetPtr > SET_TYPE;

public:
    UniformBlockSet()
        : ObjectIDOwner(),
        SET_TYPE( DrawablePrep::UniformBlockSet_t )
    {}
    UniformBlockSet( const UniformBlockSet& rhs )
        : ObjectIDOwner( rhs ),
        SET_TYPE( rhs )
    {}
    ~UniformBlockSet()
    {}


    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const
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
    virtual void deleteID( const jagDraw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->deleteID( contextID );
        }
    }
};


/*@}*/


// jagDraw
}


// __JAGDRAW_UNIFORM_BLOCK_H__
#endif
