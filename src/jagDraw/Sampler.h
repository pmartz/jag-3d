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

#ifndef __JAGDRAW_SAMPLER_H__
#define __JAGDRAW_SAMPLER_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <boost/foreach.hpp>
#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

// forward
struct SamplerState;

typedef jagBase::ptr< jagDraw::SamplerState >::shared_ptr SamplerStatePtr;


/** \class Sampler Sampler.h <jagDraw/Sampler.h>
\brief Support for Sampler objects.
\details
\gl{3.8.2}

Sampler objects were introduced in OpenGL v3.3 to provide object storage and binding
for parameters that control the behavior of GLSL texture samplers. Currently, the
Sampler class provides backwards compatible behavior for OpenGL v3.2 in order to support
Mac OSX. When OSX moves to a more recent version of OpenGL, much of the Sampler
implementation could be cleaned up at that time.
*/
class JAGDRAW_EXPORT Sampler : public DrawablePrep,
        public ObjectID, protected jagBase::LogBase
{
public:
    Sampler( const std::string& logName=std::string( "" ) );
    Sampler( const Sampler& rhs );
    ~Sampler();


    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const { return( DrawablePrepPtr( new Sampler( *this ) ) ); }

    /** \brief TBD
    \details TBD */
    virtual void activate( DrawInfo&, const unsigned int unit );

    /** \brief TBD
    \details TBD */
    virtual void execute( DrawInfo& );

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



class SamplerSet;
typedef jagBase::ptr< jagDraw::SamplerSet >::shared_ptr SamplerSetPtr;

/** \class SamplerSet Sampler.h <jagDraw/Sampler.h>
\brief TBD
\details TBD */
class SamplerSet : public ObjectIDOwner,
        public DrawablePrepSet< unsigned int, SamplerPtr, SamplerSet, SamplerSetPtr >
{
protected:
    typedef DrawablePrepSet< unsigned int, SamplerPtr, SamplerSet, SamplerSetPtr > SET_TYPE;

public:
    SamplerSet()
      : ObjectIDOwner(),
        SET_TYPE( SamplerSet_t )
    {}
    SamplerSet( const SamplerSet& rhs )
      : ObjectIDOwner( rhs ),
        SET_TYPE( rhs )
    {}
    ~SamplerSet()
    {}


    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const
    {
        return( SamplerSetPtr( new SamplerSet( *this ) ) );
    }

    /** \brief TBD
    \details Override method from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo )
    {
        BOOST_FOREACH( const MAP_TYPE::value_type& dataPair, *this )
        {
            const SamplerPtr& sampler( dataPair.second );
            sampler->activate( drawInfo, dataPair.first );
            sampler->execute( drawInfo );
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


/*@}*/


// jagDraw
}


// __JAGDRAW_SAMPLER_H__
#endif
