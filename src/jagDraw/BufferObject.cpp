#include <stdio.h>
#include <Chaskii/Draw/BufferObject.h>

namespace iiDraw {

BufferObject::BufferObject( BufferObject::Target target ):
    DrawableAttribute( DrawableAttribute::BufferObject_t ),
    m_target(target),
    m_gfxInited(false),
    m_usage( DynamicDraw )
{}

BufferObject::BufferObject( BufferObject::Target target, iiMem::Buffer_ptr b, Usage usage ):
    DrawableAttribute( DrawableAttribute::BufferObject_t ),
    m_target(target),
    m_gfxInited(false),
    m_usage(usage),
    m_buffer(b)
{}

BufferObject::BufferObject( const BufferObject &b ):
    DrawableAttribute( DrawableAttribute::BufferObject_t ),
    m_target(b.m_target),
    m_gfxInited(b.m_gfxInited),
    m_usage(b.m_usage),
    m_buffer(b.m_buffer),
    m_id(b.m_id)
{}


BufferObject::~BufferObject() 
{
}

void BufferObject::setBuffer( iiMem::Buffer_ptr b ) 
{ 
    m_buffer = b;
}

iiMem::Buffer_ptr BufferObject::getBuffer() 
{ 
    return m_buffer; 
}

void BufferObject::setUsage( BufferObject::Usage usage ) 
{ 
    m_usage = usage; 
}

BufferObject::Usage BufferObject::getUsage() 
{ 
    return m_usage; 
}

void BufferObject::apply( DrawContext &ctx )
{
    if( !m_gfxInited &&  !p_gfxInit() )
        return;

    stats();

    glBindBuffer( m_target, m_id );
}

void BufferObject::gfxInit( DrawContext &ctx )
{
    if( !m_gfxInited )
        p_gfxInit();
}

void BufferObject::subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid *data )
{
#ifdef GL_EXT_direct_state_access
    glNamedBufferSubDataEXT( m_id, offset, size, data );
#else
    glBindBuffer( m_target, m_id );
    glBufferSubData( m_target, offset, size, data );
#endif
}

GLbyte *BufferObject::map( BufferObject::Access access )
{
    if( !m_gfxInited )
        p_gfxInit();

#ifdef GL_EXT_direct_state_access
    return (GLbyte *)glMapNamedBufferEXT( m_id, access );
#else
    glBindBuffer( m_target, m_id );
    GLbyte *addr = (GLbyte *)glMapBuffer( m_target, access );
    glBindBuffer(m_target, 0 );
    return addr;
#endif
}

void BufferObject::unmap()
{
    if( !m_gfxInited )
        p_gfxInit();

#ifdef GL_EXT_direct_state_access
    glUnmapNamedBufferEXT( m_id );
#else
    glBindBuffer( m_target, m_id );
    glUnmapBuffer( m_target );
    glBindBuffer(m_target, 0 );
#endif
}


bool BufferObject::p_gfxInit()
{
    if( m_gfxInited == true )
        return m_gfxInited;

    if( m_buffer.get() == NULL )
        return m_gfxInited;

    glGenBuffers( 1, &m_id );

#ifdef GL_EXT_direct_state_access
    glNamedBufferDataEXT( m_id, m_buffer->getSize(), m_buffer->data(), m_usage );
#else
    glBindBuffer( m_target, m_id );
    glBufferData( m_target, m_buffer->getSize(), m_buffer->data(), m_usage );
    glBindBuffer( m_target, 0 );
#endif

    m_buffer_size = m_buffer->getSize();

    return (m_gfxInited = true);
}

}
