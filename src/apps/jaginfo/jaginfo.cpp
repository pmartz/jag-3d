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

#include <jagDraw/Common.h>
#include <jagSG/Common.h>
#include <jagDisk/ReadWrite.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>

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



class DrawGraphVisitor
{
public:
    DrawGraphVisitor();
    ~DrawGraphVisitor();

    void reset();
    void dump( std::ostream& ostr );

    void traverse( jagDraw::DrawGraph& drawGraph );

    virtual void visit( jagDraw::NodeContainer& nc );
    virtual void visit( jagDraw::Node& node, jagDraw::NodeContainer& nc );

protected:
    jagDraw::NodeContainerPtr _ncp;

    unsigned int _containers;
    unsigned int _nodes;
};

DrawGraphVisitor::DrawGraphVisitor()
{
    reset();
}
DrawGraphVisitor::~DrawGraphVisitor()
{
}

void DrawGraphVisitor::reset()
{
    _containers = 0;
    _nodes = 0;
}
void DrawGraphVisitor::dump( std::ostream& ostr )
{
    ostr << "            \tTotal" << std::endl;
    ostr << "            \t-----" << std::endl;
    ostr << " Containers:\t" << _containers << std::endl;
    ostr << "      Nodes:\t" << _nodes << std::endl;
}

void DrawGraphVisitor::traverse( jagDraw::DrawGraph& drawGraph )
{
    BOOST_FOREACH( jagDraw::DrawGraph::value_type& nc, drawGraph )
    {
        visit( nc );
        BOOST_FOREACH( jagDraw::NodeContainer::value_type& node, nc )
        {
            visit( node, nc );
        }
    }
}

void DrawGraphVisitor::visit( jagDraw::NodeContainer& nc )
{
    ++_containers;
}
void DrawGraphVisitor::visit( jagDraw::Node& node, jagDraw::NodeContainer& nc )
{
    ++_nodes;
}




class CountVisitor : public jagSG::VisitorBase
{
public:
    CountVisitor();
    ~CountVisitor();

    void reset();
    void dump( std::ostream& ostr );

    virtual void visit( jagSG::Node& node );

    unsigned int _nodes, _uNodes;
    unsigned int _commands, _uCommands;
    unsigned int _drawables, _uDrawables;

protected:
    std::set< jagSG::NodePtr > _nodeSet;
    std::set< jagDraw::CommandMapPtr > _commandSet;
    std::set< jagDraw::DrawablePtr > _drawableSet;
};

CountVisitor::CountVisitor()
    : jagSG::VisitorBase( "count" )
{
    reset();
}
CountVisitor::~CountVisitor()
{
}

void CountVisitor::reset()
{
    _nodes = _uNodes = 0;
    _commands = _uCommands = 0;
    _drawables = _uDrawables = 0;
    _nodeSet.clear();
    _commandSet.clear();
    _drawableSet.clear();
}
void CountVisitor::dump( std::ostream& ostr )
{
    ostr << "            \tTotal\tUnique" << std::endl;
    ostr << "            \t-----\t------" << std::endl;
    ostr << "      Nodes:\t" << _nodes << "\t" << _uNodes << std::endl;
    ostr << "CommandMaps:\t" << _commands << "\t" << _uCommands << std::endl;
    ostr << "  Drawables:\t" << _drawables << "\t" << _uDrawables << std::endl;
}

void CountVisitor::visit( jagSG::Node& node )
{
    ++_nodes;
    jagSG::NodePtr np( node.shared_from_this() );
    if( _nodeSet.find( np ) == _nodeSet.end() )
    {
        ++_uNodes;
        _nodeSet.insert( np );
    }

    jagDraw::CommandMapPtr cp( node.getCommandMap() );
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
        const jagDraw::DrawablePtr dp( node.getDrawable( idx ) );
        if( _drawableSet.find( dp ) == _drawableSet.end() )
        {
            ++_uDrawables;
            _drawableSet.insert( dp );
        }
    }

    node.traverse( *this );
}



int main( int argc, char** argv )
{
    jagBase::Log::instance();

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


    jagSG::NodePtr root( boost::make_shared< jagSG::Node >(
        *(jagSG::Node*) jagDisk::read( fileName ) ) );

    CountVisitor cv;
    root->accept( cv );
    cv.dump( std::cout );

    jagSG::CollectionVisitor collect;
    root->accept( collect );

    DrawGraphVisitor dgv;
    dgv.traverse( *( collect.getDrawGraph() ) );
    dgv.dump( std::cout );

    return( 0 );
}
