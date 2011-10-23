#ifndef STATE_CONTROL_DEF
#define STATE_CONTROL_DEF

#include <Chaskii/WindowSystem/EventResponder.h>
#include <Chaskii/OpenGL.h>

class StateControl: public iiWS::EventResponder
{
    public:
        StateControl():
            iiWS::EventResponder( true ),
            wireframe(false),
            cullface(true)
        {
        }


        virtual bool keyPress( iiWS::KeyCharacter key, bool isRepeat ) 
        {
            switch( key )
            {
                case iiWS::KeyChar_b:
                    cullface = !cullface;
                    if( cullface )
                        glEnable( GL_CULL_FACE );
                    else
                        glDisable( GL_CULL_FACE );
                    break;

                case iiWS::KeyChar_w:
                    wireframe = !wireframe;
                    if( wireframe )
                        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                    else
                        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    break;

                default:
                    break;

            }
            return false;
        }
    private:
        bool wireframe;
        bool cullface;
};
#endif
