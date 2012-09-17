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
#include <jagBase/ptr.h>
#include <gmtl/gmtl.h>

#include <boost/any.hpp>

#include <vector>
#include <string>


namespace jagDraw {


struct DrawInfo;


/** \class Uniform Uniform.h <jagDraw\Uniform.h>
\brief
\details \gl{section 2.11.4}
*/
class JAGDRAW_EXPORT Uniform : public DrawablePrep, public jagBase::LogBase,
        public SHARED_FROM_THIS(Uniform)
{
public:
    /** \brief Constructor, specified uniform name. */
    Uniform( const std::string& name );
    /** \brief Constructor, specified uniform name and type.
    \details \param type from OpenGL spec table 2.10. */
    Uniform( const std::string& name, const GLenum type );
    /** \brief Constructor, specifically for creation of sampler uniforms.
    \detauls \param type from OpenGL spec table 2.10.
    \param v Sampler texture unit. */
    explicit Uniform( const std::string& name, const GLenum type, const GLint& v );
    /** \brief Copy constructor. */
    Uniform( const Uniform& rhs );
    /** \brief Destructor. */
    virtual ~Uniform();


    /** \brief Set the value of a sampler uniform.
    \details It's also acceptable to use the set(GLint) method. */
    void setSampler( const GLint& v );


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
    UNIFORM_OF_TYPE( GLdouble );

    UNIFORM_OF_TYPE( gmtl::Point2i );
    UNIFORM_OF_TYPE( gmtl::Point3i );
    UNIFORM_OF_TYPE( gmtl::Point4i );

    UNIFORM_OF_TYPE( gmtl::Point2ui );
    UNIFORM_OF_TYPE( gmtl::Point3ui );
    UNIFORM_OF_TYPE( gmtl::Point4ui );

    UNIFORM_OF_TYPE( gmtl::Point2f );
    UNIFORM_OF_TYPE( gmtl::Point3f );
    UNIFORM_OF_TYPE( gmtl::Point4f );

    UNIFORM_OF_TYPE( gmtl::Point2d );
    UNIFORM_OF_TYPE( gmtl::Point3d );
    UNIFORM_OF_TYPE( gmtl::Point4d );

    UNIFORM_OF_TYPE( gmtl::Matrix22f );
    UNIFORM_OF_TYPE( gmtl::Matrix33f );
    UNIFORM_OF_TYPE( gmtl::Matrix44f );

    UNIFORM_OF_TYPE( gmtl::Matrix23f );
    UNIFORM_OF_TYPE( gmtl::Matrix24f );
    UNIFORM_OF_TYPE( gmtl::Matrix32f );
    UNIFORM_OF_TYPE( gmtl::Matrix34f );
    UNIFORM_OF_TYPE( gmtl::Matrix42f );
    UNIFORM_OF_TYPE( gmtl::Matrix43f );

    UNIFORM_OF_TYPE( gmtl::Matrix22d );
    UNIFORM_OF_TYPE( gmtl::Matrix33d );
    UNIFORM_OF_TYPE( gmtl::Matrix44d );

    UNIFORM_OF_TYPE( gmtl::Matrix23d );
    UNIFORM_OF_TYPE( gmtl::Matrix24d );
    UNIFORM_OF_TYPE( gmtl::Matrix32d );
    UNIFORM_OF_TYPE( gmtl::Matrix34d );
    UNIFORM_OF_TYPE( gmtl::Matrix42d );
    UNIFORM_OF_TYPE( gmtl::Matrix43d );


#undef UNIFORM_OF_TYPE


    /** \brief TBD
    \details TBD
    Does not add this uniform to drawInfo._uniformMap.
    \gl{section 2.11.4} */
    void operator()( DrawInfo& drawInfo, const GLint loc ) const;
    /** \brief TBD
    \details TBD
    Adds this uniform to drawInfo._uniformMap.
    \gl{section 2.11.4} */
    virtual void operator()( DrawInfo& drawInfo );

    void setType( const GLenum type );
    GLenum getType() { return( _type ); }

    void setTranspose( const bool transpose=true ) { _transpose = transpose; }
    bool getTranspose() const { return( _transpose ); }

    const Program::HashValue getNameHash() const { return( _indexHash ); }

protected:
    void internalInit( const std::string& name );

    std::string _name;
    Program::HashValue _indexHash;

    boost::any _value;

    GLenum _type;
    bool _isSampler;
    bool _transpose;
};

typedef jagBase::ptr< jagDraw::Uniform >::shared_ptr UniformPtr;
typedef jagBase::ptr< const jagDraw::Uniform >::shared_ptr ConstUniformPtr;
typedef std::vector< UniformPtr > UniformList;


// jagDraw
}


// __JAGDRAW_UNIFOR_H__
#endif
