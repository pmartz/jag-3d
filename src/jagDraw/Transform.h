#ifndef CHASKII_DRAW_TRANSFORM_DEF
#define CHASKII_DRAW_TRANSFORM_DEF

#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Math/matrix.h>

namespace iiDraw {

class Transform: public DrawableAttribute
{
    public:
        Transform() :
            DrawableAttribute( Transform_t )
        {}

        Transform( const iiMath::matrix4f &m ):
            DrawableAttribute( Transform_t ),
            m_matrix( m )
        {}

        void setMatrix( const iiMath::matrix4f &m )
        {
            m_matrix = m;
        }
        const iiMath::matrix4f &getMatrix() 
        { 
            return m_matrix; 
        }

        virtual void apply( DrawContext &ctx )
        {
            ctx.applyModelMatrix( m_matrix );
        }

        virtual void gfxInit( DrawContext &ctx )
        {
            // nothing to do, really.
        }

    protected:
        iiMath::matrix4f m_matrix;
};

typedef iiMem::ptr<Transform>::shared_ptr Transform_ptr;

}

#endif
