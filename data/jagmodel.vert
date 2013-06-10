#version 140

uniform mat4 jagModelViewProjMatrix;
uniform mat3 jagModelViewInvTransMatrix;

uniform vec3 ecLightDir;
uniform blockTest {
    float ambientScene;
    vec3 diffuseMat;
};

in vec4 vertex;
in vec3 normal;
out vec4 color;

void main()
{
    vec3 ecNormal = normalize( jagModelViewInvTransMatrix * normal );
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. );
    color = vec4( vec3( ambientScene ) + ( diffuseMat * diffuse ), 1. );

    gl_Position = jagModelViewProjMatrix * vertex;
} 
