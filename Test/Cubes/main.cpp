#include <set>
#include <stdio.h>
#include <fcntl.h>
#include <Chaskii/Mem/ptr.h>
#include <Chaskii/OpenGL.h>
//#include <Chaskii/Draw/Drawable.h>
#include <Chaskii/Draw/ShaderProgram.h>
#include <Chaskii/Draw/DrawableAttribute.h>
#include <Chaskii/Draw/BufferObject.h>
#include <Chaskii/Draw/VertexAttribute.h>
#include <Chaskii/Draw/VertexIterator.h>

#include <Chaskii/WindowSystem/WS.h>
#include <Chaskii/WindowSystem/OpenGLWindow.h>
#include <Chaskii/WindowSystem/Keychar.h>

#include <Chaskii/Math/matrix.h>
#include <Chaskii/Draw/DrawContext.h>
#include <Chaskii/Draw/Image.h>
#include <Chaskii/Draw/Texture.h>
#include <Chaskii/Draw/Transform.h>
#include <Chaskii/DB/ImageLoader.h>

//#include "Transp.h"

#include "Trackball.h"
#include "BooleanToggle.h"
#include "Cube.h"
#include "FrameRate.h"
#include "StateControl.h"
#include "categorized_set.h"

#define NUM_CUBES 2800
//#define NUM_CUBES 5600
//#define NUM_CUBES 16000
//#define NUM_CUBES 8


double tick()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return double(ts.tv_sec) + double(ts.tv_nsec)/(1e9);
}


template <unsigned N>
void print_matrix( const gmtl::Matrix<float,N,N> &m )
{
    for( unsigned int i = 0; i < N; i++ )
    {
        printf("\t");
        for( unsigned int j = 0; j < N; j++ )
            printf(" %8.4f", m(i,j));
        printf("\n" );
    }
}

sortable_ptr<iiDraw::DrawableAttribute> initTexture( const char *filename, iiDraw::DrawableAttribute::DrawableAttributeType t )
{
    iiDB::Image_ptr image( iiDB::ReadImage( filename ) );
    return sortable_ptr<iiDraw::DrawableAttribute>( new iiDraw::Texture( iiDB::Image_ptr(image), t ));
}

sortable_ptr<iiDraw::DrawableAttribute> initTexture( GLubyte color[3], iiDraw::DrawableAttribute::DrawableAttributeType t )
{
    iiDB::Image_ptr image( new iiDB::Image );

    image->data = new GLbyte[256*256*3];
     GLbyte *ptr = image->data.get();
    bool flag = false;
    for( int i = 0; i < 256; i++ )
    {
        if( !(i%16) ) flag = !flag;
        for( int j = 0; j < 256; j++ )
        {
            if( !(j%16)) flag = !flag;
            if( flag )
            {
                *(ptr++) = color[0];
                *(ptr++) = color[1];
                *(ptr++) = color[2];
            }
            else
            {
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
                *(ptr++) = 0xFF;
            }
        }
    }

    image->width = 256;
    image->height = 256;
    image->depth = 0;
    image->numComponents = 3;
    image->glformat = GL_RGB;
    image->gltarget = GL_TEXTURE_2D;
    image->gltype = GL_UNSIGNED_BYTE;

    image->name = std::string("checkers");

    return sortable_ptr<iiDraw::DrawableAttribute>( new iiDraw::Texture( iiDB::Image_ptr(image), t ) );
}

