#version 140

uniform mat4 jagModelViewProjMatrix;

in vec4 vertex;

void main()
{
    gl_Position = jagModelViewProjMatrix * vertex;
} 
