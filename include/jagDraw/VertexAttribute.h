#ifndef VERTEX_ATTRIBUTE_DEF
#define VERTEX_ATTRIBUTE_DEF

#include <vector>

#include <Chaskii/Mem/ptr.h>
#include <Chaskii/OpenGL.h>

#include <Chaskii/Draw/DrawContext.h>
#include <Chaskii/Draw/DrawableAttribute.h>

namespace iiDraw {

class VertexAttribute
{
    public:
        VertexAttribute()
        {}

        VertexAttribute( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset ):
            m_index(index),
            m_size(size),
            m_type(type),
            m_normalized(normalized),
            m_stride(stride),
            m_offset(offset)
        {}

        void set( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset )
        {
            m_index = index;
            m_size = size;
            m_type = type;
            m_normalized = normalized;
            m_stride = stride;
            m_offset = offset;
        }

        virtual void apply(DrawContext &ctx, GLuint offset=0)
        {
            glEnableVertexAttribArray(m_index);
            glVertexAttribPointer( m_index, m_size, m_type, m_normalized, m_stride, (GLvoid *)(m_offset + offset) );
        }

        GLuint m_index;
        GLint  m_size;
        GLenum m_type;
        GLboolean m_normalized;
        GLsizei m_stride;
        GLuint m_offset;
};

typedef iiMem::ptr<VertexAttribute>::shared_ptr VertexAttribute_ptr;
typedef iiMem::ptr<VertexAttribute>::shared_array_ptr VertexAttribute_array_ptr;

class VertexAttributeList: public std::vector<VertexAttribute_ptr>,  public DrawableAttribute
{
    public:
        VertexAttributeList() :
            DrawableAttribute( DrawableAttribute::VertexAttributeList_t ),
            m_vboOffset(0)
        {}

        virtual ~VertexAttributeList() {}

        virtual void callStats() { stats(); }

        virtual void gfxInit( DrawContext & ) { }

        void setVertexBufferOffset( GLuint off ) { m_vboOffset = off; }
        GLuint getVertexBufferOffset() { return m_vboOffset; }

        virtual void apply(DrawContext &ctx)
        {
            stats();
            for( iterator p = begin(); p != end(); p++ )
                (*p)->apply( ctx, m_vboOffset );
        }

    protected:
        GLuint m_vboOffset;
};

typedef iiMem::ptr<VertexAttributeList>::shared_ptr VertexAttributeList_ptr;


}

#endif