void initTextures( sortable_ptr<iiDraw::DrawableAttribute> t0[8], sortable_ptr<iiDraw::DrawableAttribute> t1[8]  )
{
    GLubyte colors[][3] = {
        { 0x00, 0x00, 0x00 },
        { 0xFF, 0x00, 0x00 },
        { 0x00, 0xFF, 0x00 },
        { 0x00, 0x00, 0xFF },
        { 0xFF, 0xFF, 0x00 },
        { 0x00, 0xFF, 0xFF },
        { 0xFF, 0x00, 0xFF },
        { 0xFF, 0xFF, 0xFF },
    };

    const char *files[] = {
        "images/donald_duck.jpg",
        "images/earth_256.jpg",
        "images/flower.jpg",
        "images/jaguar.jpg",
        "images/puma.jpg",
        "images/sunset.jpg",
        "images/tiger.jpg",
        "images/tree.jpg"
    };

    for( int i = 0; i < 8; i++ )
    {
        t0[i] = initTexture( colors[i], iiDraw::DrawableAttribute::Texture0_t );
        t1[i] = initTexture( files[i],  iiDraw::DrawableAttribute::Texture1_t );
    }
}

std::map<iiDraw::DrawableAttribute::DrawableAttributeType, iiDraw::DrawableAttribute *> yy;

struct Buff{
    size_t size;
    GLbyte *buffer;
    Buff() {}
    Buff( const Buff &b):
        size(b.size),
        buffer(b.buffer) {}
};

void combineVBOs( std::vector<iiDraw::Drawable_ptr> &d )
{
    std::vector<Buff> buffs;
    size_t offset = 0;
    size_t size = 0;
    for( std::vector<iiDraw::Drawable_ptr>::iterator p = d.begin(); p != d.end(); p++ )
    {
        iiDraw::Drawable_ptr s = *p;
        sortable_ptr<iiDraw::DrawableAttribute> q = (*(s.get()))[iiDraw::DrawableAttribute::BufferObject_t];
        iiDraw::BufferObject *bo = dynamic_cast<iiDraw::BufferObject *>(q.get());
        if( bo != NULL && bo->getTarget() == iiDraw::BufferObject::ArrayBuffer )
        {
            size = bo->getBufferSize();
            Buff b;
            b.size = size;
            b.buffer = new GLbyte[size];
            memcpy( b.buffer, bo->map( iiDraw::BufferObject::ReadOnly ), size );
            buffs.push_back( b );

            bo->unmap();
        }

        sortable_ptr<iiDraw::DrawableAttribute> v = (*(s.get()))[iiDraw::DrawableAttribute::VertexAttributeList_t];
        if( v != NULL )
        {
            iiDraw::VertexAttributeList *vl = dynamic_cast<iiDraw::VertexAttributeList *>(v.get());
            for( iiDraw::VertexAttributeList::iterator r = vl->begin(); r != vl->end(); r++ )
            {
                r->get()->m_offset += offset;
            }
        }

        offset += size;
    }


    iiMem::Buffer_ptr vbuffer(new iiMem::Buffer( offset ));
    offset = 0;
    for( std::vector<Buff>::iterator p = buffs.begin(); p != buffs.end(); p++ )
    {
        vbuffer->copy( p->buffer, p->size, offset );
        offset += p->size;
    }

    sortable_ptr<iiDraw::DrawableAttribute> vbo( new iiDraw::ArrayBuffer( vbuffer, iiDraw::BufferObject::StaticDraw ) );

    std::vector<GLint> firstList;
    std::vector<GLsizei> countList;
    GLint first = 0;
    for( int i = 0; i < 6; i++ )
    {
        firstList.push_back( first );
        countList.push_back( 4 );
        first += 4;
    }

    iiDraw::VertexIteratorList * vilist = new iiDraw::VertexIteratorList;
    vilist->push_back( iiDraw::VertexIterator_ptr( new iiDraw::MultiDrawArrays( GL_TRIANGLE_STRIP, firstList, countList, firstList.size() )));
    sortable_ptr<iiDraw::DrawableAttribute> vit( vilist );

    for( std::vector<iiDraw::Drawable_ptr>::iterator p = d.begin(); p != d.end(); p++ )
    {
        (*p)->addAttribute( vbo );

        // This should work but doesn't
         //(*p)->addAttribute( vit );
    }

}

#define sqr(x)  ((x)*(x))

