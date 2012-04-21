#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Chaskii/Mem/ref_ptr.h>
#include <tr1/memory>
#include "reps.h"

template <typename T>
struct ptr {
#ifdef USE_CHASKII_MEM_REF_PTR
    typedef iiMem::ref_ptr<T> shared_ptr;
#endif
#ifdef USE_STD_SHARED_PTR
    typedef std::tr1::shared_ptr<T> shared_ptr;
#endif
};

static int n_constructs = 0;
static int n_destructs = 0;
static const size_t pagesize = 4096;
static int blah = 0;

double tick()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return double(ts.tv_sec) + double(ts.tv_nsec)/(1e9);
}

class Thing
{
    public:
        Thing() { n_constructs++; }
        ~Thing() { n_destructs++; }

        int touch()
        {
            return buff[pagesize>>1];
        }

    private:
        char buff[pagesize];

};

bool measure_references = false;

static bool doargs( int argc, char **argv )
{
    int nexti = 0;
    for( int i = 1; i < argc; i = nexti )
    {
        nexti = i + 1;
        if( argv[i][0] == '-' )
        {
            for( unsigned int j = 0; j < strlen(argv[i]); j++ )
            {
                switch( argv[i][j] )
                {
                    case 'r':
                        measure_references = true;
                        break;
                }
            }
        }
    }
    return true;
}

int main( int argc, char **argv )
{
    if( doargs( argc, argv ) == false )
        return 1;

    ptr<Thing>::shared_ptr refs[nrefs];

    double t0, t1;
    t0 = tick();

    for( unsigned int i = 0; i < nreps; i++ )
    {
        {
            ptr<Thing>::shared_ptr t(new Thing);

            if( measure_references )
            {
                for( unsigned int j = 0; j < nrefs; j++ )
                {
                    blah += t->touch();
                    refs[j] = t;
                }


                for( unsigned int j = 0; j < nrefs; j++ )
                {
                    refs[j].reset();
                }
            }
        }
    }

    t1 = tick();

    printf("%lf\n", t1 - t0 );
    printf("num constructs: %d\n", n_constructs );
    printf(" num destructs: %d\n", n_destructs );
}
