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
#include <jagDraw/DrawablePrep.h>
#include <jagBase/LogBase.h>
#include <jagDraw/Shader.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>

#include <map>
#include <vector>


namespace jagDraw {


struct DrawInfo;


/** \class ShaderProgram ShaderProgram.h <jagDraw/ShaderProgram.h>
\brief
\details \gl{section 2.11.2}.
*/
class JAGDRAW_EXPORT ShaderProgram : public DrawablePrep,
            public SHARED_FROM_THIS(ShaderProgram),
            public jagBase::LogBase
{
public:
    // TBD seems out of place. Not sure why these matrices need special
    // treatment at the jagDraw level.
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
    then calls glUseProgram(). Also records itself in drawInfo._program
    for later location queries by vertex attribs and uniforms.

    This function also iterates over its list of active uniforms and
    looks for a matching uniform in drawInfo._uniformMap. If a match is
    found, this function sets the matching uniform's value in OpenGL state.
    This allows uniforms to be specified in the draw list prior to shader
    programs.
    \gl{section 2.11.2}. */
    virtual void operator()( DrawInfo& drawInfo );

    /** \brief Get the OpenGL program object ID for the specified \c contextID.
    \details If an ID hasn't already been created for \c contextID, getID() calls
    glCreateProgram() to generate the ID. */
    GLint getId( unsigned int contextID );

    /** \brief Link the program.
    \details This function performs the following tasks:
    \li If the program doesn't have a program object ID for \c contextID, it creates one.
    \li It calls glBindAttribLocation() for all vertex attribs whose locations were specified using setExplicitAttribLocation().
    \li It calls glAttachShader() for all \link Shader Shaders \endlink attached using attachShader(), then clears the list of attached shaders (to remove references to them).
    \li It calls glLinkProgram(), then queries GL_LINK_STATUS.
    If the link was unsuccessful, link() calls printInfoLog(), deletes the program object
    with a call to glDeleteProgram, and returns false.

    Otherwise, link() populates the _uniformLocations and _vertexAttribLocations maps with
    all active uniforms and vertex attribs, to allow fast look up of locations keyed by
    HashValue values of the uniform and vertex attrib names.
    \gl{section 2.11.2}. */
    bool link( unsigned int contextID );

    /** \brief Convenience wrapper around glValidateProgram().
    \details This function calls glValidateProgram, then queries
    GL_VALIDATE_STATUS.
    \returns The result of the GL_VALIDATE_STATUS query, converted to bool.
    */
    bool validate( unsigned int contextID );

    /** \brief
    \details
    */
    unsigned int getNumActiveUniforms( const GLuint id ) const;
    /** \brief
    \details
    */
    void getActiveUniform( const GLuint id, const GLuint index, std::string& name, GLenum& type );

    /** \brief
    \details
    */
    unsigned int getNumActiveAttribs( const GLuint id ) const;
    /** \brief
    \details
    */
    void getActiveAttrib( const GLuint id, const GLuint index, std::string& name, GLenum& type );



    // TBD need to get context ID, probably as a param?
    void setParameter( GLenum pname, GLint value ); // Used for Geometry Shaders 
    // TBD need to get context ID, probably as a param?
    void get( GLenum pname, GLint* params );

    // TBD need to get context ID, probably as a param?
    void printInfoLog();



    /** \brief Support for glBindAttribLocation().
    \details Locations are stored in a map indexed by name. See the
    member variable _explicitVertexAttribLocations.
    The link() function calls glBindAttribLocation() on all members
    of \c _explicitVertexAttribLocations.
    \gl{section 2.11.3}.
    */
    void setExplicitAttribLocation( GLuint index, const std::string& name );
    /** \brief Retrieve an explicitly set vertex attrib location value.
    \details See also 
    getVertexAttribLocation( const HashValue& h ) const and
    getVertexAttribLocation( const std::string& s ) const.
    */
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

    /** \brief Fast access to uniform location.
    \details Uses HashValue \c h as a map key to access the location index.
    \returns -1 if \c h is not in the map. Otherwise, returns the location.
    */
    GLint getUniformLocation( const HashValue& h ) const;
    /** \brief Slow access to uniform location.
    \details Creates a HashValue from the string \c s, then calls
    getUniformLocation( const HashValue& h ) const. */
    GLint getUniformLocation( const std::string& s ) const;
    /** \brief Fast access to vertex attrib location.
    \details Uses HashValue \c h as a map key to access the location index.
    \returns -1 if \c h is not in the map. Otherwise, returns the location.
    */
    GLint getVertexAttribLocation( const HashValue& h ) const;
    /** \brief Slow access to vertex attrib location.
    \details Creates a HashValue from the string \c s, then calls
    getVertexAttribLocation( const HashValue& h ) const. */
    GLint getVertexAttribLocation( const std::string& s ) const;

private:
    void internalInit( const unsigned int contextID );

    ShaderList _shaders;

    PerContextIDStatus _ids;

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