class CubeSorter
{
    public:
        enum SortDirection{
            FrontToBack,
            BackToFront
        };

        CubeSorter( const iiMath::vec3 eye, SortDirection dir ):
            m_eye(eye),
            m_dir(dir)
        {
        }

        bool operator() (  const iiDraw::Drawable_ptr &lhs, const iiDraw::Drawable_ptr &rhs )
        {
            iiMath::vec3 lv = lhs->m_bound.center - m_eye;
            iiMath::vec3 rv = rhs->m_bound.center - m_eye;

            switch( m_dir )
            {
                case BackToFront: return gmtl::length<double,3>(lv ) > gmtl::length<double,3>(rv);
                case FrontToBack: return gmtl::length<double,3>(lv ) < gmtl::length<double,3>(rv);
            }
        }
    private:
        iiMath::vec3 m_eye;
        SortDirection m_dir;

};


int main( int argc, char **argv )
{
    iiWS::OpenGLWindow *win = new iiWS::OpenGLWindow;
    win->setPositionAndSize( 3840, 0, 1920, 1200 );
    //win->setPositionAndSize( 0, 0, 300, 400 );
    win->realize();

    {
        int max_texture_size =  0;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_texture_size );
        int max_3D_texture_size = 0;
        glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &max_3D_texture_size );

        int max_combined_texture_image_units = 0;
        int max_texture_image_units = 0;
        int max_texture_units = 0;

        glGetIntegerv( GL_MAX_TEXTURE_UNITS, &max_texture_units );
        glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units );
        glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units );

        printf("   max_texture_size: %d\n", max_texture_size );
        printf("max_3d_texture_size: %d\n", max_3D_texture_size );
        printf("max_texture_units: %d\n", max_texture_units );
        printf("max_texture_image_units: %d\n", max_texture_image_units );
        printf("max_combined_texture_image_units: %d\n", max_combined_texture_image_units );
    }

    unsigned int wwidth, wheight;
    win->getSize( wwidth, wheight );

    iiWS::WS::instance()->setBlockOnEvents( false );

    bool done = false;
    BooleanToggle_ptr bt( new BooleanToggle( iiWS::KeyChar_Escape, done ) );

    new StateControl;

    iiDraw::DrawContext context( win->getContextID() );

    iiDraw::ShaderProgram_ptr shader(new iiDraw::ShaderProgram );
    iiDraw::ShaderProgram::SourceList l;
    l.add( GL_VERTEX_SHADER, "ii_light.glsl" );
    l.add( GL_VERTEX_SHADER, "vtx.glsl" );
    l.add( GL_FRAGMENT_SHADER, "frg.glsl" );
    shader->fromSourceFileList( l );

    shader->bindAttribLocation( 0, "ii_Vertex3" );
    shader->bindAttribLocation( 1, "ii_TexCoord20" );
    shader->bindAttribLocation( 2, "ii_Normal" );
    shader->bindAttribLocation( 3, "ii_Color4" );

    //shader->apply( context );
    shader->apply( );

    shader->setUniformValue( "texunit0", iiDraw::UniformValue( 0 ));
    shader->setUniformValue( "texunit1", iiDraw::UniformValue( 1 ));

    shader->setUniformValue( "ii_FrontMaterial.ambient",     iiDraw::UniformValue( iiMath::vec4f( 0.2, 0.2, 0.2, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.diffuse",     iiDraw::UniformValue( iiMath::vec4f( 0.8, 0.8, 0.8, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.specular",    iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));
    shader->setUniformValue( "ii_FrontMaterial.shininess",   iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));

    shader->setUniformValue( "ii_LightSource[0].ambient",    iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 0.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].diffuse",    iiDraw::UniformValue( iiMath::vec4f( 1.0, 1.0, 1.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].specular",   iiDraw::UniformValue( iiMath::vec4f( 1.0, 1.0, 1.0, 1.0 )));
    shader->setUniformValue( "ii_LightSource[0].position",   iiDraw::UniformValue( iiMath::vec4f( 0.0, 0.0, 1.0, 1.0 )));

    sortable_ptr<iiDraw::DrawableAttribute> tex0[8];
    sortable_ptr<iiDraw::DrawableAttribute> tex1[8];
    initTextures( tex0, tex1 );

    //std::vector<sortable_ptr<iiDraw::Drawable> > drawables;
    //std::vector< iiDraw::Drawable * > drawables;
    //std::vector< std::tr1::shared_ptr<iiDraw::Drawable> > drawables;

    std::vector< iiDraw::Drawable_ptr > t_drawables;
    std::vector< iiDraw::Drawable_ptr > o_drawables;


