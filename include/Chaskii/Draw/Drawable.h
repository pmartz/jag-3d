#ifndef DRAWABLE_DEF
#define DRAWABLE_DEF

#include <vector>
#include <algorithm>
#include <Chaskii/Mem/ptr.h>

#include <Chaskii/Draw/DrawableAttribute.h>
#include <Chaskii/Draw/VertexAttribute.h>
#include <Chaskii/Draw/VertexIterator.h>

namespace iiDraw {

class Drawable
{
    public:
        Drawable() {}

        void addAttribute( const DrawableAttribute_ptr &a )
        {
            if( a->getDrawableAttributeType() == DrawableAttribute::VertexAttribute_t )
            {
                addVertexAttribute( std::tr1::dynamic_pointer_cast<VertexAttribute>(a) );
            }
            else if( a->getDrawableAttributeType() == DrawableAttribute::VertexIterator_t ) 
            { 
                addVertexIterator( std::tr1::dynamic_pointer_cast<VertexIterator>(a) );
            }
            else
            {
                m_attributes[a->getDrawableAttributeType()] = a;
            }
            //m_attributes.push_back( a );
            //m_needsSort = true;
        }

        void addVertexAttribute( const VertexAttribute_ptr &va )
        {
            m_vertexAttributes.push_back( va );
        }

        void addVertexIterator( const VertexIterator_ptr &vi )
        {
            m_vertexIterators.push_back( vi );
        }

        static bool mycompare( DrawableAttribute_ptr a,  DrawableAttribute_ptr b )
        {
            return a->getDrawableAttributeType() < b->getDrawableAttributeType();
        }

        void applyDrawableAttributes( DrawContext &ctx )
        {
            for( std::map<DrawableAttribute::DrawableAttributeType, DrawableAttribute_ptr>::const_iterator p = m_attributes.begin(); 
                    p != m_attributes.end(); p++ )
            {
                p->second->apply( ctx );
            }
        }

        void applyVertexData( DrawContext &ctx )
        {
            for( VertexAttributeList::iterator p = m_vertexAttributes.begin(); 
                    p != m_vertexAttributes.end(); p++ )
            {
                (*p)->apply( ctx );
            }
            for( VertexIteratorList::iterator p = m_vertexIterators.begin(); 
                    p != m_vertexIterators.end(); p++ )
            {
                (*p)->apply( ctx );
            }
        }


        virtual void apply( DrawContext &ctx )
        {
            applyDrawableAttributes(ctx);
            applyVertexData(ctx);
        }

        virtual void gfxInit( DrawContext &ctx )
        {
            for( std::map<DrawableAttribute::DrawableAttributeType, DrawableAttribute_ptr>::const_iterator p = m_attributes.begin(); 
                    p != m_attributes.end(); p++ )
            {
                p->second->gfxInit( ctx );
            }
        }

        DrawableAttributeList &getDrawableAttributeList() 
        { 
            return m_attributes; 
        }

        const DrawableAttributeList &getDrawableAttributeList()  const
        { 
            return m_attributes; 
        }
        
#if 0
        bool containsDrawableAttribute( DrawableAttribute::DrawableAttributeType t ) const
        {
            DrawableAttributeList::const_iterator p = m_attributes.find(t);
            return p != m_attributes.end();
        }

        const DrawableAttribute &getDrawableAttribute( DrawableAttribute::DrawableAttributeType t )
        {
            DrawableAttributeList::const_iterator p = m_attributes.find(t);
            if( p != m_attributes.end() )
                return p->second;

            return DrawableAttribute();
        }

        bool operator != ( const Drawable &rhs ) 
        {
            const DrawableAttributeList &m = rhs.getDrawableAttributeList();
            for( DrawableAttributeList::const_iterator p =  m_attributes.begin();
                    p != m_attributes.end(); p++ )
            {
                if( rhs.contains( p->first ) )
                {
                    if( p->second != m[p->first] )
                        return true;
                }
            }
            return false;
        }
#endif


    private:
        DrawableAttributeList m_attributes;
        VertexAttributeList m_vertexAttributes;
        VertexIteratorList m_vertexIterators;

};

typedef iiMem::ptr<Drawable>::shared_ptr Drawable_ptr;
typedef iiMem::ptr<Drawable>::shared_array_ptr Drawable_array_ptr;

}

#endif
