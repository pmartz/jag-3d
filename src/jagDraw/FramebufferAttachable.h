/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