#if 0
    for( int i = 0; i < 3; i++ )
    {
        iiDraw::Drawable_ptr cube(new Cube(0.0, 0.0, -3.0 + (i * 3.0), true ) );
        cube->addAttribute( tex0[i] );
        cube->gfxInit( context );
        t_drawables.push_back( cube );
    }
#endif

#if 1
    float range = 200.0;
    float range_2 = range * 0.5;
    for( int i = 0; i < NUM_CUBES; i++ )
    {
        float tx = -range_2 + range * (rand()%1000)/1000.0;
        float ty = -range_2 + range * (rand()%1000)/1000.0;
        float tz = -range_2 + range * (rand()%1000)/1000.0;

        bool transp = false;//rand()%2;

        //std::tr1::shared_ptr<iiDraw::Drawable> cube(new Cube(tx,ty,tz));

        iiDraw::Drawable_ptr cube(new Cube(tx,ty,tz, transp));

        //iiDraw::Drawable *cube( new Cube(tx,ty,tz));
        //sortable_ptr<iiDraw::Drawable> cube(new Cube( tx, ty, tz ));
        //Cube cube(tx,ty,tz);
        //cube->addAttribute( iiDraw::Transform_ptr( new iiDraw::Transform( iiMath::Translatef( tx, ty, tz ))));

        cube->addAttribute( tex0[rand()%8] );
        cube->addAttribute( tex1[rand()%8] );

        cube->gfxInit( context );

        if( transp )
            t_drawables.push_back( cube );
        else
            o_drawables.push_back( cube );

        //drawables.push_back( cube );
        //drawables.insert( cube );
    }
