#ifndef CUBE_DEF
#define CUBE_DEF

#include <Chaskii/Draw/Drawable.h>
#include <Chaskii/Mem/ptr.h>

class Cube : public iiDraw::Drawable
{
    public:
        Cube();

    private:
        void asDrawArrays();
        void asMultiDrawArrays();
        void asDrawElements();

};

typedef iiMem::ptr<Cube>::shared_ptr Cube_ptr;
#endif
