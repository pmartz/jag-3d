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

#ifndef __JAGDRAW_UNIFOR_H__
#define __JAGDRAW_UNIFOR_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/Program.h>
#include <jagBase/LogBase.h>
#include <jagDraw/CommandMap.h>
#include <jagBase/ptr.h>
#include <gmtl/gmtl.h>

#include <boost/any.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <string>


namespace jagDraw {


struct DrawInfo;

class Uniform;
typedef jagBase::ptr< jagDraw::Uniform >::shared_ptr UniformPtr;


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

To be done: Constructors of classes deriving from jagBase::LogBase should
take an optional \c logName parameter to allow overriding the default
logger name. This is currently not implemented for the Uniform class.

\gl{section 2.11.4}.
*/
class JAGDRAW_EXPORT Uniform : public DrawablePrep, protected jagBase::LogBase,
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
    virtual DrawablePrepPtr clone() { return( UniformPtr( new Uniform( *this ) ) ); }

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

typedef jagBase::ptr< const jagDraw::Uniform >::shared_ptr ConstUniformPtr;
typedef std::vector< UniformPtr > UniformVec;



class UniformSet;
typedef jagBase::ptr< jagDraw::UniformSet >::shared_ptr UniformSetPtr;


/** \class UniformSet Uniform.h <jagDraw/Uniform.h>
\brief TBD
\details TBD */
class UniformSet : public DrawablePrepSet< Program::HashValue, UniformPtr, UniformSet, UniformSetPtr >
{
protected:
    typedef DrawablePrepSet< Program::HashValue, UniformPtr, UniformSet, UniformSetPtr > SET_TYPE;

public:
    UniformSet()
        : SET_TYPE( DrawablePrep::UniformSet_t )
    {}
    UniformSet( const UniformSet& rhs )
        : SET_TYPE( rhs )
    {}
    ~UniformSet()
    {}

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone()
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


// jagDraw
}


// __JAGDRAW_UNIFOR_H__
#endif
