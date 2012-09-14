#include <math.h>
#include <fcntl.h>
#include <vector>
#include <stdio.h>
#include <Chaskii/DB/Image.h>
#include <Chaskii/DB/ImageLoader.h>
//#include <Chaskii/Math/vec.h>

const float vs = 10000.0f;
const float hs = 10000.0f;

/*
struct rgb
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
*/

#define sqr(x)  ((x)*(x))
struct vec3 
{
    float v[3];
    vec3( float x, float y, float z )
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    void operator += ( const vec3 &rhs )
    {
        v[0] += rhs.v[0];
        v[1] += rhs.v[1];
        v[2] += rhs.v[2];
    }

    vec3 operator + ( const vec3 &rhs )
    {
        return vec3( v[0] + rhs.v[0],
                     v[1] + rhs.v[1],
                     v[2] + rhs.v[2] );
    }

    vec3 operator / ( float d )
    {
        return vec3( v[0]/d,
                     v[1]/d,
                     v[2]/d );
    }

    void operator /= ( float d )
    {
        v[0]/=d;
        v[1]/=d;
        v[2]/=d;
    }


    vec3 operator ^ ( const vec3 &rhs )
    {   
        return vec3(
             (v[1]*rhs.v[2]) - (v[2]*rhs.v[1]),
            -(v[0]*rhs.v[2]) + (v[2]*rhs.v[0]),
             (v[0]*rhs.v[1]) - (v[1]*rhs.v[0]) );
    }

    vec3 operator - ( const vec3 &rhs ) const 
    {    
        return vec3(v[0]-rhs.v[0], v[1]-rhs.v[1], v[2]-rhs.v[2] ); 
    }

    void normalize()
    { 
        float d = sqrtf(sqr(v[0])+sqr(v[1])+sqr(v[2])); 
        if( fabs(d) > 0.0 ) 
        { 
            v[0] /= d; 
            v[1] /= d; 
            v[2] /= d; 
        } 
    }
};

struct triangle
{
    unsigned int a, b, c;
    std::vector<vec3> &v;

    triangle( unsigned int _a, unsigned int _b, unsigned int _c, std::vector<vec3> &_v ):
        a(_a),
        b(_b),
        c(_c),
        v(_v)
    {
    }

    triangle( const triangle &t ):
        a(t.a),
        b(t.b),
        c(t.c),
        v(t.v) 
    {}

    triangle &operator = ( const triangle &t )
    {
        a = t.a;
        b = t.b;
        c = t.c;
        v = t.v;
        return *this;
    }
    
    bool has( unsigned int index )
    {
        if( a == index ) return true;
        if( b == index ) return true;
        if( c == index ) return true;
        return false;
    }

    vec3 normal()
    {
        vec3 A = v[b] - v[a];
        vec3 B = v[c] - v[a];
        return A ^ B;
    }
};

int main( int argc, char ** argv )
{
    iiDB::Image_ptr image( iiDB::ReadImage( "heightMap.jpg" ) );

    if( image == NULL )
        return 1;

    unsigned int w = image->getWidth();
    unsigned int h = image->getWidth();

    unsigned char max = 0;
    unsigned char min = 255;

    for( unsigned int i = 0; i < w * h; i++ )
    {
        unsigned char c = image->getData()[i];
        if( c < min ) min = c;
        if( c > max ) max = c;
    }
    printf("%d - %d\n", min, max );

    std::vector<vec3> array;
    for( unsigned int i = 0; i < h; i++ )
    {
        for( unsigned int j = 0; j < w; j++ )
        {
            array.push_back( vec3(  (float(j)/float(w)) * hs,
                                    (float(i)/float(h)) * hs,
                                    float(image->getData()[i]) * vs ) );
        }
    }


    std::vector<triangle> tris;
    std::map<unsigned int, std::vector<triangle> > triangle_list;
    for( unsigned int i = 0; i < h-1; i++ )
    {
        for( unsigned int j = 0; j < w-1; j++ )
        {
            unsigned int a = ((i+0) * w) + (j+0);
            unsigned int b = ((i+0) * w) + (j+1);
            unsigned int c = ((i+1) * w) + (j+0);
            unsigned int d = ((i+1) * w) + (j+1);

            triangle A( a, b, c, array );
            triangle B( c, b, d, array );

            tris.push_back( A );
            tris.push_back( B );

            triangle_list[a].push_back( A );
            triangle_list[a].push_back( B );
            triangle_list[b].push_back( A );
            triangle_list[b].push_back( B );
            triangle_list[c].push_back( A );
            triangle_list[c].push_back( B );
            triangle_list[d].push_back( A );
            triangle_list[d].push_back( B );
        }
    }


    size_t size = image->getWidth() * image->getHeight() * 3;
    float *fodata = new float[size];
    float *fptr = fodata;
    unsigned char *odata = new unsigned char[size];
    unsigned char *ptr = odata;
    for( std::map<unsigned int, std::vector<triangle> >::iterator p = triangle_list.begin(); p != triangle_list.end(); p++ )
    {
        vec3 n(0,0,0);
        float count = 0.0;
        for( std::vector<triangle>::iterator q = p->second.begin(); q != p->second.end(); q++ )
        {
            n += q->normal();

            count += 1.0;
        }
        n /= count;
        n.normalize();

        /*
printf("%f %f %f\n", n.v[0], n.v[1], n.v[2] );
unsigned char a = (unsigned char)( 255.0 * n.v[0] ); 
unsigned char b = (unsigned char)( 255.0 * n.v[1] ); 
unsigned char c = (unsigned char)( 255.0 * n.v[2] ); 
printf("%d %d %d\n", a, b, c );
        *(ptr++) = a;
        *(ptr++) = b; 
        *(ptr++) = c;
        */
        *(fptr++) = n.v[0];
        *(fptr++) = n.v[1];
        *(fptr++) = n.v[2];
    }


    int fd = open( "normals.raw", O_WRONLY | O_CREAT, 0644 );
    //write( fd, odata, size );
    write( fd, fodata, size * sizeof(float) );
    close(fd);

    return 0;
}
    
