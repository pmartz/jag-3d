// Copyright

#ifndef __JAGDRAW_SHADER_H__
#define __JAGDRAW_SHADER_H__ 1


#include <vector>
#include <string>

#include <jagDraw/Export.h>
#include <jagBase/PlatformOpenGL.h>
#include <jagBase/ptr.h>
#include <string>


namespace jagDraw {


class JAGDRAW_EXPORT Shader 
{
public:
    Shader( GLenum type );
    ~Shader();

    void addSourceFile( const std::string& fileName );

    void addSourceString( const std::string& source );

    GLuint getHandle();

    void printInfoLog();

private:
    void p_init();
    bool _initialized;

    std::string p_loadSource( const std::string& fileName );

    GLenum _type;
    std::vector< std::string > _sourceList;
    GLuint _handle;
};

typedef jagBase::ptr< jagDraw::Shader >::shared_ptr ShaderPtr;
typedef std::vector< ShaderPtr > ShaderList;


// jagDraw
}


// __JAGDRAW_SHADER_H__
#endif
