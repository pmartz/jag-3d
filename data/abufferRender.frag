#define USE_ABUFFER 1
/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/

#version 400
#extension GL_NV_gpu_shader5 : enable
#extension GL_EXT_shader_image_load_store : enable
#extension GL_EXT_bindable_uniform : enable
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_shader_buffer_store : enable


/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/
#ifndef ABUFFERLINKEDLIST_HGLSL
#define ABUFFERLINKEDLIST_HGLSL

//Macros changed from the C++ side
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BACKGROUND_COLOR_R 1.000000f
#define BACKGROUND_COLOR_G 1.000000f
#define BACKGROUND_COLOR_B 1.000000f

#define ABUFFER_SIZE 16
#define ABUFFER_PAGE_SIZE 4

#define ABUFFER_USE_TEXTURES 0
#define SHAREDPOOL_USE_TEXTURES 0

#define ABUFFER_RESOLVE_USE_SORTING 1
#define ABUFFER_RESOLVE_ALPHA_CORRECTION 1
#define ABUFFER_RESOLVE_GELLY 1

#define ABUFFER_DISPNUMFRAGMENTS 0

uniform int abufferSharedPoolSize;

#if ABUFFER_USE_TEXTURES
//A-Buffer fragments storage
coherent uniform layout(size1x32) uimage2D abufferPageIdxImg;
//A-Buffer fragment counter
coherent uniform layout(size1x32) uimage2D abufferFragCountImg;
//per-pixel semaphore
coherent uniform layout(size1x32) uimage2D semaphoreImg;
#else
coherent uniform uint *d_abufferPageIdx;
coherent uniform uint *d_abufferFragCount;
coherent uniform uint *d_semaphore;
#endif

#if SHAREDPOOL_USE_TEXTURES
//A-Buffer shared fragment pages buffer
coherent uniform layout(size4x32) imageBuffer sharedPageListImg;
//A-Buffer shared link list
coherent uniform layout(size1x32) uimageBuffer sharedLinkListImg;
#else
coherent uniform uint *d_sharedLinkList;
coherent uniform vec4 *d_sharedPageList;
#endif


//Next available page in the shared pool
coherent uniform uint *d_curSharedPage;


//Access functions
#if ABUFFER_USE_TEXTURES

bool semaphoreAcquire(ivec2 coords)
{
    return imageAtomicExchange(semaphoreImg, coords, 1U)==0U;
}
void semaphoreRelease(ivec2 coords)
{
    imageAtomicExchange(semaphoreImg, coords, 0U);
}
bool getSemaphore(ivec2 coords)
{
    return imageLoad(semaphoreImg, coords).x==0U;
}
void setSemaphore(ivec2 coords, bool val)
{
    imageStore(semaphoreImg, coords, uvec4(val ? 1U : 0U, 0U, 0U, 0U));
}

uint getPixelCurrentPage(ivec2 coords)
{
    return imageLoad(abufferPageIdxImg, coords).x;
}
void setPixelCurrentPage(ivec2 coords, uint newpageidx)
{
    imageStore(abufferPageIdxImg, coords, uvec4(newpageidx, 0U, 0U, 0U) );
}

uint getPixelFragCounter(ivec2 coords)
{
    return imageLoad(abufferFragCountImg, coords).x;
}
void setPixelFragCounter(ivec2 coords, uint val)
{
    imageStore(abufferFragCountImg, coords, uvec4(val, 0U, 0U, 0U) );
}
uint pixelFragCounterAtomicAdd(ivec2 coords, uint val)
{
    return imageAtomicAdd(abufferFragCountImg, coords, val);
}

#else

bool semaphoreAcquire(ivec2 coords)
{
    return atomicExchange(d_semaphore+coords.x+coords.y*SCREEN_WIDTH, 0U) != 0U;
}
void semaphoreRelease(ivec2 coords)
{
    atomicExchange(d_semaphore+coords.x+coords.y*SCREEN_WIDTH, 1U);
}
bool getSemaphore(ivec2 coords)
{
    return d_semaphore[coords.x+coords.y*SCREEN_WIDTH]==0U;
}
void setSemaphore(ivec2 coords, bool val)
{
    d_semaphore[coords.x+coords.y*SCREEN_WIDTH]=val ? 0U : 1U;
}

