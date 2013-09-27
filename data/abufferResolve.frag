#define USE_ABUFFER 1
/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/

#version 400
#extension GL_NV_gpu_shader5 : enable
#extension GL_EXT_shader_image_load_store : enable
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_shader_buffer_store : enable
#extension GL_EXT_bindable_uniform : enable


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
        return atomicExchange(d_semaphore+coords.x+coords.y*SCREEN_WIDTH, 0U) != 0U;
}
void semaphoreRelease(ivec2 coords){
        atomicExchange(d_semaphore+coords.x+coords.y*SCREEN_WIDTH, 1U);
}
bool getSemaphore(ivec2 coords){
        return d_semaphore[coords.x+coords.y*SCREEN_WIDTH]==0U;
}
void setSemaphore(ivec2 coords, bool val){
        d_semaphore[coords.x+coords.y*SCREEN_WIDTH]=val ? 0U : 1U;
}

uint getPixelCurrentPage(ivec2 coords){
        return d_abufferPageIdx[coords.x+coords.y*SCREEN_WIDTH];
}
void setPixelCurrentPage(ivec2 coords, uint newpageidx){
        d_abufferPageIdx[coords.x+coords.y*SCREEN_WIDTH]=newpageidx;
}

uint getPixelFragCounter(ivec2 coords){
        return d_abufferFragCount[coords.x+coords.y*SCREEN_WIDTH];
}
void setPixelFragCounter(ivec2 coords, uint val){
        d_abufferFragCount[coords.x+coords.y*SCREEN_WIDTH]=val;
}
uint pixelFragCounterAtomicAdd(ivec2 coords, uint val){
        return atomicAdd(d_abufferFragCount+coords.x+coords.y*SCREEN_WIDTH, val);
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


/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/
#ifndef ABUFFERSORT_HGLSL
#define ABUFFERSORT_HGLSL

//Local memory array (probably in L1)
vec4 fragmentList[ABUFFER_SIZE];


//Bubble sort used to sort fragments
void bubbleSort(int array_size);
//Bitonic sort test
void bitonicSort( int n );


//Bubble sort used to sort fragments
void bubbleSort(int array_size) {
  for (int i = (array_size - 2); i >= 0; --i) {
    for (int j = 0; j <= i; ++j) {
      if (fragmentList[j].w > fragmentList[j+1].w) {
                vec4 temp = fragmentList[j+1];
                fragmentList[j+1] = fragmentList[j];
                fragmentList[j] = temp;
      }
    }
  }
}

//Swap function
void swapFragArray(int n0, int n1){
        vec4 temp = fragmentList[n1];
        fragmentList[n1] = fragmentList[n0];
        fragmentList[n0] = temp;
}

//->Same artefact than in L.Bavoil
//Bitonic sort: http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm
void bitonicSort( int n ) {
        int i,j,k;
        for (k=2;k<=n;k=2*k) {
                for (j=k>>1;j>0;j=j>>1) {
                        for (i=0;i<n;i++) {
                          int ixj=i^j;
                          if ((ixj)>i) {
                                  if ((i&k)==0 && fragmentList[i].w>fragmentList[ixj].w){
                                        swapFragArray(i, ixj);
                                  }
                                  if ((i&k)!=0 && fragmentList[i].w<fragmentList[ixj].w) {
                                        swapFragArray(i, ixj);
                                  }
                          }
                        }
                }
        }
}


#endif


/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/
#ifndef ABUFFERSHADING_HGLSL
#define ABUFFERSHADING_HGLSL


/**
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/
#ifndef ABUFFERSORT_HGLSL
#define ABUFFERSORT_HGLSL

//Local memory array (probably in L1)
vec4 fragmentList[ABUFFER_SIZE];


//Bubble sort used to sort fragments
void bubbleSort(int array_size);
//Bitonic sort test
void bitonicSort( int n );


//Bubble sort used to sort fragments
void bubbleSort(int array_size) {
  for (int i = (array_size - 2); i >= 0; --i) {
    for (int j = 0; j <= i; ++j) {
      if (fragmentList[j].w > fragmentList[j+1].w) {
                vec4 temp = fragmentList[j+1];
                fragmentList[j+1] = fragmentList[j];
                fragmentList[j] = temp;
      }
    }
  }
}

//Swap function
void swapFragArray(int n0, int n1){
        vec4 temp = fragmentList[n1];
        fragmentList[n1] = fragmentList[n0];
        fragmentList[n0] = temp;
}

//->Same artefact than in L.Bavoil
//Bitonic sort: http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm
void bitonicSort( int n ) {
        int i,j,k;
        for (k=2;k<=n;k=2*k) {
                for (j=k>>1;j>0;j=j>>1) {
                        for (i=0;i<n;i++) {
                          int ixj=i^j;
                          if ((ixj)>i) {
                                  if ((i&k)==0 && fragmentList[i].w>fragmentList[ixj].w){
                                        swapFragArray(i, ixj);
                                  }
                                  if ((i&k)!=0 && fragmentList[i].w<fragmentList[ixj].w) {
                                        swapFragArray(i, ixj);
                                  }
                          }
                        }
                }
        }
}


#endif


// Don't use this. Get background from opaqueBuffer texture.
//const vec4 backgroundColor=vec4(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 0.0f);
const float fragmentAlpha=0.5f;

//Blend fragments front-to-back
vec4 resolveAlphaBlend(int abNumFrag);
//Compute gelly shader
vec4 resolveGelly(int abNumFrag);


//Blend fragments front-to-back
vec4 resolveAlphaBlend(int abNumFrag, vec4 backgroundColor ){

        vec4 finalColor=vec4(0.0f);

        const float sigma = 30.0f;
        float thickness=fragmentList[0].w/2.0f;

        finalColor=vec4(0.0f);
        for(int i=0; i<abNumFrag; i++){
                vec4 frag=fragmentList[i];

                vec4 col;
                col.rgb=frag.rgb;
                col.w=fragmentAlpha;    //uses constant alpha

#if ABUFFER_RESOLVE_ALPHA_CORRECTION
                if(i%2==abNumFrag%2)
                        thickness=(fragmentList[i+1].w-frag.w)*0.5f;
                col.w=1.0f-pow(1.0f-col.w, thickness* sigma );
#endif

                col.rgb=col.rgb*col.w;

                finalColor=finalColor+col*(1.0f-finalColor.a);
        }

        finalColor=finalColor+backgroundColor*(1.0f-finalColor.a);

        return finalColor;

}

//Blend fragments front-to-back
vec4 resolveGelly(int abNumFrag, vec4 backgroundColor ){

        float thickness=0.0f;
        vec4 accumColor= backgroundColor; //vec4(0.0f);

        vec4 prevFrag;
        for(int i=0; i<abNumFrag; i++){
                vec4 frag=fragmentList[i];

                if(i%2==1){
                        thickness+=frag.w-prevFrag.w;
                }

                vec4 col;
                col.rgb=frag.rgb;
                col.w=fragmentAlpha;    //uses constant alpha

                col.rgb=col.rgb*col.w;
                accumColor=accumColor+col*(1.0f-accumColor.a);

                prevFrag=frag;
        }
        accumColor=accumColor+backgroundColor*(1.0f-accumColor.a);


        //thickness=fragmentList[abNumFrag-1].w-fragmentList[0].w;
        float sigma = 20.0f;
        float Ia = exp(-sigma*thickness);
        float ka = 0.9f;

        vec4 finalColor=vec4(0.0f);
        //finalColor = ka * Ia + (1.0-ka) * fragmentList[0]; //Same as Louis Bavoil 2005
        finalColor = ka * Ia + (1.0-ka) * accumColor;   //Uses accumulated Color

        const vec4 jade = vec4(0.4f, 0.14f, 0.11f, 1.0f)* 8.0f;
        const vec4 green = vec4(0.3f, 0.7f, 0.1f, 1.0f)* 4.0f;
        const vec4 green2 = vec4(0.14f, 0.45f, 0.05f, 1.0f)* 8.0f;
        finalColor *= green2 ;

        return finalColor;
}

#endif



//Whole number pixel offsets (not necessary just to test the layout keyword !)
layout(pixel_center_integer) in vec4 gl_FragCoord;

//Input interpolated fragment position
smooth in vec4 fragPos;
//Output fragment color
out vec4 outFragColor;

//Keeps only closest fragment
vec4 resolveClosest(int pageIdx, int abNumFrag);
//Fill local memory array of fragments
void fillFragmentArray(int pageIdx, int abNumFrag);


// Opaque color buffer.
uniform sampler2D opaqueBuffer;

// Texture coordinates for opaque color buffer.
smooth in vec2 tc;


//Resolve A-Buffer and blend sorted fragments
void main(void)
 {
    ivec2 coords=ivec2(gl_FragCoord.xy);

    if(coords.x>=0 && coords.y>=0 && coords.x<SCREEN_WIDTH && coords.y<SCREEN_HEIGHT )
    {
        vec4 backgroundColor = vec4( texture2D( opaqueBuffer, tc ).rgb, 0.f );

        int pageIdx=(int)getPixelCurrentPage(coords);

        if(pageIdx > 0 )
        {
            //Load the number of fragments in the last page.
            int abNumFrag=(int)getPixelFragCounter(coords);


            if(abNumFrag>0)
            {
#if ABUFFER_DISPNUMFRAGMENTS==0

                //Copute ans output final color for the frame buffer
#   if ABUFFER_RESOLVE_USE_SORTING==0
                //If we only want the closest fragment
                outFragColor=resolveClosest(pageIdx, abNumFrag);
#   else
                //Copy fragments in local array
                fillFragmentArray(pageIdx, abNumFrag);
                abNumFrag=min(abNumFrag, ABUFFER_SIZE);
                //Sort fragments in local memory array
                bubbleSort(abNumFrag);

#       if ABUFFER_RESOLVE_GELLY
                //We want to sort and apply gelly shader
                outFragColor=resolveGelly(abNumFrag, backgroundColor );
#       else
                //We want to sort and blend fragments
                outFragColor=resolveAlphaBlend(abNumFrag, backgroundColor );
#       endif

#   endif

#else
                outFragColor=vec4(abNumFrag/float(ABUFFER_SIZE));
#endif

            }
            else
            {
                // No abuffer fragments. Just pait the background color
                // from the opaque pass.
                outFragColor = backgroundColor;
            }
        }
        else
        {
            // pageIdx == 0 indicates nothing was marked as transparent
            // so nothing in the abuffer. Just pait the background color
            // from the opaque pass.
            outFragColor = backgroundColor;
        }
    }
}



vec4 resolveClosest(int pageIdx, int abNumFrag){

        int curPage=pageIdx;

        //Search smallest z
        vec4 minFrag=vec4(0.0f, 0.0f, 0.0f, 1000000.0f);
        int ip=0;

        while(curPage!=0 && ip<20){
                int numElem;
                if(ip==0){
                        numElem=abNumFrag%(ABUFFER_PAGE_SIZE);
                        if(numElem==0)
                                numElem=ABUFFER_PAGE_SIZE;
                }else{
                        numElem=ABUFFER_PAGE_SIZE;
                }

                for(int i=0; i<numElem; i++){
                        vec4 val=sharedPoolGetValue(curPage+i);

                        if(val.w<minFrag.w){
                                minFrag=val;
                        }

                }

                //Get next page index
                curPage=(int)sharedPoolGetLink(curPage/ABUFFER_PAGE_SIZE);

                ip++;
        }
        //Output final color for the frame buffer
        return minFrag;
}



void fillFragmentArray(int pageIdx, int abNumFrag){
        //Load fragments into a local memory array for sorting

        int curPage=pageIdx;
        int ip=0;
        int fi=0;
        while(curPage!=0 && ip<20){
                int numElem;
                if(ip==0){
                        numElem=abNumFrag%(ABUFFER_PAGE_SIZE);
                        if(numElem==0)
                                numElem=ABUFFER_PAGE_SIZE;
                }else{
                        numElem=ABUFFER_PAGE_SIZE;
                }

                for(int i=0; i<numElem; i++){
                        if(fi<ABUFFER_SIZE){
                                fragmentList[fi]=sharedPoolGetValue(curPage+i);
                        }
                        fi++;
                }


                curPage=(int)sharedPoolGetLink(curPage/ABUFFER_PAGE_SIZE);

                ip++;
        }

}
