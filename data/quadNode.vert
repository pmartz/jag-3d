#version 400

in vec3 vertex;
out vec2 tc;

void main()
{
    gl_Position = vec4( vertex, 1. );
    tc = ( vertex.xy + 1. ) * 0.5;
}
