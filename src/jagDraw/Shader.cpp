#include <iostream>
#include <fstream>

#include <Chaskii/Draw/Shader.h>

namespace iiDraw {

void Shader::printInfoLog()
{
    GLsizei bufLen = 0;
#if !defined( __APPLE__ ) && !defined(USE_GLES2)
    std::string typeStr = m_type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          m_type == GL_GEOMETRY_SHADER_EXT ? "Geometry Shader" :
                          m_type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;
#else
    std::string typeStr = m_type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          m_type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;

#endif
    glGetShaderiv( m_handle, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        std::cerr << "\n==========  " << typeStr << " Information Log ============= " << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        glGetShaderInfoLog( m_handle, bufLen, &strLen, infoLog );
        if( strLen > 0 )
            std::cerr << infoLog << std::endl;
        std::cerr << "==================================================\n" << std::endl;
        delete [] infoLog;
    }
}

Shader::Shader( GLenum type ):
    m_initialized(false),
    m_type(type),
    m_handle(0)
{
}

Shader::~Shader()
{
    if( m_handle != 0 )
        glDeleteShader( m_handle );
}

void Shader::addSourceFile( const std::string &filename )
{
    if( m_initialized )
        return ;

    std::string src = p_loadSource( filename );
    if( !src.empty() )
        addSourceString( src );
}

void Shader::addSourceString( const std::string &source )
{
    if( m_initialized )
        return;

    m_sourceList.push_back( source );
}

GLuint Shader::getHandle()
{
    if( !m_initialized )
        p_init();

    return m_handle;
}


void Shader::p_init()
{
    if( m_initialized )
        return;
    m_initialized = true;

    std::vector<const char *>src;
    std::vector<GLint> length;
    for( std::vector<std::string>::iterator  s = m_sourceList.begin(); s != m_sourceList.end(); s++ )
    {
        //1. Scan for any built-in variables
        //   a. If OpenGL 3.1 replace deprecated 'gl_' variables with equivalent 'ii_' variables
        length.push_back( s->size() );
        src.push_back( s->c_str() );
    }

    m_handle = glCreateShader( m_type );
    glShaderSource( m_handle, src.size(), &src.front(), &length.front() );

    glCompileShader( m_handle );

    GLint status;
    glGetShaderiv( m_handle, GL_COMPILE_STATUS, &status);
    if( status != GL_TRUE )
    {
        printInfoLog();
        glDeleteShader( m_handle );
        m_handle = 0;
    }

    m_sourceList.clear();
}


std::string Shader::p_loadSource( const std::string &filename )
{
    std::ifstream  in(filename.c_str());
    if( !in )
    {
        std::cerr << "Shader::p_loadSource() - unable to load file \"" << filename << "\"." << std::endl;
        return std::string();
    }

    in.seekg( 0, std::ios::end);
    unsigned long size = in.tellg();
    in.seekg( 0, std::ios::beg);
    char *buff = new char[size + 1];
    in.read( buff, size );
    in.close();
    buff[size] = 0;
    std::string str(buff);
    delete [] buff;

    return str;
}

}


