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

#ifndef __JAGDRAW_SHADER_PROGRAM_H__
#define __JAGDRAW_SHADER_PROGRAM_H__ 1


#include <jagDraw/Export.h>
//#include <Chaskii/Draw/DrawContext.h>
//#include <Chaskii/Draw/DrawableAttribute.h>
#include <jagDraw/Shader.h>
#include <jagDraw/PerContextData.h>
#include <jagDraw/UniformValue.h>
#include <jagBase/ptr.h>

#include <map>
#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class ShaderProgram ShaderProgram.h <jagDraw/ShaderProgram.h>
\brief
\details \gl{section 2.11.2}.
*/
class JAGDRAW_EXPORT ShaderProgram : public SHARED_FROM_THIS(ShaderProgram)
{
public:
    enum UniformLocationName
    { 
        ModelViewProjectionMatrix,
        NormalMatrix
    };

    ShaderProgram();
    ~ShaderProgram();

    /** \brief Attach a shader to this program.
    \details This function adds \c shader to the \c _shaders list.
    The link() member function calls glAttachShader() for each
    shader in \c _shaders.
    \gl{section 2.11.2}.
    */
    void attachShader( ShaderPtr shader );

    /** \brief Make this program the current program.
    \details Implicitly calls link() if necessary for this context,
    then calls glUseProgram().
    \gl{section 2.11.2}.
    */
    void use( DrawInfo& drawInfo );

    /** \brief
    \details
    */
    GLint getId( unsigned int contextID );

    /** \brief
    \details \gl{section 2.11.2}.
    */
    bool link( unsigned int contextID );
    bool validate( unsigned int contextID );

    void setUniformLocationNameString( UniformLocationName name, const std::string& string );
    GLuint getUniformLocation( const std::string& name );
    GLuint getUniformLocation( UniformLocationName name );
    // TBD need to get context ID, probably as a param?
    void getActiveUniform( GLuint index, std::string& name, GLenum& type );
    void setUniformValue( UniformLocationName, const UniformValue& v );
    void setUniformValue( const std::string& uniformName, const UniformValue& v );

    // TBD need to get context ID, probably as a param?
    void setParameter( GLenum pname, GLint value ); // Used for Geometry Shaders 
    // TBD need to get context ID, probably as a param?
    void get( GLenum pname, GLint* params );

    // TBD need to get context ID, probably as a param?
    void printInfoLog();

    void setExplicitAttribLocation( GLuint index, const std::string& name );
    GLuint getExplicitAttribLocation( const std::string& name ) const;

    // Convenience Function
    void fromSourceFiles( const std::string& vertexShaderFile,
                            const std::string& fragmentShaderFile );

    void fromSourceStrings( const std::string& vertexShaderString,
                            const std::string& fragmentShaderString );

    class SourceList : public std::vector<std::pair<GLenum, std::string > >
    {
    public:
        void add( GLenum type, const std::string& s )
        {
            push_back( std::pair< GLenum, std::string >( type, s ) );
        }
    };

    void fromSourceFileList( const SourceList & );
    void fromSourceStringList( const SourceList & );


    typedef std::size_t HashValue;
    static HashValue createHash( const std::string& name );

    GLint getUniformLocation( const HashValue& h ) const;
    GLint getUniformLocation( const std::string& s ) const;
    GLint getVertexAttribLocation( const HashValue& h ) const;
    GLint getVertexAttribLocation( const std::string& s ) const;

private:
    struct LocationTypePair {
        GLint loc;
        GLenum type;

        LocationTypePair() {}
        LocationTypePair( const LocationTypePair &l):
            loc(l.loc),
            type(l.type) {}

        LocationTypePair( GLint l, GLenum t ): loc(l), type(t) {}
    };

    ShaderList _shaders;
    std::map<std::string, LocationTypePair> m_nameToLocationTypeMap;

    std::map< UniformLocationName, GLint> m_nameToLocationMap;
    std::map< std::string,  UniformLocationName > m_stringToNameMap;

    void internalInit( const unsigned int contextID );


    typedef std::pair< GLuint, bool > IDLinkPair;
    typedef PerContextData< IDLinkPair > PerContextIDLink;
    PerContextIDLink _ids;

    typedef std::map< HashValue, GLint > LocationMap;
    LocationMap _uniformLocations;
    LocationMap _vertexAttribLocations;

    typedef std::map< std::string, GLint > ExplicitLocationMap;
    ExplicitLocationMap _explicitVertexAttribLocations;
};

typedef jagBase::ptr< ShaderProgram >::shared_ptr ShaderProgramPtr;


// jagDraw
}


// __JAGDRAW_SHADER_PROGRAM_H__
#endif
