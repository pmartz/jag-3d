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

#ifndef __JAGDRAW_PROGRAM_H__
#define __JAGDRAW_PROGRAM_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/Command.h>
#include <jag/base/LogBase.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Shader.h>
#include <jagDraw/PerContextData.h>
#include <jag/base/ptr.h>

#include <map>
#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

struct DrawInfo;

class Program;
typedef jag::base::ptr< Program >::shared_ptr ProgramPtr;


/** \class Program Program.h <jagDraw/Program.h>
\brief
\details \gl{section 2.11.2}.
\logname jag.draw.program
*/
class JAGDRAW_EXPORT Program : public Command,
            public ObjectID, public SHARED_FROM_THIS(Program),
            protected jag::base::LogBase
{
public:
    // TBD seems out of place. Not sure why these matrices need special
    // treatment at the jagDraw level.
    enum UniformLocationName
    { 
        ModelViewProjectionMatrix,
        NormalMatrix
    };

    Program( const std::string& logName=std::string( "" ) );
    Program( const Program& rhs );
    ~Program();

    /** \brief Attach a shader to this program.
    \details This function adds \c shader to the \c _shaders list.
    The link() member function calls glAttachShader() for each
    shader in \c _shaders.
    \gl{section 2.11.2}.
    */
    void attachShader( ShaderPtr shader );
    /** \brief Detach all attached shaders.
    \details Useful for changing shader code on the fly. TBD This is sort of
    a sledgehammer approach; We really need more flexibility here. */
    void detachAllShaders();

    /** \brief Force a re-link for all contexts.
    \details TBD */
    void forceRelink();

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const { return( ProgramPtr( new Program( *this ) ) ); }

    /** \brief Make this program the current program.
    \details Implicitly calls link() if necessary for this context,
    then calls glUseProgram(). Also records itself in DrawInfo
    for later location queries by vertex attribs and uniforms.

    This function also iterates over its list of active uniforms and
    looks for a matching uniform in drawInfo._uniformMap. If a match is
    found, this function sets the matching uniform's value in OpenGL state.
    This allows uniforms to be specified in the draw list prior to shader
    programs.
    \gl{section 2.11.2}. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief Get the OpenGL program object ID for the specified \c contextID.
    \details If an ID hasn't already been created for \c contextID, getId() calls
    glCreateProgram() to generate the ID. */
    virtual GLuint getID( const unsigned int contextID );

    /** \brief Tell the Program how many contexts to expect.
    \details Resizes the _ids variable, then iterates over all attached
    Shader objects, invoking setMaxContexts on each.
    */
    virtual void setMaxContexts( const unsigned int numContexts );

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
    unsigned int getNumActiveAttribs( const GLuint id ) const;



    /** \brief Set a program parameter.
    \details Associates a \c value with a \c pname and marks
    the parameter list as dirty. On next call to execute(),
    all parameters will be sent to OpenGL following the call to
    glUseProgram().
    
    Note there is no way to remove a parameter. If you with to
    restore a parameter setting to a default value, call setParameter()
    and pass the default for \c value. */
    void setParameter( GLenum pname, GLint value );
    /** \brief Get a previously set program parameter.
    \details Returns the set value in \c params and returns
    true, if \c pname was previously set. If \c pname was
    not set, returns false and leaves \c params unchanged. */
    bool getParameter( GLenum pname, GLint* params ) const;

    void printInfoLog( const GLuint id );



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


    // Used in the interface to UniformBlock.
    typedef std::map< HashValue, GLuint > OffsetMap;
    typedef struct {
        GLuint _bindIndex;
        size_t _minSize;
        OffsetMap _offsets;
    } BlockInfo;

    bool hasUniformBlock( const HashValue& h ) const;
    /** \overload */
    bool hasUniformBlock( const std::string& s ) const;

    const BlockInfo& getUniformBlockInfo( const HashValue& h ) const;
    /** \overload */
    const BlockInfo& getUniformBlockInfo( const std::string& s ) const;

    void addUniformAlias( const std::string& name, const std::string& alias );
    void addVertexAttribAlias( const std::string& name, const std::string& alias );

private:
    void internalInit( const unsigned int contextID );

    ShaderVec _shaders;

    PerContextBool _linkStatus;

    void internalDetach( const unsigned int contextID );
    ShaderVec _detachedShaders;
    PerContextBool _needsDetach;

    typedef std::map< GLenum, GLint > ParameterMap;
    ParameterMap _parameters;
    PerContextBool _parametersDirty;

    // Support for uniforms in the default uniform block
    typedef std::map< HashValue, GLint > LocationMap;
    LocationMap _uniformLocations;

    // Named uniform block support (aka interface blocks)
    typedef std::map< HashValue, BlockInfo > BlockInfoMap;
    BlockInfoMap _blockInfo;

    // Vertex attribute support
    LocationMap _vertexAttribLocations;

    typedef std::map< std::string, GLint > ExplicitLocationMap;
    ExplicitLocationMap _explicitVertexAttribLocations;

    typedef std::map< std::string, std::string > NameMap;
    NameMap _uniformAliases;
    NameMap _vertexAttribAliases;
};

typedef std::vector< ProgramPtr > ProgramVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_SHADER_PROGRAM_H__
#endif
