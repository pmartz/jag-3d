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

#ifndef __JAG_DRAW_UNIFOR_H__
#define __JAG_DRAW_UNIFOR_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/Command.h>
#include <jag/draw/Program.h>
#include <jag/base/LogBase.h>
#include <jag/draw/CommandMap.h>
#include <jag/base/ptr.h>
#include <gmtl/gmtl.h>

#include <boost/any.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <string>


namespace jag {
namespace draw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

struct DrawInfo;

class Uniform;
typedef jag::base::ptr< jag::draw::Uniform >::shared_ptr UniformPtr;


/** \class Uniform Uniform.h <jagDraw\Uniform.h>
\brief Support for setting GLSL uniform variables.
\details

Future enhancements:

Requires support for arrays.

Requires efficiency enhancements. Consider the following scenarios:

\code
  frame {
    bind program A
    set uniform
    draw
    bind program B
    draw
  }
\endcode

In the above scenario, Uniform::execute() should result in a
call to glUniform*() only in the first frame, but not subsequent
frames because the uniform value is automatically restored when
program A is re-bound.

(However, note that glUniform*() must be called if the uniform value
changes, or if program A is re-linked.)

\code
  frame {
    bind program
    set value X for uniform "name"
    draw
    set value Y for uniform "name"
    draw
  }
\endcode

Jag3D supports multiple Uniform instances that all have the same name,
and therefore all map to the same GLSL uniform variable. This is the
case in the above scenario. In such a scenario, the two separate Uniform
instances must always call glUniform*() to set their different values.
Currently this already happens, becahse Uniform::execute() is dumb and
simply always sets its value. This will change in the future when
Uniform becomes "smarter", but we must keep this scenario in mind when
we make that change.

To be done: Constructors of classes deriving from jag::base::LogBase should
take an optional \c logName parameter to allow overriding the default
logger name. This is currently not implemented for the Uniform class.

\gl{section 2.11.4}.
*/
class JAGDRAW_EXPORT Uniform : public Command, protected jag::base::LogBase,
        public SHARED_FROM_THIS(Uniform)
{
public:
    /** \brief Default constructor. Name, type, and value must be specified explicitly. */
    Uniform();
    /** \brief Constructor, specified uniform name. */
    Uniform( const std::string& name );
    /** \brief Constructor, specified uniform name and type.
    \details \param type from OpenGL spec table 2.10. */
    explicit Uniform( const std::string& name, const GLenum type );
    /** \brief Constructor, specifically for creation of sampler uniforms.
    \details \param type from OpenGL spec table 2.10.
    \param v Sampler texture unit. */
    explicit Uniform( const std::string& name, const GLenum type, const GLint& v );
    /** \brief Copy constructor. */
    Uniform( const Uniform& rhs );
    /** \brief Destructor. */
    virtual ~Uniform();


    /** \brief TBD
    \details TBD */
    void setName( const std::string& name );
    /** \brief TBD
    \details TBD */
    std::string getName();


    /** \brief Set the value of a sampler uniform.
    \details It's also acceptable to use the set(GLint) method. */
    void setSampler( const GLint& v );


    /** \brief Copy the uniform value to the destination address.
    \details Used by UniformBlock to copy a uniform value into
    a mapped buffer object. */
    void copyValue( void* address );


    /** \def UNIFORM_OF_TYPE
    \brief Declares Uniform class methods for various data types.
    \details For each type of uniform variable, the UNIFORM_OF_TYPE macro
    declares an explicit constructor and set/get accessors.

    Note that UNIFORM_OF_TYPE does not need to be invoked for GL_SAMPLER*
    data types, as these all use the UNIFORM_OF_TYPE(GLint) invocation,
    and can also use the constructors that take a type and the setSampler()
    method. */
    /** \func set(GLint)
    \brief Sets a GL_INT or sampler-type uniform value.
    \details If the \c _type variable is GL_INT, or if _isSampler is true,
    this method sets the uniform value. Otherwise it logs an error and returns.
    */
#define UNIFORM_OF_TYPE(__type) \
    explicit Uniform( const std::string& name, const __type& v ); \
    void set( const __type& v ); \
    void get( __type& v );


    UNIFORM_OF_TYPE( bool );
    UNIFORM_OF_TYPE( GLint );
    UNIFORM_OF_TYPE( GLuint );
    UNIFORM_OF_TYPE( GLfloat );
#ifdef GL_VERSION_4_0
    UNIFORM_OF_TYPE( GLdouble );
#endif

    UNIFORM_OF_TYPE( gmtl::Point2i );
    UNIFORM_OF_TYPE( gmtl::Point3i );
    UNIFORM_OF_TYPE( gmtl::Point4i );

    UNIFORM_OF_TYPE( gmtl::Point2ui );
    UNIFORM_OF_TYPE( gmtl::Point3ui );
    UNIFORM_OF_TYPE( gmtl::Point4ui );

    UNIFORM_OF_TYPE( gmtl::Point2f );
    UNIFORM_OF_TYPE( gmtl::Point3f );
    UNIFORM_OF_TYPE( gmtl::Point4f );

#ifdef GL_VERSION_4_0
    UNIFORM_OF_TYPE( gmtl::Point2d );
    UNIFORM_OF_TYPE( gmtl::Point3d );
    UNIFORM_OF_TYPE( gmtl::Point4d );
#endif

    UNIFORM_OF_TYPE( gmtl::Matrix22f );
    UNIFORM_OF_TYPE( gmtl::Matrix33f );
    UNIFORM_OF_TYPE( gmtl::Matrix44f );

    UNIFORM_OF_TYPE( gmtl::Matrix23f );
    UNIFORM_OF_TYPE( gmtl::Matrix24f );
    UNIFORM_OF_TYPE( gmtl::Matrix32f );
    UNIFORM_OF_TYPE( gmtl::Matrix34f );
    UNIFORM_OF_TYPE( gmtl::Matrix42f );
    UNIFORM_OF_TYPE( gmtl::Matrix43f );

#ifdef GL_VERSION_4_0
    UNIFORM_OF_TYPE( gmtl::Matrix22d );
    UNIFORM_OF_TYPE( gmtl::Matrix33d );
    UNIFORM_OF_TYPE( gmtl::Matrix44d );

    UNIFORM_OF_TYPE( gmtl::Matrix23d );
    UNIFORM_OF_TYPE( gmtl::Matrix24d );
    UNIFORM_OF_TYPE( gmtl::Matrix32d );
    UNIFORM_OF_TYPE( gmtl::Matrix34d );
    UNIFORM_OF_TYPE( gmtl::Matrix42d );
    UNIFORM_OF_TYPE( gmtl::Matrix43d );
#endif


#undef UNIFORM_OF_TYPE


    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const { return( UniformPtr( new Uniform( *this ) ) ); }

    /** \brief Execute the glUniform call.
    \details TBD */
    void execute( DrawInfo& drawInfo, const GLint loc ) const;
    /** \brief TBD
    \details TBD
    Does not add this uniform to drawInfo._uniformMap.
    \gl{section 2.11.4}. */
    void executeWithoutMap( DrawInfo& drawInfo ) const;
    /** \brief TBD
    \details TBD
    Adds this uniform to drawInfo._uniformMap.
    \gl{section 2.11.4}. */
    virtual void execute( DrawInfo& drawInfo );

    void setType( const GLenum type );
    GLenum getType() { return( _type ); }

    void setTranspose( const bool transpose=true ) { _transpose = transpose; }
    bool getTranspose() const { return( _transpose ); }

    Program::HashValue getNameHash() const { return( _indexHash ); }

protected:
    void internalInit( const std::string& name );

    std::string _name;
    Program::HashValue _indexHash;

    boost::any _value;

    GLenum _type;
    bool _isSampler;
    bool _transpose;
};

typedef jag::base::ptr< const jag::draw::Uniform >::shared_ptr ConstUniformPtr;
typedef std::vector< UniformPtr > UniformVec;



class UniformSet;
typedef jag::base::ptr< jag::draw::UniformSet >::shared_ptr UniformSetPtr;


/** \class UniformSet Uniform.h <jag/draw/Uniform.h>
\brief TBD
\details TBD */
class UniformSet : public CommandSet< Program::HashValue, UniformPtr, UniformSet, UniformSetPtr >
{
protected:
    typedef CommandSet< Program::HashValue, UniformPtr, UniformSet, UniformSetPtr > SET_TYPE;

public:
    UniformSet()
        : SET_TYPE( Command::UniformSet_t )
    {}
    UniformSet( const UniformSet& rhs )
        : SET_TYPE( rhs )
    {}
    ~UniformSet()
    {}

    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const
    {
        return( UniformSetPtr( new UniformSet( *this ) ) );
    }

    /** \brief TBD
    \details TBD */
    void insert( UniformPtr up )
    {
        operator[]( up->getNameHash() ) = up;
    }
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_UNIFOR_H__
#endif
