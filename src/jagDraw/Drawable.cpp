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

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/Program.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/VertexAttribContainer.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>
#include <jagBase/Profile.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagDraw {


Drawable::Drawable( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.drawable" : logName ),
    ObjectIDOwner(),
    BoundOwner(),
    jagBase::Notifier()
{
}
Drawable::Drawable( const Drawable& rhs )
  : jagBase::LogBase( rhs ),
    ObjectIDOwner( rhs ),
    BoundOwner( rhs ),
    jagBase::Notifier( rhs ),
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
    if( sz == 0 )
        return;

    for( size_t idx = 0; idx < sz; ++idx )
        _drawCommands[ idx ]->execute( drawInfo );

    JAG3D_ERROR_CHECK( "Drawable::execute()" );
}


BoundPtr Drawable::newBound()
{
    return( BoundPtr( new BoundAABox() ) );
}

void Drawable::computeBound( BoundPtr& bound, const jagDraw::CommandMap& commands )
{
    JAG3D_PROFILE( "Drawable::computeBound" );

    const jagDraw::DrawablePrepPtr& drawablePrep( commands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    const jagDraw::VertexArrayObject* vao( ( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) ).get() );
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
void Drawable::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->deleteID( contextID );
    }
}


// jagDraw
}
