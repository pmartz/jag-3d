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

#include <jag/base/Version.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>



using jag::draw::CommandMap;


//#define VERBOST_OUTPUT

class PrintCommands : public jag::draw::CommandMap::Callback
{
public:
    virtual void operator()( jag::draw::CommandPtr f )
    {
        std::cout << "  " << std::setw(18) <<
            jag::draw::Command::getCommandTypeString( f->getCommandType() ) << ":  " << f->getUserDataName() << std::endl;
    }
};
class CountCommands : public jag::draw::CommandMap::Callback
{
public:
    CountCommands()
    {
        _prog = _vao = _tex = _samp = _us = _ubs = _fb = 0;
    }
    virtual void operator()( jag::draw::CommandPtr f )
    {
        switch( f->getCommandType() )
        {
        case jag::draw::Command::Program_t: ++_prog; break;
        case jag::draw::Command::VertexArrayObject_t: ++_vao; break;
        case jag::draw::Command::TextureSet_t: ++_tex; break;
        case jag::draw::Command::SamplerSet_t: ++_samp; break;
        case jag::draw::Command::UniformSet_t: ++_us; break;
        case jag::draw::Command::UniformBlockSet_t: ++_ubs; break;
        case jag::draw::Command::Framebuffer_t: ++_fb; break;
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
    jag::draw::ProgramPtr prog0( jag::draw::ProgramPtr( new jag::draw::Program() )); prog0->setUserDataName( "Program0" );
    jag::draw::ProgramPtr prog1( jag::draw::ProgramPtr( new jag::draw::Program() )); prog1->setUserDataName( "Program1" );

    jag::draw::TexturePtr tex0( jag::draw::TexturePtr( new jag::draw::Texture() )); tex0->setUserDataName( "Texture0" );
    jag::draw::TextureSetPtr ts0( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );
    (*ts0)[ GL_TEXTURE0 ] = tex0;
    jag::draw::TexturePtr tex1( jag::draw::TexturePtr( new jag::draw::Texture() )); tex1->setUserDataName( "Texture1" );
    jag::draw::TextureSetPtr ts1( jag::draw::TextureSetPtr( new jag::draw::TextureSet() ) );
    (*ts1)[ GL_TEXTURE0 ] = tex1;

    jag::draw::DrawNodeVec nodes;
    for( unsigned int idx=0; idx<8; ++idx )
    {
        jag::draw::CommandMapPtr commands( jag::draw::CommandMapPtr( new CommandMap() ) );
        commands->insert( ( ( idx & 0x1 ) == 0 ) ? prog0 : prog1 );
        commands->insert( ( ( idx & 0x4 ) == 0 ) ? ts0 : ts1 );

        nodes.push_back( jag::draw::DrawNodePtr( new jag::draw::DrawNode( commands ) ) );

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
        for( jag::draw::DrawNodeVec::const_iterator p = nodes.begin(); p != nodes.end(); ++p )
        {
            const jag::draw::DrawNodePtr& np( *p );
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
        jag::draw::Command::CommandTypeVec plist;
        plist.push_back( jag::draw::Command::Program_t );
        plist.push_back( jag::draw::Command::TextureSet_t );

        std::sort( nodes.begin(), nodes.end(), jag::draw::DrawNodeCommandSorter( plist ));


#ifdef VERBOST_OUTPUT
        std::cout << "=======================================" << std::endl;
        std::cout << "Sorted: " << std::endl;
#endif


        CountCommands cc;
        CommandMap current;
        for( jag::draw::DrawNodeVec::const_iterator p = nodes.begin(); p != nodes.end(); p++ )
        {
            const jag::draw::DrawNodePtr& np( *p );
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


