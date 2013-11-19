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

#ifndef __JAGDRAW_TRANSFORM_CALLBACK_H__
#define __JAGDRAW_TRANSFORM_CALLBACK_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/Node.h>
#include <jagBase/Transform.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/


/** \addtogroup NearFarComputation Near and Far Plane Computation
\brief Support for computed near/far with draw traversal callbacks.
\details JAG supports non-atomic collection and draw traversals. That is,
the output of the collection (cull) traversal is a draw graph, and the
application can draw that graph as many times as necessary (say, twice for
a stereo rendering, or multiple times with frame-to-frame view coherency).
(For comparison, contrast with OSG, where cull and draw are atomic. For
every cull there is a corresponding draw.)

In order to make multiple draws truly useful, the application will
typically need to modify uniform values for each draw traversal. This
same capability is also required for auto-computed near and far plane
values. See \ref NearFar.

Furthermore, applications sometimes need to set a different uniform value
per draw graph (in multidisplay or multicontext situations).

To meet these needs, JAG supports the concept of a draw traversal callback
that can be attach to each draw graph Node: Node::Callback. Draw
graph executeion calls Node::execute() on each draw graph Node.
Node::execute() iterates over the jagBase::MultiCallback vector of
Callback objects, and if all return true, the Node's Drawable
objects are executed. If any Callback returns false, Node::execute()
immediately returns.

\section NearFar More on Near and Far

Near and far values are computed during collection.
jagSG::CollectionVisitor has a projection matrix, which it uses to define
the cull frustum volume, but this projection matrix doesn't contain the
auto-computed near and far values, because they haven't been computed
until the collection traversal is complete.

After collection, a new projection matrix must be created using the near
and far values, and set as a uniform in the draw graph. Furthermore, during
the draw traversal, concatenations (such as the modelview-projection matrix)
must be computed and also set as uniforms. In the case of the
modelview-projection matrix, this needs to be done every time the model matrix
changes.

To make all this work, the jagSG::CollectionVisitor, DrawGraph,
TransformCallback, and application code must all work together. In pseudocode:

\code
frame loop {
    Set view and projection matrix in CollectionVisitor (to define frustum volume).
    Perform collection traversal.
    Get the DrawGraph from CollectionVisitor.

    Get minZ/maxZ from CollectionVisitor.
    Compute a new projection matrix using near=minZ and far=maxZ.
    Set view and (new) projection matrix in DrawGraph.
    Perform draw traversal.
}
\endcode

Note that the projection and view matrices used by CollectionVisitor to
define the frustum volume are not necessarily the same as the projection and
view matrices used by the DrawGraph, which are ultimately set as uniforms
for use by shader code.

What goes on under the hood?

\li The CollectionVisitor accumulates the jagSG::Node transform matrix as it descends
the scene graph. The accumulated transform matrix is stored in the jagDraw::Node. In
FFP terminology, this is the model part of the modelview matrix.

\li The CollectionVisitor has an instance of a TransformCallback, and adds it to
the jagDraw::Node Callback list to be executed at draw time. This same TransformCallback
is also attached to the DrawGraph that the CollectionVisitor creates. There is only one 
TransformCallback per DrawGraph.

\li After the application computes the new projection matrix using the computed
near and far values, it sets this projection matrix (and view matrix) on the
draw graph. The draw graph actually stores them inside its TransformCallback.

\li During the draw traversal, the TransformCallback is invoked for each
jagDraw::Node. This callback takes the stored projection and view matrices, as
well as the jagDraw::Node's transform (the model matrix), concatenates them
as-needed, creates new Uniform objects for them, and executes the uniforms to send
their values to shader code.

There is some work still remaining to be done:

\li The CollectionVisitor needs to be smarter about which jagDraw::Node objects
actually get a reference to a TransformCallback. Ideally, the first Node that
will be rendered during draw needs one, and subsequent Node objects need one only
if their Transform (model matrix) changes. Currently, however, the COllectionVisitor
is stupid, and sets the TransformCallback on all jagDraw::Node objects.

\li The TransformCallback needs to be smarter about which uniforms to create.
Ideally, this should be based on which uniforms the shader code actually uses. We
could pre-parse the shader code to make this determination. For now, the
TransformCallback hard-codes a couple of transform uniforms used by test shader code.

\li The TransformCallback abuses the heap by allocating Uniform objects. This could
be made more efficient. We could allocate the Uniform on the stack, for example.

*/
/**@{*/


/** \class TransformCallback TransformCallback.h <TransformCallback/Node.h>
\brief A draw node execute callback that specifies transform uniforms.
\details TBD */
class JAGDRAW_EXPORT TransformCallback : public jagDraw::Node::Callback
{
public:
    TransformCallback();
    TransformCallback( const TransformCallback& rhs );
    virtual ~TransformCallback();

    virtual bool operator()( jagDraw::Node& node, jagDraw::DrawInfo& drawInfo );

    jagBase::TransformD& getTransform() { return( _transform ); }
    const jagBase::TransformD& getTransform() const { return( _transform ); }

    /** \brief Specify which matrices are sent as uniforms.
    \details \c flags is from jagBase::Transform (VIEW_PROJ, MODEL_VIEW_PROJ, etc).
    The default is MODEL_VIEW_PROJ | MODEL_VIEW | MODEL_VIEW_INV_TRANS ). */
    void setRequiredMatrixUniforms( const unsigned int flags );
    /** \brief Get flags for matrices that will be sent as uniforms. */
    unsigned int getRequiredMatrixUniforms() const;

    /** \brief Set the name that will be used for the specified matrix uniform.
    \details TBD. */
    void setMatrixUniformName( const unsigned int matrix, const std::string& name );
    /** \brief Get the name that will be used for the specified matrix uniform.
    \details TBD. */
    std::string getMatrixUniformName( const unsigned int matrix ) const;
    /** \brief Restore matrix uniform names to default values.
    \details TBD. */
    void setDefaultMatrixUniformNames();


protected:
    jagBase::TransformD _transform;

    unsigned int _requiredUniforms;

    typedef std::map< unsigned int, std::string > UniformNameMap;
    UniformNameMap _nameMap;
};

typedef jagBase::ptr< jagDraw::TransformCallback >::shared_ptr TransformCallbackPtr;


/**@}*/
/**@}*/


// jagDraw
}


// __JAGDRAW_TRANSFORM_CALLBACK_H__
#endif
