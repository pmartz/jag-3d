#ifndef CHASII_DRAW_DRAWABLE_ATTRIBUTE_DEF
#define CHASII_DRAW_DRAWABLE_ATTRIBUTE_DEF

#include <stdio.h>
#include <map>

#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Draw/DrawContext.h>

namespace iiDraw {

class DrawableAttribute 
{
    public:
        enum DrawableAttributeType 
        {
            Empty_t,
            ShaderProgram_t,
            Transform_t,
            BufferObject_t,
            Texture0_t = GL_TEXTURE0,
            Texture1_t,
            VertexAttributeList_t,
            VertexIteratorList_t,
        };


        DrawableAttribute(): m_type(Empty_t) {}
        DrawableAttribute( DrawableAttributeType type ):
            m_type(type)
        {}

        DrawableAttribute( const DrawableAttribute &d ):
            m_type(d.m_type) {}

        virtual ~DrawableAttribute() {}

        DrawableAttributeType getDrawableAttributeType() const { return m_type; }

        virtual void apply( DrawContext & )  = 0;
        virtual void gfxInit( DrawContext & ) = 0;


        static std::map<DrawableAttributeType, int> applies;
        static void stats_clear()
        {
            applies.clear();
        }

        static void stats_report()
        {
            puts("\033[0;0H");

            printf("DrawableAttribute applies: \n" );
            printf("    ShaderProgram: %4d\n", applies[ShaderProgram_t] );
            printf("         Texture0: %4d\n", applies[Texture0_t] );
            printf("         Texture1: %4d\n", applies[Texture1_t] );
            printf("        Transform: %4d\n", applies[Transform_t] );
            printf("     BufferObject: %4d\n", applies[BufferObject_t] );
            printf("  VertexAttribute: %4d\n", applies[VertexAttributeList_t] );
            printf("   VertexIterator: %4d\n", applies[VertexIteratorList_t] );
        }

        void stats()
        {
            applies[m_type]++;
        }


        virtual bool operator < (DrawableAttribute &rhs ) const 
        {
            return m_type < rhs.m_type;
        }

        virtual bool operator > (DrawableAttribute &rhs) const
        {
            return m_type > rhs.m_type;
        }

        virtual bool operator == ( DrawableAttribute &rhs ) const
        {
            return m_type == rhs.m_type;
        }


    protected:
        DrawableAttributeType m_type;
};


class DrawableAttribute_ptr: public iiMem::ptr<DrawableAttribute>::shared_ptr
{
    public:
        DrawableAttribute_ptr (): iiMem::ptr<DrawableAttribute>::shared_ptr() {}
        DrawableAttribute_ptr ( DrawableAttribute *a ): iiMem::ptr<DrawableAttribute>::shared_ptr( a) {}

        virtual bool operator < (const DrawableAttribute_ptr &rhs )
        {
            return (*(get())) < *(rhs.get());
        }

        virtual bool operator > (const DrawableAttribute_ptr &rhs )
        {
            return (*(get())) > *(rhs.get());
        }
};

//typedef iiMem::ptr<DrawableAttribute>::shared_ptr DrawableAttribute_ptr;
//typedef std::vector<DrawableAttribute_ptr> DrawableAttributeList;
typedef std::map<DrawableAttribute::DrawableAttributeType, DrawableAttribute_ptr> DrawableAttributeList;

}

#endif