#endif


    combineVBOs( o_drawables );

    priority_list<iiDraw::DrawableAttribute::DrawableAttributeType> plist;
    plist.push_back( iiDraw::DrawableAttribute::ShaderProgram_t );
    plist.push_back( iiDraw::DrawableAttribute::BufferObject_t );
    plist.push_back( iiDraw::DrawableAttribute::Texture0_t );
    plist.push_back( iiDraw::DrawableAttribute::Texture1_t );
    plist.push_back( iiDraw::DrawableAttribute::Transform_t );
    plist.push_back( iiDraw::DrawableAttribute::VertexAttributeList_t);
    plist.push_back( iiDraw::DrawableAttribute::VertexIteratorList_t );

    /*
    puts("==== B4 sort:" );
    for( std::vector<iiDraw::Drawable *>::iterator p = drawables.begin(); p != drawables.end(); p++ )
    {
        iiDraw::Drawable *d = *p;
        iiDraw::Texture *t0 = dynamic_cast<iiDraw::Texture *>((*d)[iiDraw::DrawableAttribute::Texture0_t].get());
        iiDraw::Texture *t1 = dynamic_cast<iiDraw::Texture *>((*d)[iiDraw::DrawableAttribute::Texture1_t].get());
        printf(" %d  %d\n", t0->getID(), t1->getID() );
    }
    */

    categorized_set_sorter< iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> > css( plist );
    std::sort( o_drawables.begin(), o_drawables.end(), css );


    /*
    puts("==== After sort:" );
    for( std::vector<iiDraw::Drawable *>::iterator p = drawables.begin(); p != drawables.end(); p++ )
    {
        iiDraw::Drawable *d = *p;
        iiDraw::Texture *t0 = dynamic_cast<iiDraw::Texture *>((*d)[iiDraw::DrawableAttribute::Texture0_t].get());
        iiDraw::Texture *t1 = dynamic_cast<iiDraw::Texture *>((*d)[iiDraw::DrawableAttribute::Texture1_t].get());
        printf(" %d  %d\n", t0->getID(), t1->getID() );
    }
    */


    Trackball * trackball = new Trackball;
    trackball->setOrientation( Trackball::Y_UP);
    trackball->setDistance( 10.0 );
    trackball->setReference();
    trackball->mapButtonState( 5, Trackball::UpdatePan );

    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_CULL_FACE );
    //glFrontFace( GL_CCW );
    //glCullFace( GL_BACK );
    //glClearColor( 0.3, 0.4, 0.3, 1.0 );
    glClearColor( 0.2, 0.2, 0.4, 1.0 );

    iiMath::matrix4f P( iiMath::Frustumf(  -.38, .38, -.24, .24, 1., 1000.0 ) );
    context.setProjectionMatrix( P );
    context.setShaderProgram( shader );

    FrameRate fr;


    yy[iiDraw::DrawableAttribute::ShaderProgram_t]   = NULL;
    yy[iiDraw::DrawableAttribute::Texture0_t]        = NULL;
    yy[iiDraw::DrawableAttribute::Texture1_t]        = NULL;
    yy[iiDraw::DrawableAttribute::Transform_t]       = NULL;
    yy[iiDraw::DrawableAttribute::BufferObject_t]    = NULL;
    yy[iiDraw::DrawableAttribute::VertexAttributeList_t] = NULL;
    yy[iiDraw::DrawableAttribute::VertexIteratorList_t]  = NULL;

    iiDraw::Drawable default_drawable;
    iiDraw::Drawable current_drawable;

    std::vector< sortable_ptr<iiDraw::DrawableAttribute> > dd;

    double t0 = tick();
#if 0
    // Unsorted
    for( std::vector< iiDraw::Drawable_ptr >::iterator p = o_drawables.begin(); p != o_drawables.end(); p++ )
    {
        for( iiDraw::Drawable::iterator q = (*p)->begin(); q != (*p)->end(); q++ )
        {
            dd.push_back( q->second);
        }

    }
#else
    // Sorted
    double optime = 0.0;
    for( std::vector< iiDraw::Drawable_ptr >::iterator p = o_drawables.begin(); p != o_drawables.end(); p++ )
    {
            // 2.0 THe >> operator turns out to be miserably slow
            iiDraw::Drawable &temp = *(p->get());
            //double it0 = tick();
            categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> > diff = (temp) >> current_drawable;
            //double it1 = tick();
            //optime += it1 - it0;

            for( categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> >::iterator q = diff.begin();
                    q != diff.end(); q++ )
            {
                // q->second->apply( context );
                dd.push_back( q->second );
            }
    }
    double t1 = tick();
    printf("Sort: %lf usec\n", (t1-t0)*1e6);
    printf("time in >> operator: %lf usec\n", optime*1e6 );
