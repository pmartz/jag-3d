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

#ifndef __JAGDRAW_SHADER_H__
#define __JAGDRAW_SHADER_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogBase.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/types.h>
#include <jagBase/ptr.h>
#include <string>
#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

/** \class Shader Shader.h <jagDraw/Shader.h>
\brief TBD.
\logname jag.draw.shader
*/
class JAGDRAW_EXPORT Shader : public ObjectID, protected jagBase::LogBase
{
public:
    Shader( GLenum type, const std::string& logName=std::string( "" ) );
    Shader( const Shader& rhs );
    ~Shader();

    void addSourceString( const std::string& source );
    void insertSourceString( const std::string& source, const unsigned int index=0 );
    unsigned int getNumSourceStrings() const;
    jagBase::StringVec& getSourceVec();

    /** \brief Get the OpenGL shader object ID for the specified \c contextID.
    \details If an ID hasn't already been created for \c contextID, getId() calls
    glCreateShader() to generate the ID and compiles all attached shader source strings. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );
    
    /** \brief TBD
    \details TBD
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

protected:
    void internalInit( const unsigned int contextID );

    void printInfoLog( const GLuint id );

    std::string getFullSource() const;

    static std::string getTypeAsString( GLenum type );

    GLenum _type;
    jagBase::StringVec _sourceVec;
};

typedef jagBase::ptr< jagDraw::Shader >::shared_ptr ShaderPtr;
typedef std::vector< ShaderPtr > ShaderVec;


/*@}*/


// jagDraw
}


// __JAGDRAW_SHADER_H__
#endif
