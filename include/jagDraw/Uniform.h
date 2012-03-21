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
#include <jagDraw/ShaderProgram.h>
#include <jagBase/ptr.h>
#include <gmtl/gmtl.h>

#include <vector>
#include <string>


namespace jagDraw {


struct DrawInfo;


/** \class Uniform Uniform.h <jagDraw\Uniform.h>
\brief
\details \gl{section 2.11.4}
*/
class JAGDRAW_EXPORT Uniform : public DrawablePrep, public SHARED_FROM_THIS(Uniform)
{
public:
    Uniform( const std::string& name );
    Uniform( const Uniform& u );

    explicit Uniform( const std::string& name, const bool b );
    explicit Uniform( const std::string& name, const GLint i );
    explicit Uniform( const std::string& name, const GLfloat f );

    explicit Uniform( const std::string& name, const gmtl::Point2f& p );
    explicit Uniform( const std::string& name, const gmtl::Vec3f& p );
    explicit Uniform( const std::string& name, const gmtl::Point3f& p );
    explicit Uniform( const std::string& name, const gmtl::Point4f& p );

    explicit Uniform( const std::string& name, const gmtl::Matrix33f& m );
    explicit Uniform( const std::string& name, const gmtl::Matrix44f& m );

    /*
    UniformValue( iiMath::vec2i v2i );
    UniformValue( iiMath::vec3i v3i );
    UniformValue( iiMath::vec4i v4i );
    */

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

    GLenum getType() { return _type; }

    // TBD need more setters, one for each type.
    void set( const gmtl::Matrix44f& m );

    void setTranspose( const bool transpose=true ) { _transpose = transpose; }
    bool getTranspose() const { return( _transpose ); }

protected:
    void internalInit( const std::string& name );

    std::string _name;
    ShaderProgram::HashValue _indexHash;

    GLenum _type;

    union {
        bool    b;

        GLint   i;
        GLint   v2i[2]; 
        GLint   v3i[3]; 
        GLint   v4i[4]; 

        GLfloat f;
        GLfloat p2f[2];
        GLfloat p3f[3];
        GLfloat p4f[4];

        GLfloat mat3f[9];
        GLfloat mat4f[16];
    } _value;

    bool _transpose;
};

typedef jagBase::ptr< jagDraw::Uniform >::shared_ptr UniformPtr;
typedef jagBase::ptr< const jagDraw::Uniform >::shared_ptr ConstUniformPtr;
typedef std::vector< UniformPtr > UniformList;


// jagDraw
}


// __JAGDRAW_UNIFOR_H__
#endif
