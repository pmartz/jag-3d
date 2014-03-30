#version 400

uniform mat4 jagModelViewProjMatrix;
uniform vec3 jag3d_bbMin;
uniform vec3 jag3d_bbMax;

layout( points, invocations = 1 ) in;
layout( line_strip, max_vertices = 18 ) out;

void main( void )
{
    vec4 mn = vec4( jag3d_bbMin, 1. );
    vec4 mx = vec4( jag3d_bbMax, 1. );
    vec4 v0 = jagModelViewProjMatrix * mn;
    vec4 v1 = jagModelViewProjMatrix * vec4( mn.x, mn.y, mx.z, 1. );
    vec4 v2 = jagModelViewProjMatrix * vec4( mn.x, mx.y, mx.z, 1. );
    vec4 v3 = jagModelViewProjMatrix * vec4( mn.x, mx.y, mn.z, 1. );
    vec4 v4 = jagModelViewProjMatrix * vec4( mx.x, mn.y, mn.z, 1. );
    vec4 v5 = jagModelViewProjMatrix * vec4( mx.x, mn.y, mx.z, 1. );
    vec4 v6 = jagModelViewProjMatrix * mx;
    vec4 v7 = jagModelViewProjMatrix * vec4( mx.x, mx.y, mn.z, 1. );

    gl_Position = v0; EmitVertex();
    gl_Position = v1; EmitVertex();
    gl_Position = v2; EmitVertex();
    gl_Position = v3; EmitVertex();
    gl_Position = v0; EmitVertex();
    EndPrimitive();

    gl_Position = v4; EmitVertex();
    gl_Position = v5; EmitVertex();
    gl_Position = v6; EmitVertex();
    gl_Position = v7; EmitVertex();
    gl_Position = v4; EmitVertex();
    EndPrimitive();

    gl_Position = v0; EmitVertex();
    gl_Position = v4; EmitVertex();
    EndPrimitive();

    gl_Position = v1; EmitVertex();
    gl_Position = v5; EmitVertex();
    EndPrimitive();

    gl_Position = v2; EmitVertex();
    gl_Position = v6; EmitVertex();
    EndPrimitive();

    gl_Position = v3; EmitVertex();
    gl_Position = v7; EmitVertex();
    EndPrimitive();
}