uint getPixelCurrentPage(ivec2 coords)
{
    return d_abufferPageIdx[coords.x+coords.y*SCREEN_WIDTH];
}
void setPixelCurrentPage(ivec2 coords, uint newpageidx)
{
    d_abufferPageIdx[coords.x+coords.y*SCREEN_WIDTH]=newpageidx;
}

uint getPixelFragCounter(ivec2 coords)
{
    return d_abufferFragCount[coords.x+coords.y*SCREEN_WIDTH];
}
void setPixelFragCounter(ivec2 coords, uint val)
{
    d_abufferFragCount[coords.x+coords.y*SCREEN_WIDTH]=val;
}
uint pixelFragCounterAtomicAdd(ivec2 coords, uint val)
{
    return atomicAdd(d_abufferFragCount+coords.x+coords.y*SCREEN_WIDTH, val);
}

#endif


#if SHAREDPOOL_USE_TEXTURES
uint sharedPoolGetLink(uint pageNum)
{
    return imageLoad(sharedLinkListImg, (int)(pageNum) ).x;
}
void sharedPoolSetLink(uint pageNum, uint pointer)
{
    imageStore(sharedLinkListImg, (int)(pageNum), uvec4(pointer, 0U, 0U, 0U) );
}

vec4 sharedPoolGetValue(uint index)
{
    return imageLoad(sharedPageListImg, (int)index);
}
void sharedPoolSetValue(uint index, vec4 val)
{
    imageStore(sharedPageListImg, (int)(index), val);
}
#else
uint sharedPoolGetLink(uint pageNum)
{
    return d_sharedLinkList[(int)pageNum];
}
void sharedPoolSetLink(uint pageIdx, uint pointer)
{
    d_sharedLinkList[(int)pageIdx]=pointer;
}

vec4 sharedPoolGetValue(uint index)
{
    return d_sharedPageList[(int)index];
}
void sharedPoolSetValue(uint index, vec4 val)
{
    d_sharedPageList[(int)index]=val;
}
#endif


void setSharedPageCounter(uint val)
{
    (*d_curSharedPage)=val;
}
uint sharedPageCounterAtomicAdd(uint val)
{
    return atomicAdd(d_curSharedPage, val);
}

#endif  //ABUFFERLINKEDLIST_HGLSL


//Whole number pixel offsets (not necessary, just to test the layout keyword !)
layout(pixel_center_integer) in vec4 gl_FragCoord;

smooth in vec4 fragPos;
smooth in vec3 fragTexCoord;
smooth in vec3 fragNormal;


//Shade using green-white strips
vec3 shadeStrips(vec3 texcoord)
{
    vec3 col;
    float i=floor(texcoord.x*6.0f);

    col.rgb=fract(i*0.5f) == 0.0f ? vec3(0.4f, 0.85f, 0.0f) : vec3(1.0f);
    col.rgb*=texcoord.z;

    return col;
}


const vec4 lightModelAmbient = vec4( 0., 0., 0., 0. );
smooth in vec4 ecVertex;

uniform LightingLight {
    vec4 position;
    vec4 ambient, diffuse, specular;
} light;
uniform LightingMaterialFront {
    vec4 ambient, diffuse, specular;
    float shininess;
} front;

struct Products {
    vec4 ambient, diffuse, specular;
};
in Products frontProduct;

// 'normal' must be normalized.
vec4 lighting( in vec4 ambProd, in vec4 diffProd, in vec4 specProd, in float specExp,
    in vec3 viewVec, in vec3 normal, in vec3 lightVec )
{
    float diffDot = dot( normal, lightVec );
    vec4 diff = diffProd * max( diffDot, 0. );

    vec4 spec = vec4( 0., 0., 0., 0. );
    if( ( specExp > 0. ) && ( diffDot > 0. ) )
    {
        vec3 reflectVec = reflect( lightVec, normal ); // lightVec and normal are already normalized,
        spec = specProd * pow( max( dot( reflectVec, viewVec ), 0. ), specExp );
    }

    return( ambProd + diff + spec );
}


