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

#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/Program.h>
#include <jag/draw/VertexArrayObject.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/DrawCommand.h>
#include <jag/draw/VertexAttribContainer.h>
#include <jag/draw/DrawInfo.h>
#include <jag/draw/Error.h>
#include <jag/base/LogMacros.h>
#include <jag/base/Profile.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jag {
namespace draw {


Drawable::Drawable( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.drawable" : logName ),
    ObjectIDOwner(),
    BoundOwner(),
    jag::base::Notifier()
{
}
Drawable::Drawable( const Drawable& rhs )
  : jag::base::LogBase( rhs ),
    ObjectIDOwner( rhs ),
    BoundOwner( rhs ),
    jag::base::Notifier( rhs ),
    _drawCommands( rhs._drawCommands )
{
}
Drawable::~Drawable()
{
}


void Drawable::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "Drawable::execute" );

    const size_t sz( _drawCommands.size() );
    for( size_t idx = 0; idx < sz; ++idx )
        _drawCommands[ idx ]->execute( drawInfo );

    JAG3D_ERROR_CHECK( "Drawable::execute()" );
}


BoundPtr Drawable::newBound()
{
    return( BoundPtr( new BoundAABox() ) );
}

void Drawable::computeBound( BoundPtr& bound, const jag::draw::CommandMap& commands )
{
    JAG3D_PROFILE( "Drawable::computeBound" );

    const jag::draw::DrawablePrepPtr& drawablePrep( commands[ jag::draw::Command::VertexArrayObject_t ] );
    const jag::draw::VertexArrayObject* vao( ( boost::static_pointer_cast< jag::draw::VertexArrayObject >( drawablePrep ) ).get() );
    if( vao == NULL )
    {
        JAG3D_WARNING( "computeBound(): NULL vertex array object." );
        return;
    }

    BufferObjectPtr bop( boost::dynamic_pointer_cast< BufferObject >(
        vao->getVertexArrayCommand( VertexArrayCommand::BufferObject_t, VertexArrayObject::Vertex ) ) );
    VertexAttribPtr verts( boost::dynamic_pointer_cast< VertexAttrib >(
        vao->getVertexArrayCommand( VertexArrayCommand::VertexAttrib_t, VertexArrayObject::Vertex ) ) );
    if( ( bop == NULL ) || ( verts == NULL ) )
    {
        JAG3D_WARNING( "computeBound(): NULL buffer object or vertex attrib (VAO has no vertex data)." );
        return;
    }

    GLint size;
    GLenum type;
    verts->getSizeType( size, type );
    typedef std::pair< GLint, GLenum > IntEnum;
    const IntEnum vertType( size, type );

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        if( vertType == IntEnum( 3, GL_FLOAT ) )
        {
            VertexAttribContainer< gmtl::Point3f > vac( bop, verts, dcp );
            VertexAttribContainer< gmtl::Point3f >::iterator pointIter;
            for( pointIter = vac.begin(); pointIter != vac.end(); ++pointIter )
                bound->expand( *pointIter );
        }
        else if( vertType == IntEnum( 3, GL_DOUBLE ) )
        {
            VertexAttribContainer< gmtl::Point3d > vac( bop, verts, dcp );
            VertexAttribContainer< gmtl::Point3d >::iterator pointIter;
            for( pointIter = vac.begin(); pointIter != vac.end(); ++pointIter )
                bound->expand( *pointIter );
        }
        else
        {
            std::ostringstream ostr;
            ostr << "computeBound(): Unsupported VertexAttrib type/size combination.\n";
            ostr << "\tType: " << std::hex << type << ", size: " << std::dec << size;
            JAG3D_ERROR( ostr.str() );
        }
    }
}

void Drawable::setBoundDirty( const bool dirty )
{
    if( _dirty == dirty )
        return;

    BoundOwner::setBoundDirty( dirty );
    if( dirty )
    {
        BoundDirtyNotifyInfo bdni;
        notify( bdni );
    }
}


void Drawable::addDrawCommand( DrawCommandPtr dcp )
{
    _drawCommands.push_back( dcp );
    setBoundDirty();
}

DrawCommandVec& Drawable::getDrawCommandVec()
{
    return( _drawCommands );
}
const DrawCommandVec& Drawable::getDrawCommandVec() const
{
    return( _drawCommands );
}


void Drawable::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->setMaxContexts( numContexts );
    }
}
void Drawable::deleteID( const jag::draw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->deleteID( contextID );
    }
}


// namespace jag::draw::
}
}
