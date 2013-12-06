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

#include <jagDraw/Common.h>

#include <jagBase/Version.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>



using jagDraw::CommandMap;


//#define VERBOST_OUTPUT

class PrintCommands : public jagDraw::CommandMap::Callback
{
public:
    virtual void operator()( jagDraw::DrawablePrepPtr f )
    {
        std::cout << "  " << std::setw(18) <<
            jagDraw::Command::getCommandTypeString( f->getCommandType() ) << ":  " << f->getUserDataName() << std::endl;
    }
};
class CountCommands : public jagDraw::CommandMap::Callback
{
public:
    CountCommands()
    {
        _prog = _vao = _tex = _samp = _us = _ubs = _fb = 0;
    }
    virtual void operator()( jagDraw::DrawablePrepPtr f )
    {
        switch( f->getCommandType() )
        {
        case jagDraw::Command::Program_t: ++_prog; break;
        case jagDraw::Command::VertexArrayObject_t: ++_vao; break;
        case jagDraw::Command::TextureSet_t: ++_tex; break;
        case jagDraw::Command::SamplerSet_t: ++_samp; break;
        case jagDraw::Command::UniformSet_t: ++_us; break;
        case jagDraw::Command::UniformBlockSet_t: ++_ubs; break;
        case jagDraw::Command::Framebuffer_t: ++_fb; break;
        }
    }
    void dump()
    {
        if( _prog > 0 )
            std::cout << "Program: " << _prog << std::endl;
        if( _vao > 0 )
            std::cout << "VAO: " << _vao << std::endl;
        if( _tex > 0 )
            std::cout << "TexSet: " << _tex << std::endl;
        if( _samp > 0 )
            std::cout << "SampSet: " << _samp << std::endl;
        if( _us > 0 )
            std::cout << "UniSet: " << _us << std::endl;
        if( _ubs > 0 )
            std::cout << "UBlkSet: " << _ubs << std::endl;
        if( _fb > 0 )
            std::cout << "FBO: " << _fb << std::endl;
        std::cout << "Total: " << getTotal() << std::endl;
    }
    unsigned int getTotal()
    {
        return( _prog + _vao + _tex + _samp + _us + _ubs + _fb );
    }
    unsigned int _prog, _vao, _tex, _samp, _us, _ubs, _fb;
};


bool test()
{
    jagDraw::ProgramPtr prog0( jagDraw::ProgramPtr( new jagDraw::Program() )); prog0->setUserDataName( "Program0" );
    jagDraw::ProgramPtr prog1( jagDraw::ProgramPtr( new jagDraw::Program() )); prog1->setUserDataName( "Program1" );

    jagDraw::TexturePtr tex0( jagDraw::TexturePtr( new jagDraw::Texture() )); tex0->setUserDataName( "Texture0" );
    jagDraw::TextureSetPtr ts0( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
    (*ts0)[ GL_TEXTURE0 ] = tex0;
    jagDraw::TexturePtr tex1( jagDraw::TexturePtr( new jagDraw::Texture() )); tex1->setUserDataName( "Texture1" );
    jagDraw::TextureSetPtr ts1( jagDraw::TextureSetPtr( new jagDraw::TextureSet() ) );
    (*ts1)[ GL_TEXTURE0 ] = tex1;

    jagDraw::DrawNodeVec nodes;
    for( unsigned int idx=0; idx<8; ++idx )
    {
        jagDraw::CommandMapPtr commands( jagDraw::CommandMapPtr( new CommandMap() ) );
        commands->insert( ( ( idx & 0x1 ) == 0 ) ? prog0 : prog1 );
        commands->insert( ( ( idx & 0x4 ) == 0 ) ? ts0 : ts1 );

        nodes.push_back( jagDraw::DrawNodePtr( new jagDraw::DrawNode( commands ) ) );

#ifdef VERBOST_OUTPUT
        std::cout << "-----" << std::endl;
        commands->foreach( PrintCommands() );
#endif
    }


    {
#ifdef VERBOST_OUTPUT
        std::cout << "=======================================" << std::endl;
        std::cout << "Unsorted: " << std::endl;
#endif

        CountCommands cc;
        CommandMap current;
        for( jagDraw::DrawNodeVec::const_iterator p = nodes.begin(); p != nodes.end(); ++p )
        {
            const jagDraw::DrawNodePtr& np( *p );
            CommandMap delta = current << *( np->getCommandMap() );
#ifdef VERBOST_OUTPUT
            std::cout << "-----" << std::endl;
            std::cout << p->getCommandMap()->_name << std::endl;
            current.foreach( PrintCommands() );
            std::cout << std::endl << "Delta: " << std::endl;
            delta.foreach( PrintCommands() );
#endif
            delta.foreach( cc );
#ifdef VERBOST_OUTPUT
            std::cout << "-----" << std::endl;
#endif
        }
#ifdef VERBOST_OUTPUT
        cc.dump();
#endif

        if( cc._prog != 8 )
        {
            std::cerr << "Unsorted should set 8 programs, sets " << cc._prog << " instead." << std::endl;
            return( false );
        }
        if( cc._tex != 2 )
        {
            std::cerr << "Unsorted should set 2 texture sets, sets " << cc._tex << " instead." << std::endl;
            return( false );
        }
    }

    {
        jagDraw::Command::CommandTypeVec plist;
        plist.push_back( jagDraw::Command::Program_t );
        plist.push_back( jagDraw::Command::TextureSet_t );

        std::sort( nodes.begin(), nodes.end(), jagDraw::DrawNodeCommandSorter( plist ));


#ifdef VERBOST_OUTPUT
        std::cout << "=======================================" << std::endl;
        std::cout << "Sorted: " << std::endl;
#endif


        CountCommands cc;
        CommandMap current;
        for( jagDraw::DrawNodeVec::const_iterator p = nodes.begin(); p != nodes.end(); p++ )
        {
            const jagDraw::DrawNodePtr& np( *p );
            CommandMap delta = current << *( np->getCommandMap() );
#ifdef VERBOST_OUTPUT
            std::cout << "-----" << std::endl;
            std::cout << p->getCommandMap()->_name << std::endl;
            current.foreach( PrintCommands() );
            std::cout << std::endl << "Delta: " << std::endl;
            delta.foreach( PrintCommands() );
#endif
            delta.foreach( cc );
#ifdef VERBOST_OUTPUT
            std::cout << "-----" << std::endl;
#endif
        }
#ifdef VERBOST_OUTPUT
        cc.dump();
#endif

        if( cc._prog != 2 )
        {
            std::cerr << "Sorted should set 2 programs, sets " << cc._prog << " instead." << std::endl;
            return( false );
        }
        if( cc._tex != 4 )
        {
            std::cerr << "Sorted should set 4 texture sets, sets " << cc._tex << " instead." << std::endl;
            return( false );
        }
    }

    return( true );
}

int main( int argc, char** argv )
{
    std::cout << "This is a CTest regression test. To launch under Visual Studio, build the" << std::endl;
    std::cout << "RUN_TESTS target. Under Linux, enter 'make test' at a shell prompt." << std::endl;


    std::cout << std::endl;
    if( test() )
    {
        std::cout << "Test passed." << std::endl;
        return( 0 );
    }
    else
    {
        std::cout << "Test failed." << std::endl;
        return( 1 );
    }
}


