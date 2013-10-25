#define ABUFFER_DISPNUMFRAGMENTS 0
#define ABUFFER_PAGE_SIZE 4
#define ABUFFER_RESOLVE_GELLY 1
#define ABUFFER_RESOLVE_ALPHA_CORRECTION 1
#define ABUFFER_RESOLVE_USE_SORTING 1
#define SHAREDPOOL_USE_TEXTURES 0
#define ABUFFER_USE_TEXTURES 0
#define USE_ABUFFER 1
#define BACKGROUND_COLOR_B 1.000000f
#define BACKGROUND_COLOR_G 1.000000f
#define BACKGROUND_COLOR_R 1.000000f
/**
 * Fast Single-pass A-Buffer using OpenGL 4.0
 * Copyright Cyril Crassin, June 2010
**/

#version 400


in vec4 vertexPos;

smooth out vec4 fragPos;
smooth out vec2 tc;


void main()
{
    tc = ( vertexPos.xy + 1.f ) * 0.5f;
    fragPos = vertexPos;
    gl_Position = vertexPos;
}
