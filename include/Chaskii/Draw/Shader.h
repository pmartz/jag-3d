#ifndef SHADER_DEF
#define SHADER_DEF

#include <vector>
#include <string>

#include <Chaskii/OpenGL.h>
#include <Chaskii/Mem/ptr.h>

namespace iiDraw {

class Shader 
{
    public:
        Shader( GLenum type );
        ~Shader();

        void addSourceFile( const std::string &filename );

        void addSourceString( const std::string &source );

        GLuint getHandle();

        void printInfoLog();

    private:
        bool m_initialized;
        GLenum m_type;
        std::vector<std::string>m_sourceList;
        GLuint m_handle;

        void p_init();
        std::string p_loadSource( const std::string &filename );
};

typedef iiMem::ptr<Shader>::shared_ptr Shader_ptr;
typedef std::vector<Shader_ptr> ShaderList;
//typedef iiMem::ptr<Shader>::shared_array_ptr Shader_array_ptr;

}

#endif
