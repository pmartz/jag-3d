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
#include <jagDraw/CommandMap.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \class Texture Texture.h <jagDraw/Texture.h>
\brief TBD
\details TBD
*/
class JAGDRAW_EXPORT Texture : public DrawablePrep,
        public FramebufferAttachable, public ObjectID,
        protected jagBase::LogBase
{
public:
    /**
    \glparam{target,section 3.8}
    */
    Texture();
    Texture( const GLenum target, ImagePtr image=ImagePtr((Image*)NULL) );
    Texture( const GLenum target, ImagePtr image, SamplerPtr sampler );
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
    \details TBD */
    ImagePtr getImage() const;
    /** \brief TBD
    \details If there was no previous call to setImage() to set a cube map
    face, this function returns a NULL pointer. Otherwise, the function
    returns the cube map face image corresponging to \c cubeTarget, which
    must be one of
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, or GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
    */
    ImagePtr getImage( const GLenum cubeTarget ) const;

    /** \brief TBD
    \details TBD */
    void setSampler( SamplerPtr sampler );
    /** \brief TBD
    \details TBD */
    SamplerPtr getSampler() const;


    /** \brief TBD
    \details Set the active texture.
    Override from DrawablePrep. */
    virtual void activate( DrawInfo& drawInfo, const unsigned int unit );

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );
    /** \brief TBD
    Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );
    /** \brief TBD
    Override from ObjectID. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    \details Override from FramebufferAttachable.
    \gl{section 4.4.2} */
    virtual void attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment );

protected:
    /** GL_TEXTURE_1D, GL_TEXTURE_2D, etc.<br>
    Initial Value: GL_NONE */
    GLenum _target;

    /** Imitial value: empty() == true */
    ImageVec _image;
    /** Imitial value: jagDraw::SamplerPtr() */
    SamplerPtr _sampler;


    void internalInit( const unsigned int contextID );
    void internalSpecifyTexImage( const GLenum target, ImagePtr image );
};

typedef jagBase::ptr< jagDraw::Texture >::shared_ptr TexturePtr;
typedef std::vector< TexturePtr > TextureVec;



class TextureSet : public DrawablePrep, public ObjectIDOwner
{
public:
    TextureSet()
      : DrawablePrep( TextureSet_t ),
        ObjectIDOwner()
    {}
    TextureSet( const TextureSet& rhs )
      : DrawablePrep( rhs ),
        ObjectIDOwner( rhs )
    {}
    ~TextureSet()
    {}

    TexturePtr& operator[]( const unsigned int key )
    {
        return( _map[ key ] );
    }


    typedef std::map< unsigned int, TexturePtr > InternalMapType;

    /** \brief TBD
    \details Override method from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( const InternalMapType::value_type& dataPair, _map )
        {
            TexturePtr texture( dataPair.second );
            texture->activate( drawInfo, dataPair.first );
            texture->execute( drawInfo );
        }
    }

    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        BOOST_FOREACH( const InternalMapType::value_type& dataPair, _map )
        {
            dataPair.second->setMaxContexts( numContexts );
        }
    }
    /** \brief TBD
    \details Override method from ObjectIDOwner */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID )
    {
        BOOST_FOREACH( const InternalMapType::value_type& dataPair, _map )
        {
            dataPair.second->deleteID( contextID );
        }
    }

protected:
    InternalMapType _map;
};

typedef jagBase::ptr< jagDraw::TextureSet >::shared_ptr TextureSetPtr;


// jagDraw
}


// __JAGDRAW_TEXTURE_H__
#endif
