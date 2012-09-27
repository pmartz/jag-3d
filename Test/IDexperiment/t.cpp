#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/OpenGL.h>
//#include "ID.h"

enum VertexAttributeIndex
{
    None_t,
    Vertex_t,
    TexCoord_t,
};

struct Program 
{
    GLint id;
    GLint getLocation( const std::string & ) const { return 10 + (rand() % 25); }
    GLint getAttribIndexLocation( VertexAttributeIndex i ) { return VertexAttributeLocationMap[i]; }
    std::map<VertexAttributeIndex,GLint> VertexAttributeLocationMap;
    Program( GLint _id ):id(_id) 
    {
        VertexAttributeLocationMap[None_t] = -1;
        VertexAttributeLocationMap[Vertex_t] = 0;
        VertexAttributeLocationMap[TexCoord_t] = 2;
    }
};

struct Context 
{
    GLint id;
    Program *currentProgram;

    Context( GLint _id, Program *p ): id(_id), currentProgram(p) {}
};


#if 1
typedef std::map< GLint, GLint > LocationIDMap;
typedef std::map< GLint, GLint > IndexIDMap;
typedef std::map< GLint, GLint > ContextIDMap;
typedef std::map< GLint, LocationIDMap > ContextLocationIDMap;
typedef std::map< GLint, IndexIDMap > ContextIndexIDMap;

class IDGenerator
{
    public:
        /*
        virtual GLint operator()( const Context & ) const { return -1; }
        virtual GLint operator()( const Context &, const std::string & ) const { return -1; }
        virtual GLint operator()( const Context &, VertexAttributeIndex ) const { return -1; }
        */
};
typedef iiMem::ptr<IDGenerator>::shared_ptr IDGenerator_ptr;

//template <typename T>
class ID : public std::map<GLint,GLint>
{
    public:

        ID( const IDGenerator_ptr g ):
            m_generate(g)
        {}


        GLint get(const Context & ctx) 
        {
            if( find( ctx.id ) == end() )
            {
                id[ctx.id] = (*m_generate.get())( ctx );
            }
            return id[ctx.id];
        }
    protected:
        GLint id;

};


/*
class Location
{

        GLint get( const Context &ctx, const std::string &name )
        {
            if( id.find( ctx.id ) == id.end() ||
                id[ctx.id].find( ctx.currentProgram->id ) == id[ctx.id].end() )
            {
                id[ctx.id][ctx.currentProgram->id] = (*m_generate.get())( ctx, name );
            }
            return id[ctx.id][ctx.currentProgram->id];
        }

        GLint get( const Context &ctx, VertexAttributeIndex i )
        {
            if( id.find( ctx.id ) == id.end() ||
                id[ctx.id].find( ctx.currentProgram->id ) == id[ctx.id].end() )
            {
                id[ctx.id][ctx.currentProgram->id] = (*m_generate.get())( ctx, i ); 
            }
            return id[ctx.id][ctx.currentProgram->id];
        }

    protected:
        IDGenerator_ptr m_generate;
};
*/

typedef ID<ContextIDMap> ContextID;
typedef ID<ContextLocationIDMap> ContextLocationID;
typedef ID<ContextIndexIDMap> ContextIndexID;
#endif

class A 
{
    public:
        A( const std::string &_name): 
            name(_name),
            id(IDGenerator_ptr(new Aid_generator))
        {}

        void apply( const Context &context )
        {
            //GLint i = id.get( context, name );
            //GLint i = id.get( context );
            GLint i = id.get( context, TexCoord_t );
            printf("%d\n", i );
        }

    private:
        std::string name;
        ContextIndexID id;

        class Aid_generator: public IDGenerator
        {
            public:
                virtual GLint operator()( const Context &, VertexAttributeIndex ) const 
                { 
                    return rand() % 34;
                }
        };
};

int main( int argc, char **argv )
{
    srand(time(0L));
    A a("one");

    Program *proga = new Program( 345 );
    Program *progb = new Program( 789 );

    Context ctx(123, proga );
    a.apply( ctx );

    ctx.currentProgram = progb;
    a.apply( ctx );

    ctx.currentProgram = proga;
    a.apply( ctx );

    ctx.currentProgram = progb;
    a.apply( ctx );

}
