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
class JAGDRAW_EXPORT Uniform : public DrawablePrep,
        public SHARED_FROM_THIS(Uniform)
{
public:
    Uniform( const std::string& name );
    Uniform( const std::string& name, const GLenum type );
    Uniform( const Uniform& rhs );
    virtual ~Uniform();

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

    void setType( const GLenum type ) { _type = type; }
    GLenum getType() { return( _type ); }

    void setTranspose( const bool transpose=true ) { _transpose = transpose; }
    bool getTranspose() const { return( _transpose ); }

protected:
    void internalInit( const std::string& name );

    std::string _name;
    Program::HashValue _indexHash;

    boost::any _value;

    GLenum _type;
    bool _transpose;
};

typedef jagBase::ptr< jagDraw::Uniform >::shared_ptr UniformPtr;
typedef jagBase::ptr< const jagDraw::Uniform >::shared_ptr ConstUniformPtr;
typedef std::vector< UniformPtr > UniformList;


// jagDraw
}


// __JAGDRAW_UNIFOR_H__
#endif
