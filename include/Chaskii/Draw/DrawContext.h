#ifndef CHASKII_DRAW_CONTEXT_DEF
#define CHASKII_DRAW_CONTEXT_DEF 1

#include <Chaskii/OpenGL.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Math/matrix.h>
#include <Chaskii/Draw/ShaderProgram.h>
//#include <Chaskii/Draw/DrawableAttribute.h>

namespace iiDraw {

class DrawContext
{
    public:
        DrawContext( GLint id):
            m_id(id)
        {}

        GLint getID() { return m_id; }

        void setShaderProgram( ShaderProgram_ptr shader )
        {
            m_shader = shader;
        }

        const ShaderProgram_ptr &getCurrentShaderProgram() { return m_shader; }

        void setProjectionMatrix( const iiMath::matrix4f &P ) 
        { 
            m_P = P; 
        }
        const iiMath::matrix4f &getCurrentProjectionMatrix() { return m_P; }

        void setViewMatrix( const iiMath::matrix4f &V ) 
        { 
            m_V = V; 
        }
        const iiMath::matrix4f &getCurrentViewMatrix() { return m_V; }

        void applyModelMatrix( const iiMath::matrix4f &M ) 
        { 
            if( m_shader != NULL )
            {
                iiMath::matrix4f VM = m_V * M;
                { 
                    iiMath::matrix4f O;
                    gmtl::invertFull( O, VM ); 
                    iiMath::matrix3f b = gmtl::makeRot< iiMath::matrix3f >( gmtl::makeRot< gmtl::EulerAngleXYZf >( O ));
                    gmtl::transpose( b );
                    m_shader->setUniformValue( ShaderProgram::NormalMatrix, b );
                }
                iiMath::matrix4f PVM =  m_P * VM;
                m_shader->setUniformValue( ShaderProgram::ModelViewProjectionMatrix, PVM );
            }
        }

        /*
        void applyDrawableAttribute( const DrawableAttribute_ptr &da )
        {
            switch( da.getDrawableAttributeType() )
            {
                default:
                case ShaderProgram_t:
                    m_shader = dynamic_cast<ShaderProgram_ptr>(da);
                    break;
            }
        }
        */

        //void setCurrentDrawableAttribute( iiMem::ptr<DrawableAttribute> & );


    private:
        const GLint m_id;
        iiMath::matrix4f m_P;
        iiMath::matrix4f m_V;
        iiMath::matrix4f m_N;
        ShaderProgram_ptr m_shader;

};

}

#endif
