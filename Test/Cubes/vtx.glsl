uniform mat4 ii_ModelViewProjectionMatrix;
uniform mat3 ii_NormalMatrix;

attribute vec2 ii_TexCoord20;
attribute vec3 ii_Normal;
attribute vec3 ii_Vertex3;
attribute vec4 ii_Color4;

varying vec2 texcoord;

extern void ii_lighting( in vec3, in vec4 );

void main()
{
    texcoord = ii_TexCoord20;

    gl_Position = ii_ModelViewProjectionMatrix * vec4( ii_Vertex3, 1 );

    vec3 normal = normalize( ii_NormalMatrix * (ii_Normal) );

    ii_lighting(normal, ii_Color4);
}