// Opaque pass depth buffer.
uniform sampler2D depthBuffer;


#if USE_ABUFFER

void main(void)
{
    ivec2 coords=ivec2(gl_FragCoord.xy);

    //Check we are in the framebuffer
    if(coords.x>=0 && coords.y>=0 && coords.x<SCREEN_WIDTH && coords.y<SCREEN_HEIGHT )
    {
        // Discard immediately if obscured by opawue geometry in the depth buffer.
        vec3 tc = ( ( fragPos.xyz / fragPos.w ) + 1.f ) * 0.5f;
        float depthVal = texture2D( depthBuffer, tc.xy ).r;
        if( tc.z > depthVal )
            discard;

        //Get current page
        uint curPage=0;
        uint curFragIdx=0;

        ///Manual critical section => This can most probably be optimized !
        int ii=0; //prevents infinite loops
        bool leaveLoop = false;
        while (!leaveLoop && ii<1000)
        {
            //Aquire semaphore
            if ( semaphoreAcquire(coords) )
            {
                //Get current page
                curPage=getPixelCurrentPage(coords);

                //Get fragment counter
                curFragIdx=getPixelFragCounter(coords);
                uint curFragIdxMod=curFragIdx%ABUFFER_PAGE_SIZE;

                if(curFragIdxMod==0 )
                {
                    uint newpageidx=sharedPageCounterAtomicAdd(ABUFFER_PAGE_SIZE);

                    if(newpageidx<abufferSharedPoolSize)
                    {
                        //Create link
                        sharedPoolSetLink(newpageidx/ABUFFER_PAGE_SIZE, curPage);

                        //Save new page idx
                        setPixelCurrentPage(coords, newpageidx);

                        curPage=newpageidx;
                    }
                    else
                    {
                        curPage=0;
                    }
                }

                if(curPage>0)
                {
                    setPixelFragCounter(coords, curFragIdx+1);
                }

                curFragIdx=curFragIdxMod;

                leaveLoop = true;

                //memoryBarrier(); //We theoretically need it here, but not implemented in current drivers !
                semaphoreRelease(coords);
            }

            ii++;
        }


        //Create fragment to be stored
        vec4 abuffval;
        vec3 col;

        //Choose what we store per fragment
#if 1
        vec3 lightVec = light.position.xyz;
        if( light.position.w > 0.001 )
            lightVec = normalize( lightVec - ecVertex.xyz );
        vec3 viewVec = normalize( ecVertex.xyz );
        vec3 normal = normalize( fragNormal );
        col = lighting( frontProduct.ambient + lightModelAmbient,
            frontProduct.diffuse, frontProduct.specular,
            front.shininess, viewVec, normal, lightVec ).rgb;
#elif ABUFFER_RESOLVE_GELLY==0
        //Store color strips
        col=shadeStrips(fragTexCoord);
#else
        //Store pseudo-illumination info
        vec3 N = normalize(fragNormal);
        vec3 L = normalize(vec3(0.0f,1.0f,1.0f));
        col = vec3(dot(N,L));
#endif

        abuffval.rgb=col;
        abuffval.w=fragPos.z;   //Will be used for sorting

        //Put fragment into the  page in the shared pool
        sharedPoolSetValue(curPage+curFragIdx, abuffval);

        //Do not discard the fragment if a page failled to be allocated
        if(curPage!=0)
            discard;
    }
    else
    {
        discard;
    }
}

#else   //#if USE_ABUFFER

out vec4 outFragColor;

void main(void)
{
    vec3 col=shadeStrips(fragTexCoord);
    outFragColor=vec4(col, 1.0f);
}

#endif  //#if USE_ABUFFER
