#define USE_ABUFFER 1
/**
 * Fast Single-pass A-Buffer using OpenGL 4.0  V2.0
 * Copyright Cyril Crassin, July 2010
**/

#version 400
#extension GL_NV_gpu_shader5 : enable
#extension GL_EXT_shader_image_load_store : enable
#extension GL_NV_shader_buffer_load : enable
#extension GL_EXT_bindable_uniform : enable
#extension GL_NV_shader_buffer_store : enable


#line 0
/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/
#ifndef ABUFFERLINKEDLIST_HGLSL
#define ABUFFERLINKEDLIST_HGLSL

//Macros changed from the C++ side
#define BACKGROUND_COLOR_R 1.000000f
#define BACKGROUND_COLOR_G 1.000000f
#define BACKGROUND_COLOR_B 1.000000f

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

uniform int screenWidth;
uniform int screenHeight;


//Access functions
#if ABUFFER_USE_TEXTURES

bool semaphoreAcquire(ivec2 coords){
        return imageAtomicExchange(semaphoreImg, coords, 1U)==0U;
}
void semaphoreRelease(ivec2 coords){
        imageAtomicExchange(semaphoreImg, coords, 0U);
}
bool getSemaphore(ivec2 coords){
        return imageLoad(semaphoreImg, coords).x==0U;
}
void setSemaphore(ivec2 coords, bool val){
        imageStore(semaphoreImg, coords, uvec4(val ? 1U : 0U, 0U, 0U, 0U));
}

uint getPixelCurrentPage(ivec2 coords){
        return imageLoad(abufferPageIdxImg, coords).x;
}
void setPixelCurrentPage(ivec2 coords, uint newpageidx){
        imageStore(abufferPageIdxImg, coords, uvec4(newpageidx, 0U, 0U, 0U) );
}

uint getPixelFragCounter(ivec2 coords){
        return imageLoad(abufferFragCountImg, coords).x;
}
void setPixelFragCounter(ivec2 coords, uint val){
        imageStore(abufferFragCountImg, coords, uvec4(val, 0U, 0U, 0U) );
}
uint pixelFragCounterAtomicAdd(ivec2 coords, uint val){
        return imageAtomicAdd(abufferFragCountImg, coords, val);
}

#else

bool semaphoreAcquire(ivec2 coords){
        return atomicExchange(d_semaphore+coords.x+coords.y*screenWidth, 0U) != 0U;
}
void semaphoreRelease(ivec2 coords){
        atomicExchange(d_semaphore+coords.x+coords.y*screenWidth, 1U);
}
bool getSemaphore(ivec2 coords){
        return d_semaphore[coords.x+coords.y*screenWidth]==0U;
}
void setSemaphore(ivec2 coords, bool val){
        d_semaphore[coords.x+coords.y*screenWidth]=val ? 0U : 1U;
}

uint getPixelCurrentPage(ivec2 coords){
        return d_abufferPageIdx[coords.x+coords.y*screenWidth];
}
void setPixelCurrentPage(ivec2 coords, uint newpageidx){
        d_abufferPageIdx[coords.x+coords.y*screenWidth]=newpageidx;
}

uint getPixelFragCounter(ivec2 coords){
        return d_abufferFragCount[coords.x+coords.y*screenWidth];
}
void setPixelFragCounter(ivec2 coords, uint val){
        d_abufferFragCount[coords.x+coords.y*screenWidth]=val;
}
uint pixelFragCounterAtomicAdd(ivec2 coords, uint val){
        return atomicAdd(d_abufferFragCount+coords.x+coords.y*screenWidth, val);
}

#endif


#if SHAREDPOOL_USE_TEXTURES
uint sharedPoolGetLink(uint pageNum){
        return imageLoad(sharedLinkListImg, (int)(pageNum) ).x;
}
void sharedPoolSetLink(uint pageNum, uint pointer){
        imageStore(sharedLinkListImg, (int)(pageNum), uvec4(pointer, 0U, 0U, 0U) );
}

vec4 sharedPoolGetValue(uint index){
        return imageLoad(sharedPageListImg, (int)index);
}
void sharedPoolSetValue(uint index, vec4 val){
        imageStore(sharedPageListImg, (int)(index), val);
}
#else
uint sharedPoolGetLink(uint pageNum){
        return d_sharedLinkList[(int)pageNum];
}
void sharedPoolSetLink(uint pageIdx, uint pointer){
        d_sharedLinkList[(int)pageIdx]=pointer;
}

vec4 sharedPoolGetValue(uint index){
        return d_sharedPageList[(int)index];
}
void sharedPoolSetValue(uint index, vec4 val){
        d_sharedPageList[(int)index]=val;
}
#endif


void setSharedPageCounter(uint val){
        (*d_curSharedPage)=val;
}
uint sharedPageCounterAtomicAdd(uint val){
        return atomicAdd(d_curSharedPage, val);
}

#endif  //ABUFFERLINKEDLIST_HGLSL
#line 12


//Whole number pixel offsets (not necessary just to test the layout keyword !)
layout(pixel_center_integer) in vec4 gl_FragCoord;


void main(void) {

        ivec2 coords=ivec2(gl_FragCoord.xy);

        if(coords.x>=0 && coords.y>=0 && coords.x<screenWidth && coords.y<screenHeight )
        {
                setPixelCurrentPage(coords, 0U);
                setPixelFragCounter(coords, 0U);
                setSemaphore(coords, false);

                //sharedPoolSetLink(0, 0U);
                //sharedPoolSetValue(0, vec4(0.0f, 0.0f, 4.0f, 1.0f));

                setSharedPageCounter(ABUFFER_PAGE_SIZE);

        }

        //Discard fragment so nothing is writen to the framebuffer
        discard;
}
