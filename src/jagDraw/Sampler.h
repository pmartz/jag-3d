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

#ifndef __JAGDRAW_SAMPLER_H__
#define __JAGDRAW_SAMPLER_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


// forward
struct SamplerState;

typedef jagBase::ptr< jagDraw::SamplerState >::shared_ptr SamplerStatePtr;


/** \class Sampler Sampler.h <jagDraw/Sampler.h>
\brief Support for Sampler objects.
\details
\gl{3.8.2}
*/
class JAGDRAW_EXPORT Sampler : public ObjectID, protected jagBase::LogBase
{
public:
    Sampler();
    Sampler( const Sampler& rhs );
    ~Sampler();


    /** \brief Add the SamplerState to the Sampler object.
    \derails Also binds the Sampler object (and leaves it bound).
    */
    void executeSampler( const jagDraw::jagDrawContextID contextID, const GLuint unit );

    /** \brief Set the SamplerState as texture parameters.
    \details TBD.
    */
    void executeTexture( const GLenum target );


    /** \brief TBD
    \details TBD */
    SamplerStatePtr getSamplerState() const;


    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );
    
    /** \brief TBD
    Override from ObjectID. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    SamplerStatePtr _samplerState;
};

typedef jagBase::ptr< jagDraw::Sampler >::shared_ptr SamplerPtr;



/** \class SamplerState Sampler.h <jagDraw/Sampler.h>
\brief Comtainer for Sampler object parameters.
\details

All parameter defaults are per \glshort{table 6.18}, except:
\li _minFilter defaults to GL_NEAREST_MIPMAP_LINEAR.
\li _wrapS, _wrapT, and _wrapR default to GL_REPEAT.
Note that client code should change these values when using a SamplerState
with rectangle textures.
*/
struct JAGDRAW_EXPORT SamplerState
{
    SamplerState();
    SamplerState( const SamplerState& rhs );
    virtual ~SamplerState();

    void setDirty( const bool dirty=true ) { _dirty = dirty; }
    bool getDirty() const { return( _dirty ); }

    GLenum _wrapS, _wrapT, _wrapR;
    GLenum _minFilter, _magFilter;
    GLfloat _borderColor[4];
    GLfloat _minLOD, _maxLOD;
    GLint _lodBias;
    GLenum _compareMode, _compareFunc;

    bool _dirty;
};


// jagDraw
}


// __JAGDRAW_SAMPLER_H__
#endif
