#ifndef CHASKII_FRAME_RATE_DEF
#define CHASKII_FRAME_RATE_DEF

#include <time.h>

#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Math/rect.h>
#include <Chaskii/OpenGL.h>
#include <Chaskii/Draw/ShaderProgram.h>
#include <Chaskii/Draw/DrawableAttribute.h>
#include "Text.h"


class FrameRate 
{
    public:
        FrameRate():
            text(new Text)
        {
        }

        virtual void operator()(const iiMath::recti &r, double hz )
        {
            iiDraw::ShaderProgram::applyNone();
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glOrtho( 0.0, (double)r.width(), 0.0, (double)r.height(), -1.0, 1.0 );
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();

            glColor4f( 1.0, 1.0, 1.0, 1.0 );
            glPushAttrib( GL_ENABLE_BIT );

            glDisable( GL_LIGHTING );
            glDisable( GL_TEXTURE_2D );

            text->setPixelSize( 20.0 );
            text->setColor( 1.0, 1.0, 1.0, 1.0 );
            int y = r.height() - 30;
            text->setPosition( 10, y );

            char buff[256];
            sprintf(buff, "FPS: %4.0lf", hz );
            text->setString( buff );
            text->render();

            sprintf(buff, "DrawableAttribute applies: \n" );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();

            sprintf(buff, "    ShaderProgram: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::ShaderProgram_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();

            sprintf(buff, "         Texture0: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::Texture0_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();

            sprintf(buff, "         Texture1: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::Texture1_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();

            sprintf(buff, "        Transform: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::Transform_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();
            sprintf(buff, "     BufferObject: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::BufferObject_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();
            sprintf(buff, "  VertexAttribute: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::VertexAttributeList_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();
            sprintf(buff, "   VertexIterator: %4d\n", iiDraw::DrawableAttribute::applies[iiDraw::DrawableAttribute::VertexIteratorList_t] );
            y -= 22;
            text->setPosition( 10, y );
            text->setString( buff );
            text->render();

           glPopAttrib();
        }

    private:
        iiMem::ptr<Text>::shared_ptr text;
};

#endif
