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
 * Fast Single-pass A-Buffer using OpenGL 4.0 V2.0
 * Copyright Cyril Crassin, July 2010
**/

#version 400

uniform mat4 projectionMat;
uniform mat4 modelViewMat;
uniform mat4 modelViewMatIT;

uniform LightingLight {
    vec4 position;
    vec4 ambient, diffuse, specular;
} light;
uniform LightingMaterialFront {
    vec4 ambient, diffuse, specular;
    float shininess;
} front;

in vec3 vertexPos;
in vec3 vertexNormal;

smooth out vec4 ecVertex;
smooth out vec4 fragPos;
smooth out vec3 fragTexCoord;

smooth out vec3 fragNormal;

struct Products {
    vec4 ambient, diffuse, specular;
};
out Products frontProduct;


void main()
{
    ecVertex = modelViewMat * vec4( vertexPos.xyz, 1.0f );
    vec4 pos = projectionMat * ecVertex;

    vec3 normalEye = normalize( (modelViewMatIT*vec4(vertexNormal, 1.0f)).xyz );

    fragTexCoord.xy = vertexPos.xy;
    fragTexCoord.z = abs( normalEye.z );

    fragNormal = normalEye;
    fragPos = pos;
    gl_Position = pos;

    frontProduct.ambient = front.ambient * light.ambient;
    frontProduct.diffuse = front.diffuse * light.diffuse;
    frontProduct.specular = front.specular * light.specular;
}
