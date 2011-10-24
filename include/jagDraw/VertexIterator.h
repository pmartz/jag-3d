#ifndef VERTEX_ITERATOR_DEF
#define VERTEX_ITERATOR_DEF

#include <Chaskii/OpenGL.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/Draw/DrawContext.h>
//#include <Chaskii/Draw/DrawableAttribute.h>

namespace iiDraw {

typedef std::vector<GLint> GLintList;
typedef std::vector<GLsizei> GLsizeiList;
typedef std::vector<const GLvoid *> GLindexList;
typedef std::vector<GLint> GLbaseVertexList;

class VertexIterator //: public DrawableAttribute
{
    public:
        enum VertexIteratorType{
            DrawArrays_t,                 /* * */
            DrawArraysInstanced_t,        /* * */
            DrawArraysIndirect_t,         /* * */
            MultiDrawArrays_t,            /* * */
            DrawElements_t,               /* * */
            DrawElementsInstanced_t,      /* * */
            MultiDrawElements_t,          /* * */
            DrawRangeElements_t, 
            DrawElementsBaseVertex_t,     /* * */
            DrawRangeElementsBaseVertex_t,
            DrawElementsInstancedBaseVertex_t,
            DrawElementsIndirect_t,
            MultiDrawElementsBaseVertex_t
        };

        VertexIterator( VertexIteratorType vertexIteratorType, GLenum mode ):
            //DrawableAttribute( DrawableAttribute::VertexIterator_t),
            m_vertexIteratorType(vertexIteratorType),
            m_mode(mode)
        {}

        virtual ~VertexIterator() 
        {}


        VertexIteratorType getVertexIteratorType() { return m_vertexIteratorType; }
        virtual void apply( DrawContext & ) {}

    protected:
        VertexIteratorType m_vertexIteratorType;
        GLenum m_mode;
        GLint m_first;
        GLsizei m_count;
        GLenum m_type;
        GLuint m_offset;
        GLintList m_firstList;
        GLsizeiList m_countList;
        GLindexList m_indexList;
        GLsizei m_primcount;
        GLint m_basevertex;
        GLbaseVertexList m_baseVertexList;
};

class DrawArrays: public VertexIterator
{
    public:
        DrawArrays( GLenum mode, GLint first, GLsizei count ):
            VertexIterator( DrawArrays_t, mode )
        {
            m_first = first;
            m_count = count;
        }

        virtual void apply( DrawContext & )
        {
            glDrawArrays( m_mode, m_first, m_count );
        }
};

class DrawArraysInstanced: public VertexIterator
{
    public:
        DrawArraysInstanced( GLenum mode, GLint first, GLsizei count, GLsizei primcount ):
            VertexIterator( DrawArraysInstanced_t, mode )
        {
            m_first = first;
            m_count = count;
            m_primcount = primcount;
        }
};

class DrawArraysIndirect: public DrawArraysInstanced
{
    public:
        DrawArraysIndirect( GLenum mode, const void *indirect ):
            DrawArraysInstanced( mode, 
                    (GLint)((unsigned int *)indirect)[0], 
                    (GLsizei)((unsigned int *)indirect)[1], 
                    (GLsizei)((unsigned int *)indirect)[2]  )
        {}
};

class DrawElements: public VertexIterator
{
    public:

        DrawElements( GLenum mode,
                      GLsizei count,
                      GLenum type,
                      GLuint offset ):
            VertexIterator( DrawElements_t, mode )
        {
            m_count = count;
            m_type = type;
            m_offset = offset;
        }

        virtual void apply( DrawContext & )
        {
            glDrawElements( m_mode, m_count, m_type, (GLvoid *)m_offset );
        }
};

class DrawElementsBaseVertex: public VertexIterator
{
    public:
        DrawElementsBaseVertex( GLenum mode, 
                                GLsizei count, 
                                GLenum type, 
                                GLuint offset,
                                GLint basevertex ):
            VertexIterator( DrawElementsBaseVertex_t , mode )
        {
            m_count = count;
            m_type = type;
            m_offset = offset;
            m_basevertex = basevertex;
        }

        virtual void apply( DrawContext & )
        {
            glDrawElementsBaseVertex( m_mode, m_count, m_type, (GLvoid *)m_offset, m_basevertex );
        }
};

class DrawElementsInstanced: public VertexIterator
{
    public:

        DrawElementsInstanced( GLenum mode,
                      GLsizei count,
                      GLenum type,
                      GLuint offset,
                      GLsizei primcount):
            VertexIterator( DrawElementsInstanced_t , mode )
        {
            m_count = count;
            m_type = type;
            m_offset = offset;
            m_primcount = primcount;
        }

        virtual void apply( DrawContext & )
        {
            glDrawElementsInstanced( m_mode, m_count, m_type, (GLvoid *)m_offset, m_primcount );
        }
};

class MultiDrawArrays: public VertexIterator
{
    public:
        MultiDrawArrays( GLenum mode, const GLintList &firstList,
                                      const GLsizeiList &countList, 
                                      GLsizei primcount ):
            VertexIterator( MultiDrawArrays_t, mode )
        {
            m_firstList = firstList;
            m_countList = countList;
            m_primcount = primcount;
        }

        virtual void apply( DrawContext & )
        {
            glMultiDrawArrays( m_mode, &m_firstList.front(), &m_countList.front(), m_primcount );
        }

};

class MultiDrawElements: public VertexIterator
{
    public:
        MultiDrawElements( GLenum mode, const GLsizeiList &countList, GLenum type, const GLindexList &indexList, GLsizei primcount):
            VertexIterator( MultiDrawElements_t, mode )
        { 
            m_countList = countList;
            m_type = type;
            m_indexList = indexList;
            m_primcount = primcount;
        }

        virtual void apply ( DrawContext & )
        {
            glMultiDrawElements( m_mode, &m_countList.front(), m_type, &m_indexList.front(), m_primcount );
        }
};

class MultiDrawElementsBaseVertex: public VertexIterator
{
    public:
        MultiDrawElementsBaseVertex( GLenum mode, 
                                     const GLsizeiList &countList, 
                                     GLenum type, 
                                     const GLindexList &indexList, 
                                     GLsizei primcount,
                                     const GLbaseVertexList &baseVertexList):
            VertexIterator( MultiDrawElementsBaseVertex_t, mode )
        { 
            m_countList = countList;
            m_type = type;
            m_indexList = indexList;
            m_primcount = primcount;
            m_baseVertexList = baseVertexList;
        }

        virtual void apply( DrawContext & )
        {
            glMultiDrawElements( m_mode, &m_countList.front(), m_type, &m_indexList.front(), m_primcount );
        }
};


typedef iiMem::ptr<VertexIterator>::shared_ptr VertexIterator_ptr;
typedef iiMem::ptr<VertexIterator>::shared_array_ptr VertexIterator_array_ptr;

class VertexIteratorList: public std::vector<VertexIterator_ptr>, public DrawableAttribute
{
    public:
        VertexIteratorList():
            DrawableAttribute( DrawableAttribute::VertexIteratorList_t ),
            m_elementBufferOffset(0)

        {}

        virtual ~VertexIteratorList() {}

        virtual void callStats() { stats(); }
        virtual void gfxInit( DrawContext & ) { }

        virtual void apply(DrawContext &ctx)
        {
            stats();
            for( iterator p = begin(); p != end(); p++ )
                (*p)->apply( ctx/*, m_elementBufferOffset */ );
        }

    protected:
        GLuint m_elementBufferOffset;
};

typedef iiMem::ptr<VertexIteratorList>::shared_ptr VertexIteratorList_ptr;

}

#endif
