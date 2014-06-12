#version 400

uniform mat4 jagModelViewProjMatrix;


in vec4 vertex;


void main( void )
{
    gl_Position = jagModelViewProjMatrix * vertex;
}
