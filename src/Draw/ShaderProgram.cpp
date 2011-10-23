#include <stdio.h>
#include <iostream>
#include <Chaskii/Draw/ShaderProgram.h>

namespace iiDraw {


#ifndef USE_GLEW

#include <GL/glx.h>
void glProgramParameteri( GLuint program, GLenum pname, GLint value )
{
    static void (*_glProgramParameteri)(GLuint, GLenum, GLint) = 
            (void (*)(GLuint,GLenum,GLint))glXGetProcAddress( (GLubyte *)"glProgramParameteriARB");
    _glProgramParameteri( program, pname, value );
}
#endif


void ShaderProgram::printInfoLog()
{
    GLsizei bufLen = 0;       
    glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        std::cerr << "\n==========  Shader Information Log ============= " << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        glGetProgramInfoLog( m_handle, bufLen, &strLen, infoLog );
        if( strLen > 0 )
            std::cerr << infoLog << std::endl;
        std::cerr << "==================================================\n" << std::endl;
        delete [] infoLog;
    }
}

ShaderProgram::ShaderProgram():
    //DrawableAttribute( ShaderProgram_t ),
    m_initialized(false),
    m_linked(false),
    m_handle(0)
{
    m_stringToNameMap["ii_ModelViewProjectionMatrix"] = ModelViewProjectionMatrix;
    m_stringToNameMap["ii_NormalMatrix"] = NormalMatrix;
    m_nameToLocationMap[ModelViewProjectionMatrix] = -1;
}

ShaderProgram::~ShaderProgram()
{
    if( m_handle != 0 )
        glDeleteProgram( m_handle );
}

void ShaderProgram::attachShader( Shader_ptr shader )
{
    m_shaders.push_back( shader );
}

void ShaderProgram::applyNone()
{
    glUseProgram(0);
}

//void ShaderProgram::apply( DrawContext &ctx )
void ShaderProgram::apply( )
{
    if( !m_linked )
        link();

    if( m_handle == 0 )
        return;

    glUseProgram(m_handle);
}

GLuint ShaderProgram::getUniformLocation( const std::string &name )
{
    std::map<std::string, LocationTypePair>::iterator f =  m_nameToLocationTypeMap.find( name );
    if( f != m_nameToLocationTypeMap.end() )
        return f->second.loc;
    return -1;
}

void ShaderProgram::setUniformValue( UniformLocationName name, const UniformValue &value )
{
    GLint loc = m_nameToLocationMap[name];
    if( loc != -1 )
        value.apply( loc );
}

void ShaderProgram::setUniformValue( const std::string &name, const UniformValue &value )
{
    GLint loc = getUniformLocation(name);
    if( loc != -1 )
        value.apply(loc);
}


void ShaderProgram::bindAttribLocation( GLuint index, const std::string &name )
{
    if( !m_initialized )
        p_init();

    glBindAttribLocation( m_handle, index, name.c_str() );
}

GLuint ShaderProgram::getAttribLocation( const std::string &name )
{
    if( !m_initialized )
        p_init();

    return glGetAttribLocation( m_handle, name.c_str() );
}

void ShaderProgram::setParameter( GLenum pname, GLint value )
{
    if( !m_initialized )
        p_init();

#if 0 // these are potentially not supported if not defined correctly.

    if( pname == GL_GEOMETRY_SHADER ) pname = GL_GEOMETRY_SHADER_ARB;
    else if( pname == GL_GEOMETRY_VERTICES_OUT ) pname = GL_GEOMETRY_VERTICES_OUT_ARB;
    else if( pname == GL_GEOMETRY_INPUT_TYPE ) pname = GL_GEOMETRY_INPUT_TYPE_ARB;
    else if( pname == GL_GEOMETRY_OUTPUT_TYPE ) pname = GL_GEOMETRY_OUTPUT_TYPE_ARB;

#endif

    glProgramParameteri(m_handle, pname, value );
}

void ShaderProgram::get( GLenum pname, GLint *params )
{
    glGetProgramiv( m_handle, pname, params );
}

GLint ShaderProgram::getHandle()
{
    if( !m_initialized )
        p_init();

    return m_handle;
}

