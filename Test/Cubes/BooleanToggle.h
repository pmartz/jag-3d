#ifndef BOOLEAN_TOGGLE_DEF
#define BOOLEAN_TOGGLE_DEF

#include <Chaskii/WindowSystem/EventResponder.h>

class BooleanToggle: public iiWS::EventResponder
{
    public:
        BooleanToggle( iiWS::KeyCharacter key, bool &flag ):
            iiWS::EventResponder(true),                 // Calling the super-class constructor with auto_register
                                                        // set to true, automatically registers the event responder
            m_key(key),
            m_flag(flag)
        {}

        virtual ~BooleanToggle() { puts("BooleanToggle DESTRUCTOR"); }

        virtual bool keyPress( iiWS::KeyCharacter k, bool isRepeat ) 
        {
            if( k == m_key )
                m_flag = !m_flag;

            return false;
        }
    protected:
        iiWS::KeyCharacter m_key;
        bool &m_flag;
};

typedef iiMem::ptr<BooleanToggle>::shared_ptr BooleanToggle_ptr;

#endif