#endif


    int frame_count = 0;
    while( !done )
    {
        iiWS::WS::instance()->frame();
        double t1 = tick();
        double dt = t1 - t0;
        double hz = 1.0/dt;
        t0 = t1;
        ++frame_count;

        win->makeCurrent();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        iiDraw::DrawableAttribute::stats_clear();

        //shader->apply( context );

        shader->apply( );
        //iiMath::matrix4f V = trackball->getMatrix();
        gmtl::Matrix44f V = trackball->getMatrix();
        context.setViewMatrix( V );
        context.applyModelMatrix( iiMath::matrix4f() );



#if 0 // Front to back and Back to front sorting....
        gmtl::Matrix44f I;
        gmtl::invertAffine( I, V );
        gmtl::Point<float,4>f_eye = I * gmtl::Point<float,4>(0.0,0.0,0.0,1.0);

        iiMath::vec3 i_eye( f_eye[0], f_eye[1], f_eye[2] );

        printf("Eye %8.4f %8.4f %8.4f\r", f_eye[0], f_eye[1], f_eye[2] ); fflush(stdout);
        //std::sort( t_drawables.begin(), t_drawables.end(), CubeSorter(i_eye, CubeSorter::BackToFront) );
        //std::sort( o_drawables.begin(), o_drawables.end(), CubeSorter(i_eye, CubeSorter::FrontToBack) );
#endif

        /*
        // FIX THIS
        { 
            iiMath::matrix4f VM = V;
            iiMath::matrix4f O; 
            
            gmtl::invertFull( O, VM ); 
            iiMath::matrix3f b = gmtl::makeRot< iiMath::matrix3f >( gmtl::makeRot< gmtl::EulerAngleXYZf >( O )); 
            gmtl::transpose( b );

            shader->setUniformValue( iiDraw::ShaderProgram::NormalMatrix, b );

            iiMath::matrix4f PVM =  P * VM;
            shader->setUniformValue( iiDraw::ShaderProgram::ModelViewProjectionMatrix, b );
        }
        */

        //current_drawable = default_drawable;

        for( std::vector< sortable_ptr<iiDraw::DrawableAttribute> >::iterator p = dd.begin(); p!= dd.end(); p++ )
            (*p)->apply( context );

#if 0
        glDisable( GL_BLEND );
        for( std::vector< iiDraw::Drawable_ptr >::iterator p = o_drawables.begin(); p != o_drawables.end(); p++ )
        {
#if 0
            // 1.0 simple, no state tracking
            (*p)->apply(context);
#else


            // 2.0 THe >> operator turns out to be miserably slow
            /*
            iiDraw::Drawable * blah = p->get();
            categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> > diff = (*blah) >> current_drawable;

            for( categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> >::iterator q = diff.begin();
                    q != diff.end(); q++ )
            {
                q->second->apply( context );
            }
            */

            //iiDraw::Drawable(diff).apply( context );
            //diff.apply( context );
            //iiDraw::Drawable( diff ).apply( context );


            for( iiDraw::Drawable::iterator q = (*p)->begin(); q != (*p)->end(); q++ )
            {
                if( yy[q->second->getDrawableAttributeType()] != q->second.get() )
                {
                    yy[q->second->getDrawableAttributeType()] = q->second.get();
                    q->second->apply( context ); 
                }
            }
#endif

        }
#endif

#if 0
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        for( std::vector< iiDraw::Drawable_ptr >::iterator p = t_drawables.begin(); p != t_drawables.end(); p++ )
        {
#if 0
            // 1.0 simple, no state tracking
            (*p)->apply(context);
#else


            // 2.0 THe >> operator turns out to be miserably slow
            /*
            iiDraw::Drawable * blah = p->get();
            categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> > diff = (*blah) >> current_drawable;

            for( categorized_set<iiDraw::DrawableAttribute::DrawableAttributeType, sortable_ptr<iiDraw::DrawableAttribute> >::iterator q = diff.begin();
                    q != diff.end(); q++ )
            {
                q->second->apply( context );
            }
            */

            //iiDraw::Drawable(diff).apply( context );
            //diff.apply( context );
            //iiDraw::Drawable( diff ).apply( context );


            for( iiDraw::Drawable::iterator q = (*p)->begin(); q != (*p)->end(); q++ )
            {
                if( yy[q->second->getDrawableAttributeType()] != q->second.get() )
                {
                    yy[q->second->getDrawableAttributeType()] = q->second.get();
                    q->second->apply( context ); 
                }
            }
#endif

        }
#endif

        fr(iiMath::recti( 0, 0, wwidth, wheight ), hz );

        win->swapBuffers();
    }

    return 0;
}
