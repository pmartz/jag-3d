#include <stdio.h>
#include <unistd.h>

int main( int argc, char **argv )
{
    printf("system pagesize: %d\n",getpagesize() );
}

