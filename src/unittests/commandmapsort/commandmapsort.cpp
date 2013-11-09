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
            jagDraw::DrawablePrep::getCommandTypeString( f->getCommandType() ) << ":  " << f->getUserDataName() << std::endl;
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
        case jagDraw::DrawablePrep::Program_t: ++_prog; break;
        case jagDraw::DrawablePrep::VertexArrayObject_t: ++_vao; break;
        case jagDraw::DrawablePrep::TextureSet_t: ++_tex; break;
        case jagDraw::DrawablePrep::SamplerSet_t: ++_samp; break;
        case jagDraw::DrawablePrep::UniformSet_t: ++_us; break;
        case jagDraw::DrawablePrep::UniformBlockSet_t: ++_ubs; break;
        case jagDraw::DrawablePrep::Framebuffer_t: ++_fb; break;
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

        nodes.push_back( jagDraw::DrawNodePtr( new jagDraw::Node( commands ) ) );

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
        jagDraw::DrawablePrep::CommandTypeVec plist;
        plist.push_back( jagDraw::DrawablePrep::Program_t );
        plist.push_back( jagDraw::DrawablePrep::TextureSet_t );

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


