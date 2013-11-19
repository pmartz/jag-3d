/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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

#ifndef __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#define __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/ContextSupport.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class FramebufferAttachable FramebufferAttachable.h <jagDraw/FramebufferAttachable.h>
\brief TBD
\details TBD
\gl{section 4.4}.
*/
class JAGDRAW_EXPORT FramebufferAttachable
{
public:
    FramebufferAttachable( GLenum fboTarget=GL_DRAW_FRAMEBUFFER, GLint fboTextureLevel=0 )
      : _fboTarget( fboTarget ),
        _fboTextureLevel( fboTextureLevel )
    {}
    FramebufferAttachable( const FramebufferAttachable& rhs )
      : _fboTextureLevel( rhs._fboTextureLevel )
    {}
    virtual ~FramebufferAttachable()
    {}

    /** \brief TBD
    \details TBD */
    virtual bool isDirty( const unsigned int contextID ) const = 0;

    /** \brief TBD
    \details TBD */
    virtual void attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment ) = 0;

    /** \brief TBD
    \details TBD */
    void setFBOTextureLevel( GLint fboTextureLevel )
    {
        _fboTextureLevel = fboTextureLevel;
    }
    GLint getFBOTextureLevel() const
    {
        return( _fboTextureLevel );
    }

protected:
    GLenum _fboTarget;
    GLint _fboTextureLevel;
};

/*@}*/

typedef jagBase::ptr< jagDraw::FramebufferAttachable >::shared_ptr FramebufferAttachablePtr;
typedef std::vector< FramebufferAttachablePtr > FramebufferAttachableVec;


// jagDraw
}


// __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#endif
