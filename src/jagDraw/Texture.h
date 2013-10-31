/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef __JAGDRAW_TEXTURE_H__
#define __JAGDRAW_TEXTURE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/FramebufferAttachable.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Image.h>
#include <jagDraw/Sampler.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/CommandMap.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagDraw {


class Texture;
typedef jagBase::ptr< jagDraw::Texture >::shared_ptr TexturePtr;


/** \class Texture Texture.h <jagDraw/Texture.h>
\brief TBD
\details

\logname jag.draw.tex
*/
class JAGDRAW_EXPORT Texture : public DrawablePrep,
        public FramebufferAttachable, public ObjectID,
        protected jagBase::LogBase
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

    /** \brief TBD
    \details TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    void setSampler( SamplerPtr sampler );
    /** \brief TBD
    \details TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    SamplerPtr getSampler() const;


    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const { return( TexturePtr( new Texture( *this ) ) ); }

    /** \brief TBD
    \details Set the active texture.
    Override from DrawablePrep.

    TBD GL 4.0 spec 3.8.1 says "A texture object may be bound
    to more than one texture unit simultaneously." Need to make
    sure JAG supports this. */
    virtual void activate( DrawInfo& drawInfo, const unsigned int unit );

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    \details Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );
    /** \brief TBD
    \details Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );
    /** \brief TBD
    \details Override from ObjectID. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    \details Override from FramebufferAttachable.
    \gl{section 4.4.2} */
    virtual void attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment );

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
    /** Imitial value: jagDraw::SamplerPtr().
    TBD Sampler should be orthogonal to Texture and should be in SamplerSet. */
    SamplerPtr _sampler;

    /** Used if _target==GL_TEXTURE_BUFFER. Initial value: GL_NONE */
    GLenum _bufferFormat;
    /** Used if _target==GL_TEXTURE_BUFFER. Initial value: 0 */
    TextureBufferPtr _textureBuffer;


    void internalInit( const unsigned int contextID );
    void internalSpecifyTexImage( const unsigned int contextID );
    void internalSpecifyTexImage( const GLenum target, ImagePtr image );

    jagDraw::PerContextGLboolean _dirty;
};

typedef std::vector< TexturePtr > TextureVec;



class TextureSet;
typedef jagBase::ptr< jagDraw::TextureSet >::shared_ptr TextureSetPtr;

/** \class TextureSet Texture.h <jagDraw/Texture.h>
\brief TBD
\details TBD */
class TextureSet : public ObjectIDOwner,
        public DrawablePrepSet< unsigned int, TexturePtr, TextureSet, TextureSetPtr >
{
protected:
    typedef DrawablePrepSet< unsigned int, TexturePtr, TextureSet, TextureSetPtr > SET_TYPE;

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
    virtual DrawablePrepPtr clone() const
    {
        return( TextureSetPtr( new TextureSet( *this ) ) );
    }

    /** \brief TBD
    \details Override method from DrawablePrep. */
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
    virtual void deleteID( const jagDraw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            dataPair.second->deleteID( contextID );
        }
    }
};


// jagDraw
}


// __JAGDRAW_TEXTURE_H__
#endif
