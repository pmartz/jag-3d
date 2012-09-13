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
#include <jagDraw/ObjectID.h>
#include <jagDraw/Image.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \class Texture Texture.h <jagDraw/Texture.h>
\brief TBD
\details TBD
*/
class Texture : public DrawablePrep,
        public ObjectID, protected jagBase::LogBase
{
public:
    /**
    \glparam{target,section 3.8}
    */
    Texture();
    Texture( const GLenum target, ImagePtr image=ImagePtr((Image*)NULL) );
    Texture( const Texture& rhs );
    virtual ~Texture();

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void operator()( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

#if 0 // chaskii stuff
    virtual void callStats() { stats(); }

    virtual bool operator < (DrawableAttribute &rhs ) const
    {
        Texture &rhs_t = dynamic_cast<Texture &>(rhs);
        return m_id < rhs_t.m_id;
    }

    virtual bool operator > (DrawableAttribute &rhs) const
    {
        Texture &rhs_t = dynamic_cast<Texture &>(rhs);
        return m_id > rhs_t.m_id;
    }

    virtual bool operator == ( DrawableAttribute &rhs ) const
    {
        Texture &rhs_t = dynamic_cast<Texture &>(rhs);
        return m_id == rhs_t.m_id;
    }
#endif

private:
    /** GL_TEXTURE_1D, GL_TEXTURE_2D, etc. */
    GLenum _target;

    jagDraw::ImagePtr _image;


    void internalInit( const unsigned int contextID );
};

typedef jagBase::ptr< jagDraw::Texture >::shared_ptr TexturePtr;
typedef std::vector< TexturePtr > TextureList;


// jagDraw
}


// __JAGDRAW_TEXTURE_H__
#endif
