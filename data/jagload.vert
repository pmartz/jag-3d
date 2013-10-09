#version 140

uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
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
    vec3 ecNormal = normalize( normalMatrix * normal );
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. );
    color = vec4( ambientScene ) + vec4( diffuseMat * diffuse, 1. );

    gl_Position = viewProjectionMatrix * vertex;
} 
