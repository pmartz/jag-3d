#include <stdio.h>

#include <Chaskii/Math/vec2.h>
#include <Chaskii/Math/matrix.h>

extern void func(void);

int main( int argc, char **argv )
{
    iiMath::vec2i v0(2,3);
    iiMath::vec2i v1(4,5);
    iiMath::vec2i v2 = v0 + v1;

    printf("%d, %d\n", v2[0], v2[1] );

    iiMath::matrix4f m;
    iiMath::matrix4f n;
    iiMath::matrix4f o;
     
    o = m * n;

    func();


    return 0;
}
