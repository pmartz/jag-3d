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

#include <jag/draw/Common.h>
#include <jag/sg/Common.h>
#include <jag/util/DrawGraphCountVisitor.h>
#include <jag/util/BufferAggregationVisitor.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/Log.h>
#include <jag/base/LogMacros.h>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <string>
#include <sstream>
#include <set>

namespace bpo = boost::program_options;

std::string logstr( "jag.app.info" );



class SceneGraphCountVisitor : public jag::sg::VisitorBase
{
public:
    SceneGraphCountVisitor();
    ~SceneGraphCountVisitor();

    void reset();
    void dump( std::ostream& ostr );

    virtual void visit( jag::sg::Node& node );

    unsigned int _nodes, _uNodes;
    unsigned int _commands, _uCommands;
    unsigned int _drawables, _uDrawables;

protected:
    std::set< jag::sg::NodePtr > _nodeSet;
    std::set< jag::draw::CommandMapPtr > _commandSet;
    std::set< jag::draw::DrawablePtr > _drawableSet;
};

SceneGraphCountVisitor::SceneGraphCountVisitor()
    : jag::sg::VisitorBase( "count" )
{
    reset();
}
SceneGraphCountVisitor::~SceneGraphCountVisitor()
{
}

void SceneGraphCountVisitor::reset()
{
    _nodes = _uNodes = 0;
    _commands = _uCommands = 0;
    _drawables = _uDrawables = 0;
    _nodeSet.clear();
    _commandSet.clear();
    _drawableSet.clear();
}
void SceneGraphCountVisitor::dump( std::ostream& ostr )
{
    ostr << "            \tTotal\tUnique" << std::endl;
    ostr << "            \t-----\t------" << std::endl;
    ostr << "      Nodes:\t" << _nodes << "\t" << _uNodes << std::endl;
    ostr << "CommandMaps:\t" << _commands << "\t" << _uCommands << std::endl;
    ostr << "  Drawables:\t" << _drawables << "\t" << _uDrawables << std::endl;
}

void SceneGraphCountVisitor::visit( jag::sg::Node& node )
{
    ++_nodes;
    jag::sg::NodePtr np( node.shared_from_this() );
    if( _nodeSet.find( np ) == _nodeSet.end() )
    {
        ++_uNodes;
        _nodeSet.insert( np );
    }

    jag::draw::CommandMapPtr cp( node.getCommandMap() );
    if( cp != NULL )
    {
        ++_commands;
        if( _commandSet.find( cp ) == _commandSet.end() )
        {
            ++_uCommands;
            _commandSet.insert( cp );
        }
    }

    for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
    {
        ++_drawables;
        const jag::draw::DrawablePtr dp( node.getDrawable( idx ) );
        if( _drawableSet.find( dp ) == _drawableSet.end() )
        {
            ++_uDrawables;
            _drawableSet.insert( dp );
        }
    }

    node.traverse( *this );
}


// Support for linking statically
JAG3D_REFERENCE_PLUGIN( osgImage );
JAG3D_REFERENCE_PLUGIN( osgModel );

int main( int argc, char** argv )
{
    jag::base::Log::instance();

    bpo::options_description desc( "Options" );
    desc.add_options()
        ( "help,h", "Help text" )
        ( "file,f", bpo::value< std::string >(), "File to load." )
    ;

    bpo::variables_map vm;
    bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
    bpo::notify( vm );

    if( vm.count( "help" ) > 0 )
    {
        std::cout << desc << std::endl;
        return( 1 );
    }

    std::string fileName;
    if( vm.count( "file" ) > 0 )
    {
        fileName = vm[ "file" ].as< std::string >();
        JAG3D_INFO_STATIC( logstr, "Loading \""+fileName+"\"" );
    }
    else
    {
        JAG3D_FATAL_STATIC( logstr, "Must specify \"-f <file>\"." );
        std::cerr << desc << std::endl;
        return( 1 );
    }


    boost::any anyRoot( jag::disk::read( fileName ) );
    jag::sg::NodePtr root;
    try {
        root = boost::any_cast< jag::sg::NodePtr >( anyRoot );
    } catch( boost::bad_any_cast bac ) { bac.what(); }
    if( root == NULL )
    {
        JAG3D_FATAL_STATIC( logstr, "Can't load \"" + fileName + "\"." );
        return( false );
    }


    {
        jag::util::BufferAggregationVisitor bav( root );
    }

    {
        SceneGraphCountVisitor sgcv;
        root->accept( sgcv );
        sgcv.dump( std::cout );
    }

    jag::sg::CollectionVisitor collect;
    collect.setNearFarOps( jag::sg::CollectionVisitor::None );
    gmtl::Matrix44d proj;
    gmtl::setOrtho( proj, -DBL_MAX, DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX );
    collect.setViewProj( gmtl::MAT_IDENTITY44D, proj );
    root->accept( collect );

    jag::draw::DrawGraphPtr drawGraph( collect.getDrawGraph() );

    {
        jag::draw::Command::CommandTypeVec plist;
        plist.push_back( jag::draw::Command::UniformBlockSet_t );
        BOOST_FOREACH( jag::draw::DrawNodeContainer& nc, *drawGraph )
        {
            std::sort( nc.begin(), nc.end(), jag::draw::DrawNodeCommandSorter( plist ) );
        }
    }

    {
        jag::util::DrawGraphCountVisitor dgcv;
        dgcv.traverse( *drawGraph );
        dgcv.dump( std::cout );
    }

    return( 0 );
}


/** \defgroup appJagInfo The jaginfo Application
This application displays information about the scene graph and draw graph
for a loaded model.

Example output:
\code
> jaginfo -f <some-model>
                Total   Unique
                -----   ------
      Nodes:    113     113
CommandMaps:    29      29
  Drawables:    28      28
                Total
                -----
 Containers:    1
      Nodes:    28
  Drawables:    28
  CM deltas:    7
\endcode
The first section of the output shows counts for the scene graph.

The second section shows counts for the draw graph, including CM deltas
(non-empty changes in the CommandMap during the draw traversal, i.e.,
OpenGL state changes).

Command line options:
\li --help,-h Help text
\li --file,-f Model to load.
*/
