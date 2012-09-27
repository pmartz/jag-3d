#ifndef CUBE_DEF
#define CUBE_DEF
#include <map>

//#include <Chaskii/Draw/Drawable.h>
#include <Chaskii/Draw/DrawableAttribute.h>
#include <Chaskii/Mem/ptr.h>
#include "categorized_set.h"
#include <Chaskii/Math/vec3.h>

struct Bound 
{
    public:
        iiMath::vec3 center;
        double radius;

        Bound( const iiMath::vec3 &c, double r ):
            center(c),
            radius(r) {}

        Bound() {}
        Bound( const Bound &b ):
            center(b.center),
            radius(b.radius) {}

        void set( const iiMath::vec3 &c, double r )
        {
            center = c;
            radius = r;
        }
};

namespace iiDraw {

class Drawable: public categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<DrawableAttribute> >
{

    public:
        Drawable() { }

        Drawable( const categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<DrawableAttribute> > &d )
        {}

        virtual ~Drawable() { }

            
        void addAttribute( const sortable_ptr<DrawableAttribute> &a )
        {   
            (*this)[a->getDrawableAttributeType()] = a;
        }   

        virtual void gfxInit( DrawContext &ctx )
        {
            for( iterator p = begin(); p != end(); p++ )
            {
                p->second->gfxInit( ctx );
            }
        }   
            
        virtual void apply( DrawContext &ctx )
        {   
            for( iterator p = begin(); p != end(); p++ )
            {
                p->second->apply( ctx );
            }
        }

        Bound m_bound;
};


typedef iiMem::ptr<Drawable>::shared_ptr Drawable_ptr;

}

class Cube : public iiDraw::Drawable
{
    public:
        Cube();
        Cube( float x, float y, float z, bool t );

    private:
        bool m_is_transp;
        void asDrawArrays();
        void asMultiDrawArrays( float x, float y, float z );
        void asDrawElements();

};

//typedef iiMem::ptr<Cube>::shared_ptr Cube_ptr;
#endif
