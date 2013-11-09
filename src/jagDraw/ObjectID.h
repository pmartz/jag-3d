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

#ifndef __JAGDRAW_OBJECT_ID_H__
#define __JAGDRAW_OBJECT_ID_H__ 1

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/ContextSupport.h>
#include <jagDraw/PerContextData.h>
#include <jagDraw/types.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class ObjectIDOwner ObjectID.h <jagDraw/ObjectID.h>
\brief Base for classes that own classes derived from ObjectID ("has a" pattern).
\details Examples of classes that own classes derived (either directly or indirectly)
from ObjectID include Drawable, DrawablePrep, and DrawCommand. */
class ObjectIDOwner
{
public:
    ObjectIDOwner() {}
    ObjectIDOwner( const ObjectIDOwner& rhs ) {}
    virtual ~ObjectIDOwner() {}
    

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Used by classes that "have a" (one or more) ObjectID object(s), such as
    Drawable and the DrawElements set of DrawCommand objects. */
    virtual void setMaxContexts( const unsigned int numContexts ) {}

    /** \brief Delete the ID for the given \c contextID.
    \details OpenGL object ID cleanup is not yet implemented. TBD.
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}
};

/*@}*/

typedef jagBase::ptr< jagDraw::ObjectIDOwner >::shared_ptr ObjectIDOwnerPtr;
typedef std::vector< ObjectIDOwnerPtr > ObjectIDOwnerVec;



/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class ObjectID ObjectID.h <jagDraw/ObjectID.h>
\brief Base for classes that own OpenGL object identifiers.
\details For classes that store OpenGL identifiers, possibly multiple IDs need to be
generated, stored, and deleted, one for each active context. ObjectID provides a common
interface for such classes.

Examples of classes deriving from ObjectID include Program, Shader, BufferObject,
Texture, and VertexArrayObject.
*/
class ObjectID
{
public:
    ObjectID() {}
    ObjectID( const ObjectID& rhs )
      : _ids( rhs._ids )
    {}

    virtual ~ObjectID() {}
    

    /** \brief Get an OpenGL identifier for the given \c contextID.
    \details Classes that derive from ObjectID typically override this
    function, returning the OpenGL ID if it has already been allocated,
    and invoking the appropriate OpenGL commands to allocate an ID if
    it hasn't yet been allocated.
    
    The default implementation is provided for classes that don't own OpenGL
    IDs, but inherit indirectly from ObjectID (such as VertexAttrib). */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID )
    {
        return( 0 );
    }
    
    /** \brief Tell the ObjectID object how many contexts to expect.
    \details Resizes the PerContextData vector to accomodate \c numCOntexts IDs,
    one for each context. */
    virtual void setMaxContexts( const unsigned int numContexts )
    {
        _ids._data.resize( numContexts );
    }

    /** \brief Delete the ID for the given \c contextID.
    \details OpenGL object ID cleanup is not yet implemented. TBD.
    */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID ) {}

protected:
    PerContextGLuint _ids;
};

/*@}*/

typedef jagBase::ptr< jagDraw::ObjectID >::shared_ptr ObjectIDPtr;
typedef std::vector< ObjectIDPtr > ObjectIDVec;


// jagDraw
}


// __JAGDRAW_OBJECT_ID_H__
#endif