bool ShaderProgram::link()
{
    if( !m_initialized )
        p_init();

    for( std::vector< Shader_ptr >::iterator s = m_shaders.begin(); s != m_shaders.end(); s++ )
    {
        GLint shader = (*s)->getHandle();
        if( shader != 0 )
            glAttachShader( m_handle, shader );
    }
    m_shaders.clear();

    GLint status;
    glLinkProgram(m_handle);
    glGetProgramiv( m_handle, GL_LINK_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog();
        glDeleteProgram( m_handle );
        m_handle = 0;
    }
    m_linked = (status == GL_TRUE);

    if( m_linked == true )
    {
        GLint n;
        glUseProgram(m_handle);
        glGetProgramiv( m_handle, GL_ACTIVE_UNIFORMS, &n );
printf("Active Uniforms:\n" );
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string string;
            getActiveUniform( i, string, type );
            GLint location = glGetUniformLocation( m_handle, string.c_str() );
printf("......... %d: %s (%d)\n", i, string.c_str(), location );
            m_nameToLocationTypeMap[string] = LocationTypePair( location, type );

            std::map< std::string,  UniformLocationName >::iterator p = m_stringToNameMap.find( string ); 
            if( p != m_stringToNameMap.end() )
            {
                UniformLocationName name = p->second;
                m_nameToLocationMap[name] = location;
            }
        }

        glGetProgramiv( m_handle, GL_ACTIVE_ATTRIBUTES, &n );
printf("\nActive Attributes: (%d)\n", n );
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string name;

            char namebuff[256];
            GLsizei len;
            GLsizei isize = sizeof(namebuff);
            GLint osize;
            //getActiveAttrib( i, name, type );
            glGetActiveAttrib(  m_handle, i, isize, &len, &osize, &type, namebuff );
            name = std::string(namebuff );
            GLint loc = glGetAttribLocation( m_handle, name.c_str() );
printf("......... %d: %s (%d)\n", i, name.c_str(), loc );
            GLint location = glGetUniformLocation( m_handle, name.c_str() );
            m_nameToLocationTypeMap[name] = LocationTypePair( location, type );
        }
        glUseProgram(0);
    }

    return m_linked;
}

bool ShaderProgram::validate()
{
    if( !m_initialized )
        p_init();

    GLint status;
    glValidateProgram(m_handle);
    glGetProgramiv( m_handle, GL_VALIDATE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog();
        glDeleteProgram( m_handle );
        m_handle = 0;
    }
    return (status == GL_TRUE);
}


void ShaderProgram::getActiveUniform( GLuint index, std::string &name, GLenum &type )
{
    char namebuff[256];
    GLsizei len;
    GLsizei isize = sizeof(namebuff);
    GLint osize;
    glGetActiveUniform( m_handle, index, isize, &len, &osize, &type, namebuff );
    name = std::string(namebuff );
}


void ShaderProgram::p_init()
{
    if( m_initialized )
        return;

    m_handle = glCreateProgram();

    m_initialized = true;

}

bool ShaderProgram::p_findLocationTypePair( const std::string &name, ShaderProgram::LocationTypePair &lp )
{
    std::map<std::string, LocationTypePair>::iterator f = m_nameToLocationTypeMap.find(name);

    if( f == m_nameToLocationTypeMap.end() )
        return false;

    lp = f->second;
    return true;
}

void ShaderProgram::fromSourceFiles( const std::string &vertexShaderFile,
                              const std::string &fragmentShaderFile )
{
    iiDraw::Shader_ptr vshader(new iiDraw::Shader(GL_VERTEX_SHADER ));
    vshader->addSourceFile( vertexShaderFile );
    attachShader( vshader );

    iiDraw::Shader_ptr fshader(new iiDraw::Shader(GL_FRAGMENT_SHADER ));
    fshader->addSourceFile( fragmentShaderFile );
    attachShader( fshader );
}

void ShaderProgram::fromSourceStrings( const std::string &vertexShaderString,
                               const std::string &fragmentShaderString )
{
    iiDraw::Shader_ptr vshader(new iiDraw::Shader(GL_VERTEX_SHADER ));
    vshader->addSourceString( vertexShaderString );
    attachShader( vshader );

    iiDraw::Shader_ptr fshader(new iiDraw::Shader(GL_FRAGMENT_SHADER ));
    fshader->addSourceString( fragmentShaderString );
    attachShader( fshader );

}

void ShaderProgram::fromSourceFileList( const SourceList &l )
{
    for( SourceList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        iiDraw::Shader_ptr shader(new iiDraw::Shader( p->first ));
        shader->addSourceFile( p->second );
        attachShader( shader );
    }
}

void ShaderProgram::fromSourceStringList( const SourceList &l )
{
    for( SourceList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        iiDraw::Shader_ptr shader(new iiDraw::Shader( p->first ));
        shader->addSourceString( p->second );
        attachShader( shader );
    }
}


}
