/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAG_DRAW_TEXTURE_H__
#define __JAG_DRAW_TEXTURE_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/Command.h>
#include <jag/draw/FramebufferAttachable.h>
#include <jag/draw/ObjectID.h>
#include <jag/draw/Image.h>
#include <jag/draw/Sampler.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/CommandMap.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>


namespace jag {
namespace draw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

class Texture;
typedef jag::base::ptr< jag::draw::Texture >::shared_ptr TexturePtr;


/** \class Texture Texture.h <jag/draw/Texture.h>
\brief TBD
\details

\logname jag.draw.tex
*/
class JAGDRAW_EXPORT Texture : public Command,
        public FramebufferAttachable, public ObjectID,
        protected jag::base::LogBase
{
public:
    /**
    \glparam{target,section 3.8}
    */
    Texture( const std::string& logName=std::string( "" ) );
    Texture( const GLenum target, ImagePtr image=ImagePtr((Image*)NULL), const std::string& logName=std::string( "" ) );
    /** TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    Texture( const GLenum target, ImagePtr image, SamplerPtr sampler, const std::string& logName=std::string( "" ) );
    /** \brief Constructor for texture buffer usage.
    \details Warning log message will be generates if \c target is not GL_TEXTURE_BUFFER. */
    Texture( const GLenum target, GLenum bufferFormat, TextureBufferPtr& textureBuffer, const std::string& logName=std::string( "" ) );
    Texture( const Texture& rhs );
    virtual ~Texture();

    /** \brief TBD
    \details TBD */
    void setTarget( const GLenum target );
    /** \brief TBD
    \details TBD */
    GLenum getTarget() const;
    /** \brief TBD
    \details TBD */
    bool isProxy() const;


    /** \brief TBD
    \details
    If \c _target is GL_TEXTURE_CUBE_MAP, use the \c cubeTarget
    parameter to indicate the cube map face, one of
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, or GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
    */
    void setImage( ImagePtr image, const GLenum cubeTarget=GL_NONE );
    /** \brief TBD
    \details If \c cubeTarget is not specified, this function returns
    the image stored in _image[ 0 ]. If \c cibeTarget is one of
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, or GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    the Image corresponding to the cube map face is returned.

    If \c cubeTarget is specified but is not a valid cube map target,
    NULL is returned. */
    ImagePtr getImage( const GLenum cubeTarget=GL_NONE ) const;

    /** \brief Update the osg::draw::Image data from the OpenGL texture object.
    \details This function is a convenience wrapper around glGetTexImage()
    that stores the returned data in the attached jag::draw::Image object.
    If no Image is attached, one is allocated and attached. The attached Image
    allocates memory to hold the returned data, with format, type, dimensions, etc.
    modified in accordance with that of the texture object. */
    bool uploadImage( const GLenum cubeTarget=GL_NONE );

    /** \brief TBD
    \details TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    void setSampler( SamplerPtr sampler );
    /** \brief TBD
    \details TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    SamplerPtr getSampler() const;


    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const { return( TexturePtr( new Texture( *this ) ) ); }

    /** \brief TBD
    \details Set the active texture.
    Override from Command.

    TBD GL 4.0 spec 3.8.1 says "A texture object may be bound
    to more than one texture unit simultaneously." Need to make
    sure JAG supports this. */
    virtual void activate( DrawInfo& drawInfo, const unsigned int unit );

    /** \brief TBD
    Override from Command. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    \details Override from ObjectID. */
    virtual GLuint getID( const jag::draw::jagDrawContextID contextID );
    /** \brief TBD
    \details Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );
    /** \brief TBD
    \details Override from ObjectID. */
    virtual void deleteID( const jag::draw::jagDrawContextID contextID );

    /** \brief TBD
    \details Override from FramebufferAttachable.
    \gl{section 4.4.2} */
    virtual void attachToFBO( const jag::draw::jagDrawContextID contextID, const GLenum attachment );

    /** \brief Mark the Texture dirty for all contexts.
    \details On next execute(), if the Texture is dirty for the current context,
    the texture image will be re-dubmitted to OpenGL. markAllDirty() does not
    delete any existing OpenGL texture IDs, rather existing OpenGL IDs are re-used
    with new Image (texel) data.

    In a typical use case, an application might change the Image data values used
    by a texture. Texture has no way to know that its Image paramaters have been altered,
    so the app must call markAllDirty() to effect the change. */
    void markAllDirty();

    /** \brief Check if the Texture is dirty for the given \c contextID.
    \details Override from base class FramebufferAttachable. */
    virtual bool isDirty( const unsigned int contextID ) const;

protected:
    /** GL_TEXTURE_1D, GL_TEXTURE_2D, etc.<br>
    Initial Value: GL_NONE */
    GLenum _target;
    /** GLenum used to query the current binding. Set when _target changes. */
    GLenum _bindQuery;
    void determineBindQuery();

    /** Imitial value: empty() == true */
    ImageVec _image;
    /** Imitial value: jag::draw::SamplerPtr().
    TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    SamplerPtr _sampler;

    /** Used if _target==GL_TEXTURE_BUFFER. Initial value: GL_NONE */
    GLenum _bufferFormat;
    /** Used if _target==GL_TEXTURE_BUFFER. Initial value: 0 */
    TextureBufferPtr _textureBuffer;


    void internalInit( const unsigned int contextID );
    void internalSpecifyTexImage( const unsigned int contextID );
    void internalSpecifyTexImage( const GLenum target, ImagePtr image );

    jag::draw::PerContextBool _dirty;
};

typedef std::vector< TexturePtr > TextureVec;



class TextureSet;
typedef jag::base::ptr< jag::draw::TextureSet >::shared_ptr TextureSetPtr;

/** \class TextureSet Texture.h <jag/draw/Texture.h>
\brief TBD
\details TBD */
class TextureSet : public ObjectIDOwner,
        public CommandSet< unsigned int, TexturePtr, TextureSet, TextureSetPtr >
{
protected:
    typedef CommandSet< unsigned int, TexturePtr, TextureSet, TextureSetPtr > SET_TYPE;

public:
    TextureSet()
      : ObjectIDOwner(),
        SET_TYPE( TextureSet_t )
    {}
    TextureSet( const TextureSet& rhs )
      : ObjectIDOwner( rhs ),
        SET_TYPE( rhs )
    {}
    ~TextureSet()
    {}


    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const
    {
        return( TextureSetPtr( new TextureSet( *this ) ) );
    }

    /** \brief TBD
    \details Override method from Command. */
    virtual void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            const TexturePtr& texture( dataPair.second );
            texture->activate( drawInfo, dataPair.first );
            texture->execute( drawInfo );
        }
    }

    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->setMaxContexts( numContexts );
        }
    }
    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void deleteID( const jag::draw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->deleteID( contextID );
        }
    }
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_TEXTURE_H__
#endif
