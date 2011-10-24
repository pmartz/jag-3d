#ifndef CHASKII_DRAW_PROGRAM_DEF
#define CHASKII_DRAW_PROGRAM_DEF 1

#include <map>
#include <vector>

#include <Chaskii/Mem/ptr.h>
//#include <Chaskii/Draw/DrawContext.h>
//#include <Chaskii/Draw/DrawableAttribute.h>
#include <Chaskii/Draw/Shader.h>
#include <Chaskii/Draw/UniformValue.h>

namespace iiDraw {

class ShaderProgram //: public DrawableAttribute
{
    public:
        enum UniformLocationName
        { 
            ModelViewProjectionMatrix,
            NormalMatrix
        };

        ShaderProgram();
        ~ShaderProgram();

        void attachShader( Shader_ptr shader );

        GLint getHandle();
        bool link();
        bool validate();

        //void apply( DrawContext &);
        void apply();
        static void applyNone();

        void setUniformLocationNameString( UniformLocationName name, const std::string &string );
        GLuint getUniformLocation( const std::string &name );
        GLuint getUniformLocation( UniformLocationName name );
        void getActiveUniform( GLuint index, std::string &name, GLenum &type );
        void setUniformValue( UniformLocationName, const UniformValue &v );
        void setUniformValue( const std::string &uniformName, const UniformValue &v );

        void setParameter( GLenum pname, GLint value ); // Used for Geometry Shaders 
        void get( GLenum pname, GLint *params );

        void printInfoLog();

        void bindAttribLocation( GLuint index, const std::string &name );
        GLuint getAttribLocation( const std::string &name );

        // Convenience Function
        void fromSourceFiles( const std::string &vertexShaderFile,
                              const std::string &fragmentShaderFile );

        void fromSourceStrings( const std::string &vertexShaderString,
                               const std::string &fragmentShaderString );

        class SourceList: public std::vector<std::pair<GLenum, std::string > >
        {
            public:
                void add( GLenum type, const std::string &s )
                {
                    push_back( std::pair<GLenum,std::string>(type,s));
                }
        };

        void fromSourceFileList( const SourceList & );
        void fromSourceStringList( const SourceList & );

    private:
        bool m_initialized;
        bool m_linked;
        GLuint m_handle;

        struct LocationTypePair {
            GLint loc;
            GLenum type;

            LocationTypePair() {}
            LocationTypePair( const LocationTypePair &l):
                loc(l.loc),
                type(l.type) {}

            LocationTypePair( GLint l, GLenum t ): loc(l), type(t) {}
        };

        ShaderList m_shaders;
        std::map<std::string, LocationTypePair> m_nameToLocationTypeMap;

        std::map< UniformLocationName, GLint> m_nameToLocationMap;
        std::map< std::string,  UniformLocationName > m_stringToNameMap;

        void p_init();

        bool p_findLocationTypePair( const std::string &name, ShaderProgram::LocationTypePair &lp );


};

typedef iiMem::ptr<ShaderProgram>::shared_ptr ShaderProgram_ptr;
//typedef iiMem::ptr<ShaderProgram>::shared_array_ptr ShaderProgram_array_ptr;

}

#endif
